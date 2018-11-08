INC=-I. -I$(ORACLE_HOME)/precomp/public \
	-I$(ORACLE_HOME)/rdbms/demo \
	-I$(ORACLE_HOME)/rdbms/public

CC 	= g++  -g
#CC 	= g++343 -g

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

