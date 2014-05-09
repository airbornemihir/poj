all: ko_aluru ukkonen ukkonen2

%.o: %.cpp
	g++ -g -c -o $@ $<

%: %.o
	g++ -g -o $@ $<
