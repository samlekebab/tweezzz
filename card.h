#ifndef CARD
#define CARD

#include "lib/regs.h"
#include "lib/spcerr.h"
#include "lib/dlltyp.h"
#include "lib/spcm_drv.h"

#include <cstdint>
#include <time.h>
#include "setting.h"

#include <thread>
#include <mutex>
#include <barrier>
class Card{//TODO make this class closer to the equivalent class from the python code of pierre-Antoine Bourdel
	public:
		Card();
		~Card();
		//quite stable fifo one channel
		double ajustementMax = 2.031; 
		double ajustementSlope = 0.003;
		double ajustement = 0;
		double securityThreshold = 1.8;
		int controleRate = 500;
		int printRate = 4000;
		long bufsizeInSamples = BUFFER_SIZE * (long)SEGMENT_SIZE * 4;
		int16_t* buffer;//where we are putting the data to send to the card

		//quite stable fifo one channel
//		double ajustementMax = 2.031; 
//		double ajustementSlope = 0.003;
//		double ajustement = 0;
//		double securityThreshold = 1.8;
//		int controleRate = 1000;
//		int printRate = 1000;
//		long bufsizeInSamples = BUFFER_SIZE * (long)SEGMENT_SIZE * 4;
//		int16_t* buffer;//where we are putting the data to send to the card

	//works only for a few second before loosing the signal
		/*
		double ajustementMax = 2.031; 
		double ajustementSlope = 0.003;
		double ajustement = 0;
		double securityThreshold = 1.8;
		int controleRate = 1000;
		int printRate = 1000;
		int llBufsizeInSamples = 50 * 262144;
		*/


		drv_handle hDrv; 


		static timespec startTimer();
		static long timespec_to_long(timespec time);
		static long getTimer(timespec timer);

		void initTransfert();
		void start();
		timespec timerEstimator;
		void updateEstimation();

		static Card* card;
		static int readInput();
		int inputReadout=0;
		long freq;
		std::thread asyncInputThread;
		void asyncReadInput();

		long diffSum=0;//TODO unit all this counters (diff, diffSum, newEstimation, tick...)
		unsigned long tick = 0;

		std::mutex recordDispenserMutex;//to wait on the initialisation that the dispenser fill 1/2 of the buffer
		
		void recordDispenser(int16_t* record, size_t MaxLength);
	private:

		char szErrorText[ERRORTEXTLEN];
		long long availBytes = 0;
		long estimator = 0;
		long newEstimator = 0;
		long localMax = 0;
		long oldTick = 0;
		
		//for 200MHz
		float avg = 0.5;
		float avgAvailBytes = bufsizeInSamples;
		//float P = 6e-9,I = 5e-8, D = 2e-5;
		float P = 19e-9,I = 5e-8, D = -25e-6;
 		float IError=0,pastError=0;
 		void syncClock2();

		long c = 0;
		long k = 0;


		int initCard();
		void syncClockAsync();
		void syncClock();


};
#endif
