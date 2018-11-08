CC 	= g++
CFLAGS  += -DLINUX -DLIB_EXPORTS

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

ifneq ($(NOTUSEODBC64BITMODE),1)
CFLAGS  += -DBUILD_LEGACY_64_BIT_MODE
endif

CFLAGS  += -D_X64_
endif


#INCLUDE = -I. -I$(ORACLE_HOME)/precomp/public \
	#-I$(ORACLE_HOME)/rdbms/demo \
	#-I$(ORACLE_HOME)/rdbms/public
LIB = -L$(ORACLE_HOME)/lib -locci -lclntsh

INCLUDE += -I/usr/include
#-I/usr/include
#-I/opt/ibm/db2/V9.7/include
#LIB += -L/opt/ibm/db2/V9.7/lib32 -ldb2
#LIB = -L/usr/local/freetds/lib -ltdsodbc
LIB += -L/usr/lib/oracle/11.2/client/lib -lsqora

ROBJS = #../../logmgr/LogMgr.o ../../common/LockBase.o ../../common/DerivedLock.o ../../common/SafeLock.o ../../common/pthread_cond_t_ex.o
LOBJS = SQLValue.o SQLField.o SQLResult.o SQLBase.o SQLException.o SQLConnection.o test.o
OBJS = $(ROBJS) $(LOBJS)

LSRCS = SQLBase.cpp SQLConnection.cpp SQLException.cpp SQLField.cpp SQLResult.cpp SQLValue.cpp
SRCS =  $(RSRCS) $(LSRCS)

TARGET = $(LOBJS)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS) $(INCLUDE)

#$(TARGET): $(OBJS)
#	$(CC) $(INCLUDE) $(CFLAGS) -o $@ $(OBJS) -lpthread $(LIB)

#.cpp.o:
#	$(CC) $(INCLUDE) $(CFLAGS) -c $<

clean:
	rm $(LOBJS)
	rm $(TARGET)
	rm ./*.o
