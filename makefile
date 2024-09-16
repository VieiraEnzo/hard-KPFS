WARNINGS := -Wall -Wextra -Wshadow -Wconversion
SANITIZERS := -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -fsanitize=address
OPTIMIZATIONS := -O3 -mavx2
DEFINES := -Dlocal

run: run.o main
	g++ run.o -o run

run.o: run.cpp
	g++ -c run.cpp -o run.o

main: main.o constructive.o
	g++ main.o constructive.o -o main

main.o: main.cpp
	g++ -c ${OPTIMIZATIONS} main.cpp -o main.o

constructive.o: constructive.cpp
	g++ -c ${OPTIMIZATIONS} constructive.cpp -o constructive.o

clean:
	rm *.o main run
	find results -type f -delete