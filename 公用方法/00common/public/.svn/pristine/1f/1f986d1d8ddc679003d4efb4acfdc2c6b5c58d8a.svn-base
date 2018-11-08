CC 	= g++
CFLAGS  += -DLINUX -DSQL_WCHART_CONVERT -DLIB_EXPORTS

CFLAGS += $(LCFLAGS) 

#version type(debug or release)
ifeq ($(VERTYPE),DEBUG)
CFLAGS  += -Wall -g -DDEBUG
endif
ifeq ($(VERTYPE),RELEASE)
CFLAGS  +=
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
CFLAGS  += -D_X64_ -fPIC
endif

ROBJS = ../../common/DerivedLock.o \
	../../common/LockBase.o \
	../../common/SafeLock.o
LOBJS = Adapter.o CompatibleEvent.o CompatibleThread.o \

OBJS = $(ROBJS) $(LOBJS)
RSRCS =
LSRCS = Adapter.cpp CompatibleEvent.cpp CompatibleThread.cpp
SRCS =  $(RSRCS) $(LSRCS)

TARGET = $(LOBJS)

$(TARGET):$(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS) -lpthread -rdynamic -ldl

.cpp.o:
	$(CC) $(CFLAGS) -c $< $(INCLUDE)

clean:
	rm -f $(LOBJS)
	rm -f  ./*.o
	rm -f test

