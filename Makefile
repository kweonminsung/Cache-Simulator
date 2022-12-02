all: main result

main: main.o
	g++ -o $@ $^
main.o: main.cpp
	g++ -c $^

result: result.o
	g++ -o $@ $^
result.o: result.cpp
	g++ -c $^

clear: rm -rf *.o main result