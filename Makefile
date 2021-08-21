main: main.cpp
	clang++ -g -O3 -lprofiler -stdlib=libc++ -std=c++17 main.cpp -o main

prof: main
	CPUPROFILE=prof.out ./main
	pprof --gif main prof.out > output.gif && xdg-open output.gif

clean:
	rm main

run: main
	./main
