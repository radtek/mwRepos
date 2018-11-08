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

ROBJS =
LOBJS = LockBase.o DerivedLock.o SafeLock.o Semaphore.o md5.o \
	UTF-8.o GetMem.o MemPool.o pthread_cond_t_ex.o process_stat_linux.o \
	KmpSearch.o cstring.o Encrypt.o DoubleListEx.o CompressStr.o \
	LongMsgSeq.o SocketPool.o
	#Encrypt.o
OBJS = $(ROBJS) $(LOBJS)
RSRCS =
LSRCS = LockBase.cpp DerivedLock.cpp SafeLock.cpp Semaphore.cpp md5.cpp \
	UTF-8.cpp GetMem.cpp MemPool.cpp pthread_cond_t_ex.cpp process_stat_linux.cpp\
	KmpSearch.cpp cstring.cpp Encrypt.cpp DoubleListEx.cpp CompressStr.cpp \
	LongMsgSeq.cpp SocketPool.cpp
	#Encrypt.cpp
SRCS =  $(RSRCS) $(LSRCS)

TARGET = $(LOBJS)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -c $(LSRCS)

clean:
	rm $(LOBJS)
	rm ./*.o
