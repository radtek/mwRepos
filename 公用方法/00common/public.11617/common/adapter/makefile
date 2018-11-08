CC 	= g++
CFLAGS  += -DLINUX -DSQL_WCHART_CONVERT -DLIB_EXPORTS

#version type(debug or release)
ifeq ($(VERTYPE),DEBUG)
CFLAGS  += -Wall -g -DDEBUG
endif
ifeq ($(VERTYPE),RELEASE)
CFLAGS  +=
endif

ifeq ($(GCCTYPE),OLD)
CC = g++343
endif
ifeq ($(GCCTYPE),NEW)
CC = g++
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
	rm $(LOBJS)
	rm ./*.o
	rm test

