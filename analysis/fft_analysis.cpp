#include <iostream>
#include "fftw3.h"
#include <stdio.h>
#include "../setting.h"
#include <cmath>
using namespace std;

#define RECORD_PATH "./tmpres/record.bin"
#define CHANNEL 0
#define OUT_PATH "./res/recordFFT$(CHANNEL).txt"
fftw_complex* getChannel(int channel,FILE* file, size_t nbSamples){

	fftw_complex* chx;
	chx = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*nbSamples);
	int16_t tmp[4];
	for(size_t i;i<nbSamples;i++){
		fread(tmp,2,4,file);
		chx[0][0] = tmp[channel];
		chx[0][1]=0;
	}
	
	//reset cursor of the file
	fseek(file,0,SEEK_SET);

	return chx;


}
fftw_complex* fft(fftw_complex* in, size_t nbSamples){

	fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*nbSamples);
	auto p = fftw_plan_dft_1d(nbSamples, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_execute(p);
	fftw_destroy_plan(p);
	return out;
}
inline double amplitude(const fftw_complex c){
	return c[0]*c[0] + c[1]*c[1];
}
inline double phase(const fftw_complex c){
	return atan2(c[1],c[0]);
}
//TODO take argument for path, channel, output
int main(){

	printf("loading recorded file\n");
	auto file = fopen(RECORD_PATH,"rb");

	//get file size
	fseek(file,0,SEEK_END);
	size_t size = ftell(file);
	size_t nbSamples = size/2/4;
	fseek(file,0,SEEK_SET);
	printf("file of %f GBytes, meaning %ld samples",size/(float)1073741824,nbSamples);

	//retreve a channel
	auto in = getChannel(CHANNEL,file,nbSamples);
	
	
	//compute fft
	printf("compute fft\n");
	auto out = fft(in,nbSamples);


	//save the result
	auto outfile = fopen(OUT_PATH,"w");
	fprintf(outfile,"frequency\tamplitude\tphase");
	for (size_t i=0;i<nbSamples;i++){
		fprintf(outfile,"%f\t%f\t%f",(double)i,amplitude(out[i]),phase(out[i]));
	}
	fclose(outfile);
	fclose(file);



	return 0;
}
