#ifndef FORM_UTILS
#define FORM_UTILS
#include "formGenerator.h"
#include "timeEvent.h"


class Ramp:public FormGenerator{
	

	public:
		struct Setting{
			long duration;
			float finalValue=1.0;
		};
		Setting setting;
		float initial;
		Ramp(Setting setting):setting(setting){
			setTag("rampup");
		}
		//returns the duration in sample of the FormGenerator
		long getDuration(){
			return setting.duration;
		}

		//take a sampletime with the beginning as origin and return a value between 0 and 1
		int ii = 0;
		float calc(long time){
			*target = initial + (float)time/setting.duration * (setting.finalValue - initial);
			if (ii%1'000 ==0)
				//printf("id %d, %f\n",id,*target);
			ii++;
			return *target;
		
		}

		//if we want to take the value at the beginning to adapte our calculation
		//it can be done that way
		void setBeginningValue(float value){initial = value;}
};

class MesurementTimeEvent:public TimeEvent{
	public: 
		struct Setting{
			AsyncInput& input;
		};
		Setting setting;
		MesurementTimeEvent(Setting setting):setting(setting){
			setTag("MesurementTimeEvent");
		}
		float calc(long tick){
			//TODO mesure async input X0,1,2
			//DEBUG (?)
			int input = Card::card->readInput();
			printf("input read : %d\n",input);
			//TODO put the result in setting.input.X0,1,2
			setting.input.X0 = input & 0b001; 
			setting.input.X1 = input & 0b010; 
			setting.input.X2 = input & 0b100; 
			return 0;
		}
};

class Marker:public FormGenerator{
	float calc(long tick){return 0;};
	long getDuration(){return 1;};
	void setBeginningValue(float value){};

};
#endif
