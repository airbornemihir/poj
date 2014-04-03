all: ko_aluru ukkonen

%.o: %.cpp
	g++ -g -c -o $@ $<

%: %.o
	g++ -g -o $@ $<
