#include "card.h"
#include <iostream>
#include <cmath>
#include <cstring>
Card* Card::card = nullptr;

std::barrier recordDispenserBarrier(2,[]() noexcept {});//to resume the dispenser 
Card::Card():diffSum(absClock), pidTimer(absClock) {
	Card::card = this;
	initCard();
	
	//init transfert in now called later by the core
	//initTransfert();
}
Card::~Card() {
	spcm_vClose(hDrv);
	free(buffer);
}
int Card::initCard(){


	int32 lCardType; // a place to store card information
	hDrv = spcm_hOpen("/dev/spcm0"); // Opens the board and gets a handle
	if (!hDrv) // check whether we can access the card
		return -1;

	spcm_dwGetParam_i32(hDrv, SPC_PCITYP, &lCardType); // simple command, read out of card type
	printf("Found card M2i / M3i / M4i / M4x / M2p. % 04x in the system\n", lCardType & TYP_VERSIONMASK);


	//card reset
	spcm_dwSetParam_i32(hDrv, SPC_M2CMD, M2CMD_CARD_RESET);

	//clock (sample rate)
	spcm_dwSetParam_i32(hDrv, SPC_CLOCKMODE, SPC_CM_INTPLL);
	spcm_dwSetParam_i32(hDrv, SPC_CLOCKOUT, 0);
	spcm_dwSetParam_i64(hDrv, SPC_SAMPLERATE, SAMPLE_RATE);
	if (spcm_dwGetErrorInfo_i32(hDrv, NULL, NULL, szErrorText) != ERR_OK) // check for an error
	{
		printf(szErrorText); // print the error text
		spcm_vClose(hDrv); // close the driver
		exit(0); // and leave the program
	}

	//channel selection 
	spcm_dwSetParam_i64(hDrv, SPC_CHENABLE, CHANNEL0|CHANNEL1|CHANNEL2|CHANNEL3);

	//output enable
	spcm_dwSetParam_i32(hDrv, SPC_ENABLEOUT0, 1);
	spcm_dwSetParam_i32(hDrv, SPC_ENABLEOUT1, 1);
	spcm_dwSetParam_i32(hDrv, SPC_ENABLEOUT2, 1);
	spcm_dwSetParam_i32(hDrv, SPC_ENABLEOUT3, 1);

	int lActivatedChannels;
	int lChCount;
	spcm_dwGetParam_i32(hDrv, SPC_CHENABLE, &lActivatedChannels);
	spcm_dwGetParam_i32(hDrv, SPC_CHCOUNT, &lChCount);
	printf("Activated channels bitmask is: 0x%08x\n", lActivatedChannels);
	printf("Number of activated channels with this bitmask: %d\n", lChCount);

	//max amplitude
	spcm_dwSetParam_i32(hDrv, SPC_AMP0, 300);
	spcm_dwSetParam_i32(hDrv, SPC_AMP1, 300);
	spcm_dwSetParam_i32(hDrv, SPC_AMP2, 200);
	spcm_dwSetParam_i32(hDrv, SPC_AMP3, 300);
	if (spcm_dwGetErrorInfo_i32(hDrv, NULL, NULL, szErrorText) != ERR_OK) // check for an error
	{
		printf(szErrorText); // print the error text
		spcm_vClose(hDrv); // close the driver
		exit(0); // and leave the program
	}
	//freq filters
	//spcm_dwSetParam_i32(hDrv, SPC_FILTER0, 0);

	//I/O
	//input
	spcm_dwSetParam_i32 (hDrv, SPCM_X0_MODE, SPCM_XMODE_ASYNCIN);
	spcm_dwSetParam_i32 (hDrv, SPCM_XX_ASYNCIO, 0);

	//enable FIFO
	spcm_dwSetParam_i32(hDrv, SPC_CARDMODE, SPC_REP_FIFO_SINGLE);
	//spcm_dwSetParam_i32(hDrv, SPC_SEGMENTSIZE, 16*10);//segment size

	spcm_dwSetParam_i64(hDrv, SPC_LOOPS, 0);//no loops

 	spcm_dwSetParam_i64(hDrv, SPC_DATA_OUTBUFSIZE,HW_BUFFER);
	spcm_dwSetParam_i32(hDrv, SPC_M2CMD, M2CMD_CARD_WRITESETUP);

	if (spcm_dwGetErrorInfo_i32(hDrv, NULL, NULL, szErrorText) != ERR_OK) // check for an error
	{
		printf(szErrorText); // print the error text
		spcm_vClose(hDrv); // close the driver
		exit(0); // and leave the program
	}
	printf("setup complete\n");

	return 0;
}

