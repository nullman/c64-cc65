.RECIPEPREFIX = >

CXX = cc65
CLX = cl65
CXXFLAGS = -t c64 -O

all: bitarray

bitarray:
> $(CLX) $(CXXFLAGS) -o bitarray.prg *.c

clean:
> rm -f *.prg *.inc *.o
