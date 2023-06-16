#include "setting.h"
__kernel
__attribute__((work_group_size_hint(1024, 1, 1)))
void compute(float phase,__global short* output,__global float* GaomTable, __global float* sinTable){
	size_t g_tick = get_global_id(0);
	size_t tick = g_tick %BATCH_SIZE;
	__local short batch_number;
	batch_number = get_global_id(0)/BATCH_SIZE;

	//printf("phase %f\n",(float)phase);
	const short tweezerCount=200;//TODO replace with non hard coded, with tweezerCount 
	const short nbOfParam = 5;
	float res = 0;
	__local float localAomTableA[200];//bug if we put tweezerCount instead of 100
	__local float localAomTableN[200];
	__local float localAomTableW[200];
	__local float localAomTableP[200];
	__local float localAomTablePR[200];

	barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);//I don't understand why the barrier impove the performance
	__global float* aomTable = &GaomTable[batch_number*nbOfParam*tweezerCount];//if there is no barrier, this line is reducing a lot the performances.

	for (int i=0;i<tweezerCount ;i++){
		localAomTableA[i] = aomTable[i];
	 }
	for (int i=0;i<tweezerCount ;i++){
		localAomTableN[i] = aomTable[i+tweezerCount ];
	 }
	for (int i=0;i<tweezerCount ;i++){
		localAomTableW[i] = aomTable[i+2*tweezerCount ];
	 }
	for (int i=0;i<tweezerCount ;i++){
		localAomTableP[i] = aomTable[i+3*tweezerCount ];
		//printf("p%d : %f\n",i,localAomTableP[i] );
	}
	for (int i=0;i<tweezerCount ;i++){
		localAomTablePR[i] = aomTable[i+4*tweezerCount ];
	 }
	for (int i=0;i<200 ;i++){
		float A = localAomTableA[i];
		float N = localAomTableN[i];
		float w = localAomTableW[i];
		float p = localAomTableP[i];
		float pr = localAomTablePR[i];
		 res += A * N * native_sin(tick*(2*3.14f/SAMPLE_RATE)*w + p + pr);
//		 if (i==0)
//			printf("%i: w%f sin%f res%f\n",i,w,native_sin(tick),res);	
	}
	//printf("res on worker %d : %f\n",g_tick,res);
	output[/*2**/g_tick] =(short)(MAX_VALUE * res);//the 2 will be used to calculated two row of tweezers
	
}

