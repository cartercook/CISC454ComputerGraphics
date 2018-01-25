# This Makefile works only for Linux.

LIBS = -lglut -lGLU -lGL -lm
CXXFLAGS = -g -DLINUX
CXX = g++

PROG = missile
OBJS = main.o vector.o state.o

.C.o:
	$(CXX) $(CXXFLAGS) -c $<

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

all:	$(PROG)

$(PROG): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROG) $(OBJS) $(LIBS)

clean:
	rm -f *~ *.o $(PROG)
depend:
	makedepend -Y *.h *.cpp

# DO NOT DELETE

buildings.o: vector.h
circle.o: vector.h
city.o: buildings.h vector.h
missile.o: vector.h
silo.o: buildings.h vector.h
state.o: seq.h vector.h missile.h silo.h buildings.h city.h circle.h
main.o: headers.h state.h seq.h vector.h missile.h silo.h buildings.h city.h
main.o: circle.h
state.o: headers.h state.h seq.h vector.h missile.h silo.h buildings.h city.h
state.o: circle.h
vector.o: vector.h
