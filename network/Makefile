PROGRAM=libutils_network.a
cc=g++

PUB_DIR=../../../../../../../Library
PRJ_PUB_DIR=../../../../../../common
INCLUDES= $(PUB_DIR)/funshion/include/netsvc/0.1.4 -I$(PRJ_PUB_DIR)/include/json -I$(PRJ_PUB_DIR)/include \
	  -I$(PUB_DIR)/funshion/include/http/0.1.2  -I$(PUB_DIR)/funshion/include/kernel/0.1.2 \
	  -I$(PUB_DIR)/funshion/include/zlib/1.2.4 -I$(PUB_DIR)/funshion/include/logger/0.2.0
LIB_PATH= $(PUB_DIR)/funshion/lib/netsvc/0.1.4 -L$(PRJ_PUB_DIR)/lib -L$(PRJ_PUB_DIR)/lib/json \
          -L$(PUB_DIR)/funshion/lib/http/0.1.2 -L$(PUB_DIR)/funshion/lib/kernel/0.1.2 \
	  -L$(PUB_DIR)/funshion/lib/zlib/1.2.4 -L$(PUB_DIR)/funshion/lib/logger/0.2.0
SRC_DIRS= .

SRC=$(wildcard $(SRC_DIRS)/*.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRC))
LIBS=-lnetsvc -lpthread -ljson -lrt -lhttp -lkernel -lz -llogger

CXXFLAGS=${flags} -Wall -static

all:$(OBJS)
	ar -r $(PROGRAM) $(OBJS)

$(OBJS):%.o:%.cpp
	$(cc) -c $(CXXFLAGS) -I$(INCLUDES) $< -o $@

clean:
	rm -rf ${PROGRAM} *.o

print:
	ls $(PUB_DIR)
	ls $(PRJ_PUB_DIR)
	ls $(INCLUDES)
	ls $(LIB_PATH)
	ls $(PRJ_PUB_DIR)

