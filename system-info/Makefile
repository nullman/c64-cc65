.RECIPEPREFIX = >

CXX = cc65
CLX = cl65
CXXFLAGS = -t c64 -O

all: systeminfo.prg

systeminfo.prg:
> $(CLX) $(CXXFLAGS) -o systeminfo.prg *.c

clean:
> rm -f *.prg *.inc *.o
