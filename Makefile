hello:
	echo "hello world"
main:
	g++ -O main.cpp tweezer.cpp aom1D.cpp aom.cpp -o main.out
perftest:
	g++ -O0 perfTest.cpp core.cpp aom.cpp aom1D.cpp aom2D.cpp scheduler.cpp formGenerator.cpp tweezer.cpp -o perftest.out
