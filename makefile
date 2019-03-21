CXX=g++
RM=rm -f
CPPFLAGS= -g -O3 -Wl,--no-as-needed $(shell grsi-config --cflags)
LDFLAGS= -g $(shell root-config --ldflags) $(shell grsi-config --root)
LDLIBS= $(shell grsi-config --all-libs)

SRCS=MakeHistos.cxx FunctionsForMakeHistos.cxx SetupHistos_only.cxx
OBJS=$(subst .cc,.o,$(SRCS))

all: MakeHistos

MakeHistos: $(OBJS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o RunMe $(OBJS) $(LDLIBS) 

FunctionsForMakeHistos.o: FunctionsForMakeHistos.cc FunctionsForMakeHistos.hh

SetupHistos_only.o: SetupHistos_only.hh SetupHistos_only.cc

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) tool
