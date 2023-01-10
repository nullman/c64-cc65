.RECIPEPREFIX = >

CXX = cc65
CLX = cl65
CXXFLAGS = -t c64 -O

all: qixlines.prg

qixlines.prg:
> $(CLX) $(CXXFLAGS) -o qixlines.prg *.c

clean:
> rm -f *.prg *.inc *.o
