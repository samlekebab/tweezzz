hello:
	echo "hello world"
main:
	g++ -O main.cpp tweezer.cpp aom1D.cpp aom.cpp -o main.out
perftest:
	g++ -fext-numeric-literals -std=c++20 -O2 -ftree-vectorize perfTest.cpp core.cpp coregpu.cpp aom.cpp aom1D.cpp aom2D.cpp scheduler.cpp formGenerator.cpp tweezer.cpp tas.cpp mmath.cpp -o perftest.out -pg -lOpenCL 
wavetest:
	g++ -fext-numeric-literals -std=c++20 -O2 -ftree-vectorize waitTimeEvent.cpp timeEvent.cpp waveTest.cpp core.cpp coregpu.cpp aom.cpp aom1D.cpp aom2D.cpp scheduler.cpp formGenerator.cpp tweezer.cpp tas.cpp mmath.cpp card.cpp -o wavetest.out -pg -lOpenCL -lspcm64 -Wall
withOptiInfo:
	g++ -std=c++20 -O2 -ftree-vectorize -fopt-info-vec-missed perfTest.cpp core.cpp coregpu.cpp aom.cpp aom1D.cpp aom2D.cpp scheduler.cpp formGenerator.cpp tweezer.cpp tas.cpp mmath.cpp -o perftest.out -pg
test:
	gcc -std=c++20 -O2 -ftree-vectorize -fopt-info-vec-all core.cpp
rectest:
	g++ -fext-numeric-literals -std=c++20 -O2 -ftree-vectorize waitTimeEvent.cpp timeEvent.cpp recordTest.cpp core.cpp coregpu.cpp aom.cpp aom1D.cpp aom2D.cpp scheduler.cpp formGenerator.cpp tweezer.cpp tas.cpp mmath.cpp card.cpp MasterLock.cpp -o rectest.out -pg -lOpenCL -lspcm64 -Wall 
analyse:
	g++ -fext-numeric-literals -std=c++20 -O2 -ftree-vectorize analysis/fft_analysis.cpp -lfftw3 -lm -o analyse.out 
rttest:
	g++ -fext-numeric-literals -std=c++20 -O2 -ftree-vectorize waitTimeEvent.cpp timeEvent.cpp rtTest.cpp core.cpp coregpu.cpp aom.cpp aom1D.cpp aom2D.cpp scheduler.cpp formGenerator.cpp tweezer.cpp tas.cpp mmath.cpp card.cpp MasterLock.cpp -o rtTest.out -pg -lOpenCL -lspcm64 -Wall 
