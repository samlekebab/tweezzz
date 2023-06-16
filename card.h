#ifndef CARD
#define CARD

#include "lib/regs.h"
#include "lib/spcerr.h"
#include "lib/dlltyp.h"
#include "lib/spcm_drv.h"

#include <cstdint>
#include <time.h>
#include "setting.h"
class Card{//TODO make this class closer to the equivalent class from the python code of pierre-Antoine Bourdel
	public:
		Card();
		~Card();
		//quite stable fifo
		double ajustementMax = 2.031; 
		double ajustementSlope = 0.003;
		double ajustement = 0;
		double securityThreshold = 1.8;
		int controleRate = 1000;
		int printRate = 1000;
		long bufsizeInSamples = BUFFER_SIZE * (long)SEGMENT_SIZE;
		int16_t* buffer;//where we are putting the data to send to the card

	//only a few second before loosing the signal
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

		void start();
		timespec timerEstimator;
		void updateEstimation();

		long tick = 0;
	private:

		char szErrorText[ERRORTEXTLEN];
		long long availBytes = 0;
		long estimator = 0;
		long newEstimator = 0;
		long localMax = 0;
		long oldTick = 0;
		

		long c = 0;
		long k = 0;


		int initCard();
		void initTransfert();


};
#endif
