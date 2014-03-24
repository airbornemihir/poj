all: long_long_message

long_long_message: long_long_message.o
	g++ -g -o long_long_message long_long_message.o

long_long_message.o: long_long_message.cpp
	g++ -g -c -o long_long_message.o long_long_message.cpp
