#include "coregpu.h"
#include <stdio.h>
#include <cstring>
#include <cmath>
#include <thread>
#include <chrono>
#include "setting.h"
CoreGPU::CoreGPU(){
	//DEBUG 
	//create dummmy sin TODO replace by the sin lut of mmath
	for(int i=0;i<workSize;i++){
		sinBuffer[i] = sin(i*0.001);
		//printf("%f\n",sinBuffer[i]);
	}
	loadCoreKernel();
}
int CoreGPU::initCl(const char *programPath,const char *kernelName, cl_kernel& kernel, cl_command_queue& queue,int buffSize, cl_mem& buffer, int16_t* hostBuff){
	cl_int cl_err = CL_SUCCESS;

	cl_uint numPlatforms = 0;
	cl_platform_id plt_id[3];
	cl_err = clGetPlatformIDs( 3, plt_id, &numPlatforms );

	if (cl_err == CL_SUCCESS)
		printf("%u platform(s) found\n", numPlatforms);
	else{
		printf("clGetPlatformIDs(%i)\n", cl_err);
		return -1;
	}
	cl_device_id device_id[3];
	cl_uint numDevices;
	int plt = OCL_PLAT;
	cl_err = clGetDeviceIDs(plt_id[plt],CL_DEVICE_TYPE_GPU,3,device_id,&numDevices);

	if (cl_err == CL_SUCCESS)
		printf("%u device(s) found\n", numDevices);
	else{
		printf("clGetDeviceIDs(%i)\n", cl_err);
		return -1;
	}

	context = clCreateContext(NULL,1,device_id,NULL,NULL,&cl_err);
	if (cl_err == CL_SUCCESS)
		printf("context created\n");
	else{
		printf("clCreateContext(%i)\n", cl_err);
		return -1;
	}
	
	FILE* f = fopen(programPath,"r");
	    fseek(f, 0L, SEEK_END);
	    int filesize = ftell(f) + 1;
	    rewind(f);
	    char *programString = (char*)malloc(filesize);
	    memset(programString, 0, filesize);
	    fread(programString, 1, filesize, f);
	    fclose(f);
	    //printf("%s", programString); 
	cl_program program = clCreateProgramWithSource(context,1,(const char**)&programString,NULL,&cl_err);

	if (cl_err == CL_SUCCESS)
		printf("program loaded\n");
	else{
		printf("clCreateProgramWithSource(%i)\n", cl_err);
		return -1;
	}

	cl_err = clBuildProgram(program,1,device_id, "",NULL,NULL);
	if (cl_err == CL_SUCCESS)
		printf("program builded\n");
	else{
		printf("clBuildProgram(%i)\n", cl_err);
		char tmp[1000];
		clGetProgramBuildInfo(program,device_id[0],
				CL_PROGRAM_BUILD_LOG,1000,tmp,NULL);
		printf(tmp);

		return -1;
	}

	
	kernel = clCreateKernel(program,kernelName,&cl_err);
	
	if (cl_err == CL_SUCCESS)
		printf("kernel loaded\n");
	else{
		printf("clCreateKernel(%i)\n", cl_err);
		return -1;
	}
	//get the max local worker size
	char retValue[1000]{'\0'};
	printf("ret : %i\n",clGetDeviceInfo(device_id[0],CL_DEVICE_MAX_WORK_GROUP_SIZE,1000,&retValue,NULL));
	printf("max work group size %d\n",(size_t)*retValue);

	queue = clCreateCommandQueue(context,device_id[0],0,&cl_err);	

	if (cl_err == CL_SUCCESS)
		printf("queue created\n");
	else{
		printf("clCreateCommandQueue(%i)\n", cl_err);
		return -1;
	}

	buffer = clCreateBuffer(context,
			CL_MEM_WRITE_ONLY
			|CL_MEM_HOST_READ_ONLY
			|CL_MEM_USE_HOST_PTR ,
			buffSize,hostBuff,&cl_err);

	if (cl_err == CL_SUCCESS)
		printf("buffer created\n");
	else{
		printf("clCreateBuffer(%i)\n", cl_err);
		return -1;
	}	

	//TODO replace hard coded value to tweezerCount and nbOfParam
	cl_aomBuffer = clCreateBuffer(context,
			CL_MEM_READ_ONLY
			|CL_MEM_HOST_WRITE_ONLY
			,
			100*5*sizeof(float)*(SEGMENT_SIZE/BATCH_SIZE),NULL,&cl_err);

	if (cl_err == CL_SUCCESS)
		printf("aom buffer created\n");
	else{
		printf("clCreateBuffer(%i)\n", cl_err);
		return -1;
	}

	cl_sinBuffer = clCreateBuffer(context,
			CL_MEM_READ_ONLY
			|CL_MEM_HOST_NO_ACCESS
			|CL_MEM_COPY_HOST_PTR,
			CoreGPU::workSize*sizeof(float),sinBuffer,&cl_err);

	if (cl_err == CL_SUCCESS)
		printf("sin buffer created\n");
	else{
		printf("clCreateBuffer(%i)\n", cl_err);
		return -1;
	}
	return 0;

}	
int CoreGPU::loadCoreKernel(){
	printf("load core kernel\n");
	const size_t workSize[]{(size_t)this->workSize};
	int err = initCl("./kernel.cl","compute",kernel,queue, workSize[0]*sizeof(float),cl_outBuffer,outBuffer);
	printf("init cl return %d\n",err);
	if (err!=0)
		return -1;
	
	float phase(0.5f16);
	cl_int cl_err = CL_SUCCESS;
	cl_err = clSetKernelArg(kernel,0,sizeof(float),&phase);	
	cl_err = clSetKernelArg(kernel,1,sizeof(int16_t*),&cl_outBuffer);
	cl_err = clSetKernelArg(kernel,2,sizeof(float*),&cl_aomBuffer);
	cl_err = clSetKernelArg(kernel,3,sizeof(float*),&cl_sinBuffer);
	
	if (cl_err == CL_SUCCESS)
		printf("set kernel arg\n");
	else{
		printf("clSetKernelArg1(%i)\n", cl_err);
		return -1;
	}
	return 0;
}
int16_t* CoreGPU::calculate(long tick, int16_t* buffer){

		//printf("YYYYYYYYYYYYYYYY\n");
		size_t workOffset[]{0};

		phase+=workSize*0.1f16;
		clSetKernelArg(kernel,0,sizeof(float),&phase);	

		auto tmp = workSize;//because worksize is static const, no dereferencing possible.
		clEnqueueNDRangeKernel(queue,kernel,1,workOffset,&tmp,NULL,0,NULL,NULL);
		cl_int cl_err = clEnqueueReadBuffer(queue,cl_outBuffer,CL_TRUE,0,workSize*sizeof(int16_t),outBuffer,0,NULL,NULL);
		//printf("XXXXXXXXXXXXXXXX\n");
		//printf("read(%i)\n", cl_err);
		//printf("a : gpu outbuffer 0 %d\n",outBuffer[938]);
//			for (size_t i=0;i<960;i++){
//				printf("res %d : %d\n",i,outBuffer[i]);
//			}
		return outBuffer;

}

int CoreGPU::setParams(Aom& aom, int j){
	//this 4 is the number of params of a tweezer, not great to hard code it like that : 
	//if it changes some day we will probably forget to change this value... 
	//hahaha it happened, now i correct it but i left this comment because it makes me laugh
	int length = aom.tweezerCount*Tweezer::nbOfParam*sizeof(float);
	clEnqueueWriteBuffer(queue,cl_aomBuffer,CL_TRUE,j*length,
			length,
			aom.table,
		0,NULL,NULL);//wtf putting cl_false(asynchrone load) kills the performances !?
	

	return 0;
}
