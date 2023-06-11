#ifndef TWEEZER
#define TWEEZER
class Tweezer{
	public :
		static const int nbOfParam = 4;
		float& A;//amplitude of the tweezer
		float& N;//normalisation coefficient
		float& w;//frequency (in Hz, maybe this code should be refactored w->v for clarity), and in Mhz would improve calculation precision 
		float& p;//phase 
		Tweezer(float* table,int tweezerCount):A(table[0]),N(table[1*tweezerCount]),w(table[2*tweezerCount]),p(table[3*tweezerCount]){//super weird code :)

			A=1.0f/tweezerCount;
			N=1;
			w=70e6;
			p=1;

		}	
};
#endif
