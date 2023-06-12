
#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include "core.h"
#include "scheduler.h"
#include "aom1D.h"
#include "aom2D.h"
#include "formGenerator.h"
#include <thread>
#include "lib/regs.h"
#include "lib/spcerr.h"
#include "lib/dlltyp.h"
#include "lib/spcm_drv.h"
#include <thread>
#include <profileapi.h>
#include "mmath.h"
using namespace std;
class Rampup:public FormGenerator{
	

	public:
		struct Setting{
			int test;
			double duration;
		};
		Setting setting;
		double initial;
		Rampup(Setting setting):setting(setting){}
	//return the sample duration of the effect
		long getDuration(){
			return setting.duration;
		}
	//take a sampletime from the begining of and return a value between 0 and 1
		double calc(long time){
			cout<<"target "<<*target<<endl;
			*target = (double)time*1.0/setting.duration;
			return *target;
		
		}
		//if we want to take the value at the beginning to adapte our calculation
		
		void setBeginningValue(double value){}






};
float ramp(float x){
	float value = x/10000;
	if (value<100){
		return 100;
	}
	return value;
}
auto startTimer(){
	long res = 0;
	LARGE_INTEGER* tmp = new LARGE_INTEGER;
	QueryPerformanceCounter(tmp);
	res =(long) tmp->QuadPart;
	delete tmp;
	return res;
	//return chrono::system_clock::now();
}
long getTimer(auto timer){
	return startTimer() - timer;
	//return chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now()-timer).count();
}
int test1(){
	auto now = startTimer();
	unsigned long i,j;
	float res(0.0f);
	srand(getTimer(now));
	cout<<(unsigned long)((long)-1)<<endl;
	long x = (getTimer(now) *1043 )% 3;
	srand(time(nullptr)+getTimer(now)+x);
	auto* rp = new Rampup({.test = (int)1.789234567e9});
	cout<<rp->calc(0)<<endl<<rp->getDuration()<<endl;
	/*
	while(getTimer(now)<30*1000000){
		i+=1;
		res += cos((float)(rand()));
		if (i==1000000){
			j+=1;
			i=0;
		}
	}
*/
	cout<<j<<" "<<res<<" "<<i<<endl;
	//cout<<rampup.calc(0)<<endl;
	return 0;
}