void Card::initTransfert(){


		//inspired by example of the manual
		//creating the buffer
		printf("hardware buffer max is %ld bytes\n",bufsizeInSamples*2);
		buffer = (int16*)malloc(bufsizeInSamples * sizeof(int16)); 

		for (int i=0;i<bufsizeInSamples;i++){
			buffer[i]=15000;//initialize buffer//DEBUG non zero to see the diff
		}
		
		std::this_thread::yield();
		//start the record dispenser
		recordDispenserBarrier.arrive_and_wait();
		
 		spcm_dwDefTransfer_i64(hDrv, SPCM_BUF_DATA, SPCM_DIR_PCTOCARD, NOTIF_SIZE,
			(void*)buffer, 0, 2 * bufsizeInSamples);

		printf("wait dispenser\n");
		//wait for the record dispenser to fill the buffer
		recordDispenserBarrier.arrive_and_wait();
		printf("end wait dispenser\n");

		//load 1/2buffer 
		spcm_dwSetParam_i64(hDrv, SPC_DATA_AVAIL_CARD_LEN, bufsizeInSamples);
		spcm_dwSetParam_i32(hDrv, SPC_M2CMD, M2CMD_DATA_STARTDMA);
		spcm_dwSetParam_i32(hDrv, SPC_M2CMD, M2CMD_DATA_WAITDMA);
		
		//debug
		spcm_dwSetParam_i64(hDrv, SPC_DATA_AVAIL_CARD_LEN, 1*bufsizeInSamples/3);
}
void Card::start(){
	//input async
	//asyncInputThread = std::thread(&Card::asyncReadInput,this);
	
	//TODO switch to a triggered start (depending on setting) and do a reset after sequence end
	spcm_dwSetParam_i32(hDrv, SPC_M2CMD, M2CMD_CARD_START | M2CMD_CARD_FORCETRIGGER);
	timerEstimator = startTimer(); 

}
timespec Card::startTimer(){
	timespec time;
	clock_gettime(CLOCK_MONOTONIC,&time);
	return time;
}
long Card::timespec_to_long(timespec time){
	return (long)time.tv_sec*1'000'000 + time.tv_nsec/1000 ;
}

long Card::getTimer(timespec timer){
	return timespec_to_long(Card::startTimer()) - timespec_to_long(timer);
}


//asyncrhone clock synchronisation
void Card::syncClockAsync(){
	while(1) syncClock();
}
void Card::syncClock(){
	//syncronyse the estimator
	spcm_dwGetParam_i64(hDrv, SPC_DATA_AVAIL_USER_LEN, &availBytes);//is this slow ?
	long delta = availBytes - bufsizeInSamples;

	float ratio = availBytes / (float)bufsizeInSamples;
	ajustement = ajustementMax + (ratio-2)*ajustementSlope;

	localMax = 0;
	timerEstimator = startTimer();
	newEstimator = estimator = 0;
	//estimator += ajustement;
	//printf("counters : %d,%d, %f\n", c, k,c/(float)k);

	if (k % (controleRate * printRate) == 0) {
			printf("disponible : %d (%f)->%f,\ndeltas %d\n localMax %d,%d\n", availBytes / 1000, ratio, ajustement, delta, localMax,c);
		}

}

