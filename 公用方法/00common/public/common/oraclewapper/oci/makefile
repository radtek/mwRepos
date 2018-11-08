INC=-I. -I$(ORACLE_HOME)/precomp/public \
	-I$(ORACLE_HOME)/rdbms/demo \
	-I$(ORACLE_HOME)/rdbms/public

CC 	= g++  -g
#CC 	= g++343 -g

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
CFLAGS  += -D_X64_
endif

ORA_LIB= -L$(ORACLE_HOME)/lib -lclntsh

.SUFFIXES: .cpp .hpp .c .h

OBJS = OCIDB.o OCIException.o Exception.o OCIError.o test.o #123.o

TARGET = ./test

$(TARGET):$(OBJS)
	$(CC)  -o  $@  $^   $(ORA_LIB)

.cpp.o:
	$(CC) $(INC) -c  $<

.c.o:
	$(CC) $(INC) -c  $<

clean:
	rm $(OBJS)
	rm $(TARGET)
	rm ./*.o