int test2(){
	coreCalc::startTimer();
	while(1){
		cout<<coreCalc::getCurrentCardSegment()<<endl;
	}
}
int test3(){
	Aom1D aom1D;
	Aom2D aom2D;
	Scheduler scheduler;
	coreCalc::startCore(scheduler,aom1D,aom2D);
	return 0;
}
void sequence(Aom1D& aom1D, Aom2D& aom2D){
	//cout<<"begining of the sequence"<<endl;
	aom1D.A = 0.25;
	(new Rampup({.test = 0, .duration = 100'000}))->connect(aom1D.A);
}
long tick = 0;
#include <cmath>
void vCalcOrLoadData(int16* buff, long long size,float amplitude) {
	int f = mmath::lut(70e6);
	for (long long i = 0; i < size; i++) {
		buff[i] = 3200 * mmath::sin[f+i% SIN_TAB_SIZE] * amplitude;
		tick++;
	}
	
}
void ondulation(int16* buff, long long size) {
	auto timer = startTimer();
	mmath::fillUpTable();
	while (1) {
		float amp = (1 + sin(getTimer(timer) / 10'000'000.0)) / 2;
		vCalcOrLoadData(buff, size,amp);
		printf("amplitude : %f\n", amp);
	}

}
#define SAMPLE_RATE 400'000'000
int main() {
		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
		char szErrorText[ERRORTEXTLEN];

		//quit stable fifo
		double ajustementMax = 2.031; 
		double ajustementSlope = 0.003;
		double ajustement = 0;
		double securityThreshold = 1.8;
		int controleRate = 1000;
		int printRate = 1000;
		int llBufsizeInSamples = 350 * 262144;

		//only a few secound before loosing the signal
		/*
		double ajustementMax = 2.031; 
		double ajustementSlope = 0.003;
		double ajustement = 0;
		double securityThreshold = 1.8;
		int controleRate = 1000;
		int printRate = 1000;
		int llBufsizeInSamples = 50 * 262144;
		*/

		//init card
		
		drv_handle hDrv; // the handle of the device
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
		spcm_dwSetParam_i64(hDrv, SPC_CHENABLE, CHANNEL0);

		//output enable
		spcm_dwSetParam_i32(hDrv, SPC_ENABLEOUT0, 1);
		//spcm_dwSetParam_i32(hDrv, SPC_ENABLEOUT1, 0);
		//spcm_dwSetParam_i32(hDrv, SPC_ENABLEOUT2, 0);
		//spcm_dwSetParam_i32(hDrv, SPC_ENABLEOUT3, 0);

		long lActivatedChannels;
		long lChCount;
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

		//inspired by example of the manual
		// in FIFO mode we need to define the buffer before starting the transfer
		int16* pnData = (int16*)malloc(llBufsizeInSamples * 2); // assuming 2 byte per sample
		spcm_dwDefTransfer_i64(hDrv, SPCM_BUF_DATA, SPCM_DIR_PCTOCARD, 8*1024,
			(void*)pnData, 0, 2 * llBufsizeInSamples);
		// before start we once have to fill some data in for the start of the output
		vCalcOrLoadData(pnData,llBufsizeInSamples, 1.0f);
		spcm_dwSetParam_i64(hDrv, SPC_DATA_AVAIL_CARD_LEN, llBufsizeInSamples);
		spcm_dwSetParam_i32(hDrv, SPC_M2CMD, M2CMD_DATA_STARTDMA);

		thread calculator(ondulation, pnData, llBufsizeInSamples);
		//spcm_dwSetParam_i32(hDrv, SPC_M2CMD, M2CMD_DATA_WAITDMA);
		// we replay data in a loop. As we defined a notify size of 4k we’ll get the data in >=4k chuncks
		int llTotalBytes = 2 * llBufsizeInSamples;
		long long llAvailBytes = 0;
		long long llUserPosInBytes = 0;
		// now the first <notifysize> bytes have been transferred to card and we start the output
		long estimator = 0;
		long newEstimator = 0;
		long localMax = 0;
		int tmp;
		QueryPerformanceFrequency((LARGE_INTEGER*)&tmp);
		float perfCountFreq = (float)tmp;
		printf("freq %f\n", perfCountFreq);

		spcm_dwSetParam_i32(hDrv, SPC_M2CMD, M2CMD_CARD_START | M2CMD_CARD_FORCETRIGGER);
		auto timerEstimator = startTimer(); 
		long c = 0;
		long k = 0;

	

		while (1)
		{
			
			auto t = startTimer();
			newEstimator = t - timerEstimator ;
			long diff = (newEstimator - estimator) *(((long)SAMPLE_RATE / perfCountFreq) * ajustement);

			k++;
			if (diff > 1024*128) {
				estimator = newEstimator;
				localMax = localMax < diff ? diff : localMax;
				spcm_dwSetParam_i64(hDrv, SPC_DATA_AVAIL_CARD_LEN, diff);//is this slow ? 
				c++;
			}

			if (k % controleRate == 0) {
				//syncronyse the estimator
				spcm_dwGetParam_i64(hDrv, SPC_DATA_AVAIL_USER_LEN, &llAvailBytes);//is this slow ?
				long delta = llAvailBytes - llBufsizeInSamples;

				float ratio = llAvailBytes / (float)llBufsizeInSamples;
				ajustement = ajustementMax + (ratio-2)*ajustementSlope;

				if (ratio > securityThreshold) {
					spcm_dwSetParam_i64(hDrv, SPC_DATA_AVAIL_CARD_LEN, max(0, (ratio - securityThreshold+0.01)* llBufsizeInSamples));
					//printf("underun security triggerd\n");
				}

				if (k % (controleRate * printRate) == 0) {
					printf("disponible : %d (%f)->%f,\ndeltas (%d)%d\n localMax %d,%d\n", llAvailBytes / 1000, ratio, ajustement, delta, diff, localMax,c);
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
				printf("llAvailBytes %d\n", llAvailBytes); 
				printf("push stat %d,%d, %f\n", c, k, c / (float)k);
				printf("localMax %d\n", localMax);
				printf(szErrorText); // print the error text	

				spcm_vClose(hDrv); // close the driver
				exit(0); // and leave the program
			}
			//std::this_thread::yield();
		}
		spcm_vClose(hDrv);

		return 0;
	
}
int test4() {
	//initialisation
	Scheduler scheduler;
	FormGenerator::scheduler = &scheduler;
	Aom1D aom1D; aom1D.A = 0.5;
	Aom2D aom2D;
	thread coreThread(coreCalc::startCore,ref(scheduler),ref(aom1D),ref(aom2D));
	
	double initFreq{67e6};
	for(int i=0; i<aom1D.tweezerCount; i++){
		aom1D.tweezers[i]->w = initFreq+i*5e5;
	}

	//run
	thread sequence_thread(sequence, std::ref(aom1D), std::ref(aom2D));
	
	//end
	coreThread.join();
	sequence_thread.join();

	cout<<"end of the program"<<endl;//this never happens
	return 0;
}
