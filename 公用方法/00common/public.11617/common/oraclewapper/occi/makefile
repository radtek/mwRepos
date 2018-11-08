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

INCLUDE += -I/usr/include/oracle/11.2/client
LIB += -L/usr/lib/oracle/11.2/client/lib -locci -lclntsh -lnnz11
#INCLUDE += -I/usr/include/oracle/10.2.0.4/client
#LIB 	+= -L/usr/lib/oracle/10.2.0.4/client/lib -locci -lclntsh -lnnz10

RSRCS =
LSRCS = SQLValue.cpp SQLField.cpp SQLResult.cpp SQLBase.cpp SQLException.cpp SQLConnection.cpp
SRCS = $(RSRCS) $(LSRCS)

ROBJS =
LOBJS = SQLValue.o SQLField.o SQLResult.o SQLBase.o SQLException.o SQLConnection.o test.o
OBJS = $(ROBJS) $(LOBJS)

TARGET = ./test

$(TARGET): $(OBJS)
	$(CC) $(INCLUDE) $(CFLAGS) -o $@ $(OBJS) -lpthread $(LIB)
#	$(CC) $(CFLAGS) -c $(LSRCS) $(INCLUDE)

.cpp.o:
	$(CC) $(CFLAGS) -c $< $(INCLUDE)

clean:
	rm $(LOBJS)
	rm $(TARGET)
	rm ./*.o
