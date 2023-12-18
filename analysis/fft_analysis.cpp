//#include <iostream>
#include "fftw3.h"
//#include <stdio.h>
#include "../setting.h"
#include <cmath>

#define PY_SSIZE_T_CLEAN
#include "python3.11/Python.h"

using namespace std;

#define STR(X) #X
#define STRR(X) STR(X)

#define RECORD_PATH "./tmpres/record.bin"
#define CHANNEL 0
#define OUT_PATH "./res/recordFFT" STRR(CHANNEL) ".txt"
#define WINDOW_SIZE 10'000
#define STEPS 500
fftw_complex* getChannel(int channel,FILE* file, size_t nbSamples){

	fftw_complex* chx;
	chx = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*nbSamples);
	int16_t tmp[4];
	for(size_t i;i<nbSamples;i++){
		fread(tmp,2,4,file);
		chx[i][0] = tmp[channel]/1000.0;
	//	chx[i][1]=0;
	//	printf("%d\n",tmp[channel]);
	}
	
	//reset cursor of the file
	fseek(file,0,SEEK_SET);

	return chx;


}
fftw_complex* fft(fftw_complex* in, size_t nbSamples){

	fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*nbSamples);
	auto p = fftw_plan_dft_1d(nbSamples, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	printf("execute\n");
	fftw_execute(p);
	fftw_destroy_plan(p);
	return out;
}
inline double amplitude(const fftw_complex c){
	//printf("c[0] %f",c[0]);
	return c[0]*c[0] + c[1]*c[1];
}
inline double phase(const fftw_complex c){
	return atan2(c[1],c[0]);
}
//TODO take argument for path, channel, output
int m_main(){

	printf("loading recorded file\n");
	auto file = fopen(RECORD_PATH,"rb");

	//get file size
	fseek(file,0,SEEK_END);
	size_t size = ftell(file);
	size_t nbSamples = size/2/4;
	fseek(file,0,SEEK_SET);
	printf("file of %f GBytes, meaning %ld samples\n",size/(float)1073741824,nbSamples);

	long windowSize = WINDOW_SIZE;
	//retreve a channel
	auto in = getChannel(CHANNEL,file,WINDOW_SIZE);
	
	
	//compute fft
	printf("compute fft\n");
	auto out = fft(in,WINDOW_SIZE);


	//save the result
	printf("writing file\n");
	auto outfile = fopen(OUT_PATH,"w");
	fprintf(outfile,"frequency\tamplitude\tphase\n");
	int facteur = 1;//nbSamples / 100 ;
	for (size_t i=0;i<WINDOW_SIZE;i+=facteur){
		fprintf(outfile,"%f\t%f\t%f\n",(double)i,amplitude(out[i]),phase(out[i]));
	}
	fclose(outfile);
	fclose(file);

	

	return 0;
}
extern "C" PyObject* python_test(PyObject *self, PyObject *args){
	printf("hello woooooooooooooooorld\n");
	m_main();
	return PyLong_FromLong(17);
}
static PyMethodDef FftWindowMethods[] = {
    {"test",  python_test, METH_VARARGS,
     "testing python module written in C"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};
static struct PyModuleDef fftWindowModule = {
    PyModuleDef_HEAD_INIT,
    "fftWindow",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    FftWindowMethods
};
PyMODINIT_FUNC
PyInit_fftWindow(void)
{
    return PyModule_Create(&fftWindowModule);
}
