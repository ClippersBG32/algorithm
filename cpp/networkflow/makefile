#2018-10-21

objects = main.o

main: $(objects)
	g++ -o main main.o


main.o: main.cpp ford_fulkerson.h push_relabel.h edge.h
	g++ -c main.cpp


clean:
	rm main $(objects)
