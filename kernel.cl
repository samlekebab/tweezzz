#define SAMPLE_RATE 600000000
__kernel
__attribute__((work_group_size_hint(256, 1, 1)))
void compute(float phase,__global short* output,__global float* aomTable, __global float* sinTable){
	size_t tick = get_global_id(0);
	//printf("phase %f\n",(float)phase);
	float res=0;
	__local float localAomTableA[100];
	__local float localAomTableN[100];
	__local float localAomTableW[100];
	__local float localAomTableP[100];

	for (int i=0;i<100;i++){
		localAomTableA[i] = aomTable[i];
	 }
	for (int i=0;i<100;i++){
		localAomTableN[i] = aomTable[i+100];//TODO replace with non hard coded, with tweezerCount 
	 }
	for (int i=0;i<100;i++){
		localAomTableW[i] = aomTable[i+200];
		//printf("w%d : %f\n",i,localAomTableW[i] );
	 }
	for (int i=0;i<100;i++){
		localAomTableP[i] = aomTable[i+300];
		//printf("p%d : %f\n",i,localAomTableP[i] );
	 }
	for (int i=0;i<100;i++){
		float A = localAomTableA[i];
		float N = localAomTableN[i];
		float w = localAomTableW[i];
		float p = localAomTableP[i];
		 res += A * N * native_sin(tick*(2*3.14f/SAMPLE_RATE)*w + p);
		//printf("%i: w%f N%f res%f\n",i,w,N,res);	
	}
	//printf("res on worker %d : %f\n",tick,res);
	output[tick] =(short)(3200 * res);//TODO remove hardcoded maxAmplitude
}

