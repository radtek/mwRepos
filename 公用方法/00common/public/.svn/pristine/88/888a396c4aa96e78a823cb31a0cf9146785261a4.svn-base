CC 	= g++
CFLAGS  += -DLINUX 
GCCTYPE=

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