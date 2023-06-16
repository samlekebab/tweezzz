#ifndef COREGPU
#define COREGPU
#define CL_TARGET_OPENCL_VERSION 120

#include <CL/cl.h>
#include "aom1D.h"
#include "aom2D.h"

#include "setting.h"
class CoreGPU{
	public:
		CoreGPU();

		int initCl(const char *programPath,const char *kernelName, cl_kernel& kernel, cl_command_queue& queue,int buffSize, cl_mem& buffer, int16_t* hostBuff);
		
		const static size_t workSize = SEGMENT_SIZE;
		cl_context context;
		cl_kernel kernel;
		cl_command_queue queue;
		cl_mem cl_outBuffer;
		cl_mem cl_aomBuffer;//TODO same for the aom2D
		int16_t outBuffer[2*workSize];

		cl_mem cl_sinBuffer;//represent the sin lut buffer in the video memory
		float sinBuffer[workSize];
		
		float phase = 0.5f16;

		int loadCoreKernel();
		int setParams(Aom1D& aom1D,Aom2D& aom2D, int i);
		int16_t* calculate(long tick);
};
#endif
