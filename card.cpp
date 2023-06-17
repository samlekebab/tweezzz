#include "card.h"
#include <iostream>
#include <cmath>
Card::Card(){
	initCard();
	initTransfert();
}
Card::~Card(){
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

	//channel selection //TODO multichannel mode
	spcm_dwSetParam_i64(hDrv, SPC_CHENABLE, CHANNEL0);

	//output enable
	spcm_dwSetParam_i32(hDrv, SPC_ENABLEOUT0, 1);
	//spcm_dwSetParam_i32(hDrv, SPC_ENABLEOUT1, 0);
	//spcm_dwSetParam_i32(hDrv, SPC_ENABLEOUT2, 0);
	//spcm_dwSetParam_i32(hDrv, SPC_ENABLEOUT3, 0);

	int lActivatedChannels;
	int lChCount;
	spcm_dwGetParam_i32(hDrv, SPC_CHENABLE, &lActivatedChannels);
	spcm_dwGetParam_i32(hDrv, SPC_CHCOUNT, &lChCount);
	printf("Activated channels bitmask is: 0x%08x\n", lActivatedChannels);
	printf("Number of activated channels with this bitmask: %d\n", lChCount);

	//max amplitude
	spcm_dwSetParam_i32(hDrv, SPC_AMP0, 2000);
	if (spcm_dwGetErrorInfo_i32(hDrv, NULL, NULL, szErrorText) != ERR_OK) // check for an error
	{
		printf(szErrorText); // print the error text
		spcm_vClose(hDrv); // close the driver
		exit(0); // and leave the program
	}
	//freq filters
	//spcm_dwSetParam_i32(hDrv, SPC_FILTER0, 0);


	//enable FIFO
	spcm_dwSetParam_i32(hDrv, SPC_CARDMODE, SPC_REP_FIFO_SINGLE);
	//spcm_dwSetParam_i32(hDrv, SPC_SEGMENTSIZE, 16*10);//segment size

	spcm_dwSetParam_i64(hDrv, SPC_LOOPS, 0);//no loops

	spcm_dwSetParam_i64(hDrv, SPC_DATA_OUTBUFSIZE, 2 * 256 * 1024);
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
			buffer[i]=-30000;//initialize buffer//DEBUG non zero to see the diff
		}

		spcm_dwDefTransfer_i64(hDrv, SPCM_BUF_DATA, SPCM_DIR_PCTOCARD, 8*1024,
			(void*)buffer, 0, 2 * bufsizeInSamples);

		//load the buffer of the card with blank data
		spcm_dwSetParam_i64(hDrv, SPC_DATA_AVAIL_CARD_LEN, bufsizeInSamples);
		spcm_dwSetParam_i32(hDrv, SPC_M2CMD, M2CMD_DATA_STARTDMA);
		//spcm_dwSetParam_i32(hDrv, SPC_M2CMD, M2CMD_DATA_WAITDMA);
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

void Card::start(){
	spcm_dwSetParam_i32(hDrv, SPC_M2CMD, M2CMD_CARD_START | M2CMD_CARD_FORCETRIGGER);
	timerEstimator = startTimer(); 
}

//TODO try put the card adjustement part on thread to see if it changes something
void Card::updateEstimation(){

	newEstimator = getTimer(timerEstimator);
	long diff = (newEstimator - estimator) *(SAMPLE_RATE/1'000'000) * ajustement;

	tick = oldTick + diff/2;
	k++;
	if (diff > 1024) {
		estimator = newEstimator;
		localMax = localMax < diff ? diff : localMax;
		spcm_dwSetParam_i64(hDrv, SPC_DATA_AVAIL_CARD_LEN, diff);//is this slow ? 
		c++;
		oldTick = tick;
	}

	if (k % controleRate == 0) {
		//syncronyse the estimator
		spcm_dwGetParam_i64(hDrv, SPC_DATA_AVAIL_USER_LEN, &availBytes);//is this slow ?
		long delta = availBytes - bufsizeInSamples;

		float ratio = availBytes / (float)bufsizeInSamples;
		ajustement = ajustementMax + (ratio-2)*ajustementSlope;

		if (ratio > securityThreshold) {
			spcm_dwSetParam_i64(hDrv, SPC_DATA_AVAIL_CARD_LEN, std::max(0.0, (ratio - securityThreshold+0.01)* bufsizeInSamples));
			//printf("underun security triggerd\n");
		}

		if (k % (controleRate * printRate) == 0) {
			printf("disponible : %d (%f)->%f,\ndeltas (%d)%d\n localMax %d,%d\n", availBytes / 1000, ratio, ajustement, delta, diff, localMax,c);
		}
		localMax = 0;
		timerEstimator = startTimer();
		newEstimator = estimator = 0;
		//estimator += ajustement;
		//printf("counters : %d,%d, %f\n", c, k,c/(float)k);

	}
	
	

	//spcm_dwSetParam_i32(hDrv, SPC_M2CMD, M2CMD_DATA_WAITDMA);
	if (spcm_dwGetErrorInfo_i32(hDrv, NULL, NULL, szErrorText) != ERR_OK) // check for an error
	{
		printf("llAvailBytes %d\n", availBytes); 
		printf("push stat %d,%d, %f\n", c, k, c / (float)k);
		printf("localMax %d\n", localMax);
		printf(szErrorText); // print the error text	

		spcm_vClose(hDrv); // close the driver
		exit(0); // and leave the program
	}
}
