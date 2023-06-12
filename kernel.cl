#include "setting.h"
__kernel
__attribute__((work_group_size_hint(256, 1, 1)))
void compute(float phase,__global short* output,__global float* GaomTable, __global float* sinTable){
	size_t g_tick = get_global_id(0);
	size_t tick = g_tick %BATCH_SIZE;
	__local short batch_number;
	batch_number = get_global_id(0)/BATCH_SIZE;

	//printf("phase %f\n",(float)phase);
	const short tweezerCount =100;//TODO replace with non hard coded, with tweezerCount 
	const short nbOfParam = 5;
	float res = 0;
	__local float localAomTableA[tweezerCount ];
	__local float localAomTableN[tweezerCount ];
	__local float localAomTableW[tweezerCount ];
	__local float localAomTableP[tweezerCount ];
	__local float localAomTablePR[tweezerCount ];

	barrier(CLK_LOCAL_MEM_FENCE | CLK_GLOBAL_MEM_FENCE);//I don't understand why the barrier impove the performance
	//printf("global aomtable index %d\n",batch_number*nbOfParam*tweezerCount);
	__global float* aomTable = &GaomTable[batch_number*nbOfParam*tweezerCount];//if there is no barrier, this line is reducing a lot the performances.
	for (int i=0;i<tweezerCount ;i++){
		localAomTableA[i] = aomTable[i];
	 }
	for (int i=0;i<tweezerCount ;i++){
		localAomTableN[i] = aomTable[i+tweezerCount ];
	 }
	for (int i=0;i<tweezerCount ;i++){
		localAomTableW[i] = aomTable[i+2*tweezerCount ];
		//printf("w%d : %f\n",i,localAomTableW[i] );
	 }
	for (int i=0;i<tweezerCount ;i++){
		localAomTableP[i] = aomTable[i+3*tweezerCount ];
		//printf("p%d : %f\n",i,localAomTableP[i] );
	}
	for (int i=0;i<tweezerCount ;i++){
		localAomTablePR[i] = aomTable[i+4*tweezerCount ];
	 }
	for (int i=0;i<tweezerCount ;i++){
		float A = localAomTableA[i];
		float N = localAomTableN[i];
		float w = localAomTableW[i];
		float p = localAomTableP[i];
		float pr = localAomTablePR[i];
		 res += A * N * native_sin(tick*(2*3.14f/SAMPLE_RATE)*w + p + pr);
		//printf("%i: w%f tick%d res%f\n",i,w,tick,res);	
	}
	//printf("res on worker %d : %f\n",tick,res);
	output[/*2**/g_tick] =(short)(MAX_VALUE * res);//the 2 will be used to calculated two row of tweezers
}

