8puzzle: examples/8puzzle.cpp
	clang++ -g -O3 -lprofiler -stdlib=libc++ -std=c++17 examples/8puzzle.cpp -o 8puzzle

knight: examples/knight.cpp
	clang++ -g -O3 -lprofiler -stdlib=libc++ -std=c++17 examples/knight.cpp -o knight

grid: examples/grid.cpp
	clang++ -g -O3 -lprofiler -stdlib=libc++ -std=c++17 examples/grid.cpp -o grid


prof: grid
	CPUPROFILE=prof.out ./grid
	pprof --gif grid prof.out > output.gif && xdg-open output.gif

clean:
	rm -f 8puzzle knight grid prof.out output.gif

run: 8puzzle
	./8puzzle
