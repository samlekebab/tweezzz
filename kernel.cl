#include "setting.h"
__kernel
__attribute__((work_group_size_hint(1024, 1, 1)))
void compute(float phase,__global short* output,__global float* GaomTable, __global float* sinTable){
	size_t g_tick = get_global_id(0);
	size_t tick = g_tick %BATCH_SIZE;
	__local short batch_number;
	batch_number = get_global_id(0)/BATCH_SIZE;

	//printf("phase %f\n",(float)phase);
	const short tweezerCount=100;//TODO replace with non hard coded, with tweezerCount 
	const short nbOfParam = 5;
	float res = 0;
	__local float localAomTableA[100];//bug if we put tweezerCount instead of 100
	__local float localAomTableN[100];
	__local float localAomTableW[100];
	__local float localAomTableP[100];
	__local float localAomTablePR[100];

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
	for (int i=0;i<100 ;i++){
		float A = localAomTableA[i];
		float N = localAomTableN[i];
		float w = localAomTableW[i];
		float p = localAomTableP[i];
		float pr = localAomTablePR[i];
		//un grosse erreur de 5 pour mille lorsque l'on fait tick*(2.0f*3.14159f) (etrange !!!!) resultant en un fort bruit de phase...
		 res += A * N * native_sin((tick * 6.283185f)*(w/(float)SAMPLE_RATE) + p + pr);
//		 if (i==0)
//			printf("%ld,%f: arg%f pr%f res%f \n",g_tick,(float)tick,(tick * 6.283185f)*(w/(float)SAMPLE_RATE),res);	
	}
//	printf("res on worker %d : %f\n",g_tick,res);
	output[/*2**/g_tick] =(short)(MAX_VALUE * res);//the 2 will be used to calculated two row of tweezers
	
}

