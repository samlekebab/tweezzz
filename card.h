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
#include "ClockCard.h"

#include <fstream>

class Card{//TODO make this class closer to the equivalent class from the python code of pierre-Antoine Bourdel
	public:
		Card();
		~Card();
		//quite stable fifo one channel
		double ajustementMax = 2.031; 
		double ajustementSlope = 0.003;
		double ajustement = 2.12;
		double ajustementInit = ajustement;
		double threshold = 1.5;
		double securityThreshold = 1.8;
		int controleRate = 200'000;
		int printRate = 10'000;
		long bufsizeInSamples = BUFFER_SIZE * (long)SEGMENT_SIZE * 4;
		int16_t* buffer;//where we are putting the data to send to the card

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

		ClockCard absClock;
		ClockCard oldAbsClock;
		ClockCard::ByteTimer diffSum;
		ClockCard::Tick& tick = absClock.tick;
		ClockCard::ByteTimer pidTimer;

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
		float P = 6e-8,I = 1e-6, D = 2e-14;
 		float IError=0,pastError=0;
 		void syncClock2();

		long c = 0;
		long k = 0;


		int initCard();
		void syncClockAsync();
		void syncClock();

		std::ofstream pidLog{"res/pidLog.txt"};


};
#endif