void Card::syncClock2(){
 	spcm_dwGetParam_i64(hDrv, SPC_DATA_AVAIL_USER_LEN, &availBytes);//is this slow ?
	//printf("syncClock2 1 %ld\n",(long)availBytes);
 
	avgAvailBytes = avg * availBytes + (1.0-avg) * avgAvailBytes;//average mesurements
 	float error = availBytes - bufsizeInSamples;//TODO divid by time ?
 	float DError = (error - pastError)/newEstimator;
 	IError += error * newEstimator * 1e-6 - 0.005*IError;
 	
 	ajustement = ajustementInit + P*error + I*IError + D*DError;
	ajustement = fmin(fmax(ajustement,ajustementInit-threshold),ajustementInit+threshold);
 	pastError = error;
	pidLog << error << " " << DError << " " << IError << " " << ajustement <<"\n";
	pidLog.flush();
 
 	timerEstimator = startTimer();
 	newEstimator = estimator = 0;

	k++;
 	float ratio = availBytes / (float)bufsizeInSamples;
	if (k > printRate) {
		printf("disponible : %lld (%f)->%f,\n",availBytes / 1000, ratio, ajustement);
		printf("errror %fM, IError %f, DError %f \n",error/1'000'000, IError, DError);
		printf("localMax %ld\n",  localMax);
		k=0;
	}
	localMax = 0;
	
}
void Card::updateEstimation(){

	newEstimator = getTimer(timerEstimator);
	long diff = (newEstimator - estimator) * (SAMPLE_RATE/1'000'000) * ajustement;
	
	//tick = oldTick + diff/2;
	absClock.set((long)oldAbsClock);//more reliable counter
	//printf("absClock 1 %ld\n",(long)absClock);
	absClock.inc(diff);
	//printf("absClock 2 %ld\n",(long)absClock);

	//printf("diff %ld\n",diff);
	if (diff > 8*1024) {
		estimator = newEstimator;
		localMax = localMax < diff ? diff : localMax;
		spcm_dwSetParam_i64(hDrv, SPC_DATA_AVAIL_CARD_LEN, diff*4);//is this slow ? 
		c++;
		//oldTick = tick;
		oldAbsClock.set((long)absClock);
		//printf("oldAbsClock %ld\n",(long)oldAbsClock); 

		//diffSum += diff;
		if ( (long)diffSum > SEGMENT_SIZE*4*2 ){
			//allow the record dispenser to resume writing in the buffer
			//printf("unlocking record dispenser\n");
			auto dump = recordDispenserBarrier.arrive();
			diffSum.reset();
		}
	}

	
	//printf("absClock 3 %ld\n",(long)absClock);
	//syncronisation with the card sometimes, with a PID
	if ((long)pidTimer > controleRate ) {
		pidTimer.reset();
		syncClock2();
	}
	

	if (spcm_dwGetErrorInfo_i32(hDrv, NULL, NULL, szErrorText) != ERR_OK) // check for an error
	{
		printf("llAvailBytes %lld\n", availBytes); 
		printf("push stat %ld,%ld, %f\n", c, k, c / (float)k);
		printf("localMax %ld\n", localMax);
		printf("%s\n",szErrorText); // print the error text	

		spcm_vClose(hDrv); // close the driver
				   
		exit(0); // and leave the program //TODO that might be one source of seg fault
	}

}

int Card::readInput(){

	int value = 0;
	spcm_dwGetParam_i32 (card->hDrv, SPCM_XX_ASYNCIO, &value);
	return value;
}

//this need a time limitation : it runs about every 5us and brick the card.
//if we add a "wait Xus", it might be an interesting tool
void Card::asyncReadInput(){
	while(1){
		inputReadout = readInput();
		++freq;
		if(freq%10000==0){
			printf("nb of mesurements : %ld * 1000 \n",freq/1000);
		}
	}
}

//looping to write the recording in the card buffer in advance. locking until new bytes are ready to be written
//TODO implement reset controle/end of sequence
void Card::recordDispenser(int16_t* record, size_t MaxLength){
	int i=0;
	//int k=0;
	while(1){
		size_t written4chSamples = 0;
		printf("recording length : %ld\n",MaxLength);
		while(written4chSamples<4*MaxLength){

			//wait for the card
			recordDispenserBarrier.arrive_and_wait();
			recordDispenserMutex.lock();
			//fill half of the buffer
			//TODO round to the next buffer cycle the value MaxLength*i (dephasing of placeInBuffer)
			size_t min = 4*(tick-MaxLength*i)+bufsizeInSamples/2;
			min = min<4*MaxLength?min:4*MaxLength;

			//printf("wait ended, writing : %ld samples in buffer\t tick : %ld, min : %ld, i : % d \n written samples was %ld\n", min-written4chSamples,tick,min,i,written4chSamples);
			for(;written4chSamples<min;written4chSamples+=4*SEGMENT_SIZE){
				size_t placeInBuffer = (written4chSamples)%bufsizeInSamples;
				
				//memcpy(&buffer[placeInBuffer],&record[written4chSamples],SEGMENT_SIZE* 4 * 2);
			}

			//printf("record dispenser done, waiting\t writtenSamples : %ld\n",written4chSamples);
			recordDispenserMutex.unlock();
			std::this_thread::yield();

			

		}
		i++;
		printf("end of recording\n");
	}

		//if we are in trigger mode, signal the end of the sequence to the card and wait for restart signal.
		//the card will stop the time, and ask the core to reset the sequence.
		//the card will wait for the trigger and signal the core when the sequence restart. the restart will engage the start sequence like it was the first time.


}
