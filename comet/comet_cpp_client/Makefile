# set CPUS for Linux or FreeBSD

PLATFORM := $(shell uname)
CPUS := $(strip $(if $(shell echo $(PLATFORM)|grep Linux),\
    $(shell cat /proc/cpuinfo|grep -c processor),\
    $(shell sysctl -a | egrep -i 'hw.ncpu' | cut -d: -f2)))

VERSION=

CXX := g++
CC := gcc

CXXFLAGS := -g3 -O2 -fno-strict-aliasing -Wall -Werror -fPIC -Wno-invalid-offsetof  -Wno-unused-function \
    -isystem /usr/include \
    -isystem /usr/local/include \
    -I/home/s/include \
	-I/usr/local/boost/include  \
    -I../..

LDFLAGS := -pthread \
    -L/home/s/lib \
    -L/usr/lib64/boost141 \
    -L/usr/lib64/ \
    -L/usr/lib/ \
	-L/usr/local/boost/lib \
    -L.

RTFLAGS := \
    -Wl,-rpath=/usr/local/boost/lib

# for snappy compress
# for gzip compress
LIBS :=/usr/local/boost/lib/libboost_system.a


SRC := $(wildcard *.cc)
OBJ := $(patsubst %.cc, %.o, $(SRC))
DEP := $(patsubst %.o, %.d, $(OBJ))

TARGET := comet_client 

ifeq ($(USE_DEP),1)
-include $(DEP) $(GEN_DEP)
endif

target: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $^  -o $@  $(LDFLAGS) $(LIBS)

%.o : %.cc
	$(CXX) -c $(CXXFLAGS) $< -o $@

%.d : %.cc
	@$(CXX) -MM $< $(CXXFLAGS) | sed 's/$(notdir $*)\.o/$(subst /,\/,$*).o $(subst /,\/,$*).d/g' > $@

clean:
	-rm -rf  $(OBJ) $(TARGET) *.pid *.log.* *.core $(DEP) $(GEN_DEP)

.PHONY: target clean

