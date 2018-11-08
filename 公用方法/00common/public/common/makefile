CC 	= g++
CFLAGS  += -DLINUX -DSQL_WCHART_CONVERT -DLIB_EXPORTS

ARCH = _x86_

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
ARCH = _x64_
endif

ROBJS =
LOBJS = LockBase.o DerivedLock.o SafeLock.o Semaphore.o md5.o md5_new.o \
	UTF-8.o GetMem.o MemPool.o pthread_cond_t_ex.o process_stat_linux.o process_stat_linux_ex.o\
	KmpSearch.o cstring.o Encrypt.o DoubleListEx.o CompressStr.o \
	LongMsgSeq.o SocketPool.o eatool.o dump.o
	#Encrypt.o
OBJS = $(ROBJS) $(LOBJS)
RSRCS =
LSRCS = LockBase.cpp DerivedLock.cpp SafeLock.cpp Semaphore.cpp md5.cpp md5_new.cpp \
	UTF-8.cpp GetMem.cpp MemPool.cpp pthread_cond_t_ex.cpp process_stat_linux.cpp process_stat_linux_ex.cpp\
	KmpSearch.cpp cstring.cpp Encrypt.cpp DoubleListEx.cpp CompressStr.cpp \
	LongMsgSeq.cpp SocketPool.cpp eatool.cpp dump.cpp
	#Encrypt.cpp
SRCS =  $(RSRCS) $(LSRCS)

TARGET = $(LOBJS)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -c $(LSRCS)

clean:
	rm -f $(LOBJS)
	rm -f ./*.o
