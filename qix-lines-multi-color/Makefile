.RECIPEPREFIX = >

CXX = cc65
CLX = cl65
CXXFLAGS = -t c64 -O

all: qixlinesmc

qixlinesmc:
> $(CLX) $(CXXFLAGS) -o qixlinesmc.prg *.c

clean:
> rm -f *.prg *.inc *.o
