.RECIPEPREFIX = >

CXX = cc65
CLX = cl65
CXXFLAGS = -t c64 -O

all: helloworld

helloworld:
> $(CLX) $(CXXFLAGS) -o helloworld.prg *.c

clean:
> rm -f *.prg *.inc *.o
