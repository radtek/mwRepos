CC 	= g++
CFLAGS  += -DLINUX -DLIB_EXPORTS #-DSQL_WCHART_CONVERT

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

#INCLUDE = -I. -I$(ORACLE_HOME)/precomp/public \
	#-I$(ORACLE_HOME)/rdbms/demo \
	#-I$(ORACLE_HOME)/rdbms/public
#LIB = -L$(ORACLE_HOME)/lib -locci -lclntsh

INCLUDE += -I/usr/include
#-I/usr/include
#-I/opt/ibm/db2/V9.7/include
LIB += -L/opt/ibm/db2/V9.7/lib32 -ldb2
#LIB = -L/usr/local/freetds/lib -ltdsodbc

ROBJS = #../../logmgr/LogMgr.o ../../common/LockBase.o ../../common/DerivedLock.o ../../common/SafeLock.o ../../common/pthread_cond_t_ex.o
LOBJS = SQLValue.o SQLField.o SQLResult.o SQLBase.o SQLException.o SQLConnection.o test.o
OBJS = $(ROBJS) $(LOBJS)

TARGET = ./test

$(TARGET): $(OBJS)
	$(CC) $(INCLUDE) $(CFLAGS) -o $@ $(OBJS) -lpthread $(LIB)

.cpp.o:
	$(CC) $(INCLUDE) $(CFLAGS) -c $<

clean:
	rm $(LOBJS)
	rm $(TARGET)
	rm ./*.o
