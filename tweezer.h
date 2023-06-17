#ifndef TWEEZER
#define TWEEZER
class Tweezer{
	public :
		static const int nbOfParam = 5;
		float& A;//amplitude of the tweezer

		//we should delete this (redondant with A) parameter to reduce L3 bandwidth usage or precalculate the A*N ?
		float& N;//normalisation coefficient
		float& w;//frequency (in Hz, maybe this code should be refactored w->v for clarity), and in Mhz would improve calculation precision 
		float& p;//phase 
			 
		 //used for calculation, do not connect to it
		 //same idea as A*N, we could precalculate p + pr to reduce bandwidth of gpu (should we do it cpu side (will reduce writing latency to gpu memory or gpu side (will reduce cpu and accelerate calculation but might increase latency in general)
		float& pr;//cumulated phase (to reduce the arg in the sin to value closer to 1 (more precise)

		Tweezer(float* table,int tweezerCount):A(table[0]),N(table[1*tweezerCount]),w(table[2*tweezerCount]),p(table[3*tweezerCount]),pr(table[4*tweezerCount]){//super weird code :)

			A=1.0f/tweezerCount;
			N=1;
			w=70e6;
			p=0.0;
			pr=0;


		}	
};
#endif
