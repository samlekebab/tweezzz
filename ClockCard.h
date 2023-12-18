#pragma once

//this class keeps track of the clock on the card and the tick of the sequence
//it containes "timers" that can be reset, to count specific stuff
class ClockCard{
public:
	static long bytesToTicks(long bytes){
		return bytes/2/4;
	}
	struct Timer{
		long start=0;
		ClockCard& ref;
		virtual long get(){return ClockCard::bytesToTicks(ref-start);}
		operator long() {return get();};
		void reset(){start = (long)ref;}
		Timer(ClockCard& ref):ref(ref),start((long)ref){};
	};
	struct ByteTimer:public Timer{
		virtual long get(){return (ref-start);}
		ByteTimer(ClockCard& ref):Timer(ref){}
	};
	struct Tick{
		ClockCard& ref;
		operator long() {return ClockCard::bytesToTicks(ref);};
		Tick(ClockCard& ref):ref(ref){};
	};
	Tick tick;
	long bytes=0;//bytes sended to the card since the beginning
	void inc(long i){
		bytes+=i;
	}
	void set(long i){
		bytes = i;
	}
	operator long() {return bytes;};
	operator Tick() {return tick;};
	ClockCard():tick(*this){};

};
