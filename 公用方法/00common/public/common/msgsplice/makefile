CC 	= g++
#CFLAGS  += -DLINUX  -DSQL_WCHART_CONVERT -DDEBUG  
CFLAGS  += -DLINUX -DSQL_WCHART_CONVERT

#version type(debug or release)
ifeq ($(VERTYPE),DEBUG)
CFLAGS += -Wall -g -DDEBUG
endif
ifeq ($(VERTYPE),RELEASE)
CFLAGS +=
endif

#
ifeq ($(OLEVEL),0)
CFLAGS  += 
endif
ifeq ($(OLEVEL),1)
CFLAGS  += -O1
endif
ifeq ($(OLEVEL),2)
CFLAGS  += -O2
endif
ifeq ($(OLEVEL),3)
CFLAGS  += -O3
endif

ifeq ($(GCCTYPE),OLD)
CC = g++343
endif
ifeq ($(GCCTYPE),NEW)
CC = g++
endif
ifeq ($(GCCTYPE),G64M32)
CC = g++
CFLAGS  += -D_686_M32_
endif
ifeq ($(GCCTYPE),X86_64)
CC = g++
CFLAGS  += -D_X64_
endif

#submake and params
SUBMAKE=make VERTYPE=$(VERTYPE)
SUBMAKECLEAN=make -i clean

ROBJS = ../public/common/adapter/Adapter.o \
		../public/common/adapter/CompatibleEvent.o \
		../public/common/adapter/CompatibleThread.o \
		../public/common/DerivedLock.o \
		../public/common/LockBase.o \
		../public/common/SafeLock.o 
LOBJS = MsgSpliceMgr.o MsgSplice.o 
OBJS = $(ROBJS) $(LOBJS)

RSRCS = 
LSRCS = MsgSpliceMgr.cpp MsgSplice.cpp
SRCS =  $(RSRCS) $(LSRCS)


TARGET = ../out/MsgSplice.so

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lpthread -shared -lcurses -s

.cpp.o:
	$(CC) $(CFLAGS) -c $< $(INCLUDE)
	
all:
	$(SUBMAKE) common
	$(SUBMAKE) adapter

adapter:
	cd ../public/common/adapter && $(SUBMAKECLEAN)
	cd ../public/common/adapter && $(SUBMAKE)

common:
	cd ../public/common && $(SUBMAKECLEAN)
	cd ../public/common && $(SUBMAKE)

clean:
	rm ./*.o
	rm $(TARGET)

cleanall:	
	rm $(OBJS)
	rm $(TARGET)
