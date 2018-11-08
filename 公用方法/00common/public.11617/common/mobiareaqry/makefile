CC 	= g++
CFLAGS  += -DLINUX 

VERSION=1.0.0.4

#version type(debug or release)
TARGET_TMP = ./out/libareaqry.so_
ifeq ($(VERTYPE),DEBUG)
CFLAGS  += -Wall -g -DDEBUG
TARGET = $(TARGET_TMP)d.$(VERSION)
endif
ifeq ($(VERTYPE),RELEASE)
CFLAGS  +=
TARGET = $(TARGET_TMP)r.$(VERSION)
endif

ifeq ($(GCCTYPE),OLD)
CC = g++343
endif
ifeq ($(GCCTYPE),NEW)
CC = g++
endif 

ROBJS = 

LOBJS = MobiAreaQry.o 

OBJS = $(ROBJS) $(LOBJS)
RSRCS = 
LSRCS = MobiAreaQry.cpp 
SRCS =  $(RSRCS) $(LSRCS)


$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -shared -s

.cpp.o:
	$(CC) $(CFLAGS) -c $< $(INCLUDE)

all:

clean:
	rm ./*.o
	rm $(TARGET)
	rm $(TARGET_TMP)*
	
cleanall:
	rm ./*.o
	rm $(TARGET)
	rm $(TARGET_TMP)*