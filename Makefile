all : pub.x  sub.x example.x

pub.x : pub.o
	g++ -g -std=gnu++11 -I. -o pub.x pub.o -lhiredis

pub.o : pub.cpp
	g++ -g -std=gnu++11 -I. -I/usr/local/include/hiredis -c pub.cpp

sub.x : sub.o
	g++ -g -std=gnu++11 -I. -o sub.x sub.o -lhiredis

sub.o : sub.cpp
	g++ -g -std=gnu++11 -I. -I/usr/local/include/hiredis -c sub.cpp

example.x : example.o
	g++ -g -std=gnu++11 -I. -o example.x example.o -lhiredis

example.o : example.cpp
	g++ -g -std=gnu++11 -I. -I/usr/local/include/hiredis -c example.cpp

clean :
	rm -f sub.x sub.o pub.x pub.o example.x example.o 
