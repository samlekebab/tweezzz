#include "ClockCard.h"

#include <cstdio>

int main(){

	ClockCard mCC;
	printf("mCC : %ld\n",(long)mCC);
	auto timer1 = ClockCard::Timer(mCC);
	mCC.inc(10);
	auto timer2 = ClockCard::ByteTimer(mCC);
	auto timer3 = ClockCard::Timer(mCC);
	printf("mCC : %ld\n",(long)mCC);
	printf("mCC,tick : %ld\n",(long)mCC.tick);
	printf("timer1 : %ld\n",(long)timer1);
	printf("timer2 : %ld\n",(long)timer2);
	printf("timer3 : %ld\n",(long)timer3);
	mCC.inc(100);
	printf("mCC : %ld\n",(long)mCC);
	printf("mCC.tick : %ld\n",(long)mCC.tick);
	printf("timer1 : %ld\n",(long)timer1);
	printf("timer2 : %ld\n",(long)timer2);
	printf("timer3 : %ld\n",(long)timer3);
	printf("mCC tick : %ld\n",(long)(ClockCard::Tick)mCC);



}


