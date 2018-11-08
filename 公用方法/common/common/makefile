#   build all C/C++ file that file path is 06wbs_std_linux\common\common 
#	make GCCTYPE=X86_64

CC 	= g++
CFLAGS  += -DLINUX -DSQL_WCHART_CONVERT -DLIB_EXPORTS

ARCH = _x86_
CFLAGS  += -Wall -g -DDEBUG

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


#LOBJS：	要编译生成的.o文件
#ROBJS：	引用已编好的.o文件。依赖的.o文件。
#LSRCS:		要编译的.cpp文件，与.o文件一一对应。
#INCLUDES：	编译要引用的头文件路径

export WBS_ROOT := $(shell cd ../../;pwd)

#export WBS_ROOT := ../..

INCLUDES= -I ./ -I  ../../include -I ../../00common/public/common/ \
	-I $(WBS_ROOT)/src/mw_mo_push_layer
CFLAGS += $(INCLUDES)

LOBJS = LogMgr.o Global.o FileMoRpt.o  PtMonitor.o MonInfoWriter.o ConnPoolMgr.o 
LSRCS =	LogMgr.cpp  Global.cpp FileMoRpt.cpp PtMonitor.cpp MonInfoWriter.cpp ConnPoolMgr.cpp

#ROBJS = ../../00common/public/common/DerivedLock.o \
#	../../00common/public/common/LockBase.o \
#	../../00common/public/common/SafeLock.o \
#	../../00common/public/common/adapter/Adapter.o 

OBJS = $(ROBJS) $(LOBJS)
RSRCS =
SRCS =  $(RSRCS) $(LSRCS)	#编译依赖的.cpp文件

TARGET = $(LOBJS) #编译目标

$(TARGET): 00common $(SRCS)
	$(CC) $(CFLAGS) -c $(LSRCS)
	@echo compile is finish.


clean: clean_00common
	rm -f $(LOBJS)
	rm -f ./*.o

print:
	@echo WBS_ROOT =	${WBS_ROOT}
	@echo INCLUDES =	${INCLUDES}
	@echo CC =	${CC}
	@echo CFLAGS =	${CFLAGS}
	@echo LSRCS =	${LSRCS}
	@echo $(CC) $(CFLAGS) -c $(LSRCS)

00common:
	cd ${WBS_ROOT}/00common/public/common/adapter && make GCCTYPE=X86_64 VERTYPE=DEBUG
	@echo -e "\n"
	cd ${WBS_ROOT}/00common/public/common && make GCCTYPE=X86_64 VERTYPE=DEBUG
	@echo -e "\n"

clean_00common:
	cd ${WBS_ROOT}/00common/public/common/adapter && make clean;
	@echo -e "\n"
	cd ${WBS_ROOT}/00common/public/common && make clean;
	@echo -e "\n"
#   build all C/C++ file that file path is 06wbs_std_linux\common\common 
#	make GCCTYPE=X86_64

