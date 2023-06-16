hello:
	echo "hello world"
main:
	g++ -O main.cpp tweezer.cpp aom1D.cpp aom.cpp -o main.out
perftest:
	g++ -fext-numeric-literals -std=c++20 -O2 -ftree-vectorize perfTest.cpp core.cpp coregpu.cpp aom.cpp aom1D.cpp aom2D.cpp scheduler.cpp formGenerator.cpp tweezer.cpp tas.cpp mmath.cpp card.cpp -o perftest.out -pg -lOpenCL -lspcm64
withOptiIfo:
	g++ -std=c++20 -O2 -ftree-vectorize -fopt-info-vec-missed perfTest.cpp core.cpp coregpu.cpp aom.cpp aom1D.cpp aom2D.cpp scheduler.cpp formGenerator.cpp tweezer.cpp tas.cpp mmath.cpp -o perftest.out -pg

test:
	gcc -std=c++20 -O2 -ftree-vectorize -fopt-info-vec-all core.cpp
