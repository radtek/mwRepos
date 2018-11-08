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

INCLUDE += -I/usr/include/mysql
LIB += -L/usr/lib/mysql -lmysqlclient

ROBJS =
LOBJS = SQLBase.o SQLConnection.o SQLException.o SQLField.o SQLResult.o SQLValue.o
OBJS = $(ROBJS) $(LOBJS)
RSRCS =
LSRCS = SQLBase.cpp SQLConnection.cpp SQLException.cpp SQLField.cpp SQLResult.cpp SQLValue.cpp
SRCS =  $(RSRCS) $(LSRCS)

TARGET = $(LOBJS)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -c $(SRCS) $(INCLUDE)

#.cpp.o:
#	$(CC) $(CFLAGS) -c $< $(INCLUDE)

clean:
	rm $(LOBJS)
	rm ./*.o
