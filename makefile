# $(info  *****************)
CPPFLAGS= -g -O3 -Wl,--no-as-needed -Wall -Wno-unused-function
# $(info CPPFLAGS is ${CPPFLAGS})
CPPFLAGS+= $(shell root-config --cflags)
# $(info Adding ROOT: $(shell root-config --cflags) to CPPFLAGS)
CPPFLAGS+= $(shell grsi-config --cflags) 
# $(info Adding GRSI: $(shell grsi-config --cflags) to CPPFLAGS)
# $(info FULL CPPFLAGS is ${CPPFLAGS})

# $(info  *****************)
LDFLAGS= -g 
# $(info LDFLAGS is ${LDFLAGS})
LDFLAGS+=$(shell root-config --ldflags) 
# $(info Adding ROOT: $(shell root-config --ldflags) to LDFLAGS)
LDFLAGS+=$(shell grsi-config --root)
# $(info Adding GRSI: $(shell grsi-config --root) to LDFLAGS)

# $(info  *****************)
LDLIBS= -lm
# $(info LDLIBS is ${LDLIBS})
LDLIBS+=$(shell root-config --glibs) 
# $(info Adding ROOT: $(shell root-config --glibs) to LDLIBS)
LDLIBS+=$(shell grsi-config --all-libs)  
# $(info Adding GRSI: $(shell grsi-config --all-libs) to LDLIBS)
# $(info  *****************)

EXE = RunMe

SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.cxx)
OBJ = $(SRC:$(SRC_DIR)/%.cxx=$(OBJ_DIR)/%.o)

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS) -lSpectrum

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cxx
	$(CXX) $(CPPFLAGS) -c $< -o $@ 

clean:
	$(RM) $(OBJ)