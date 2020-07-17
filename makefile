CXX=g++
CPPFLAGS=-std=c++11 -Wall -g
LDFLAGS=-g
LDLIBS=-lsfml-graphics -lsfml-window -lsfml-system
SRCS=$(shell find . -name "*.cpp")
OBJS=$(patsubst %.cpp, %.o, $(SRCS))

random-walk-app : $(OBJS)
	$(CXX) $(LDFLAGS) -o random-walk-app $(OBJS) $(LDLIBS)

%.o : %.cpp
	g++ $(CPPFLAGS) -MMD -c -o $@ $<


clean:
	rm -f $(OBJS)


depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^ > ./.depend;

include .depend


