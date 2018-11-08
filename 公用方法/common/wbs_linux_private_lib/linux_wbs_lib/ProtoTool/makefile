CC = cc
CXX = g++
AR = ar
export WBS_ROOT := $(shell cd ../../../../;pwd)


PLATFORM = OS_LINUX
PLATFORM_LDFLAGS = -lodbc -pthread -ldl -lrt -lm
PLATFORM_CFLAGS = -g -D__POSIX__ -Wno-write-strings -fPIC -DLINUX -D_X64_ 
PLATFORM_INCL = -I. -I.. -I $(WBS_ROOT)/src/mw_mo_push_layer \
	-I $(WBS_ROOT)/00common/public/common -I $(WBS_ROOT)/common/common -I $(WBS_ROOT)/00common/public/include
PLATFORM_LIBDIR = -L/usr/local/lib -L.
PLATFORM_LIBS = -lodbc_wapper
PLATFORM_SHARED_CFLAGS = -fPIC
PLATFORM_SHARED_EXT = so
PLATFORM_SHARED_LDFLAGS = -shared -Wl,-soname -Wl,
PLATFORM_SHARED_VERSIONED = true

.SUFFIXES: .o .cpp

#OI_OBJS = DerivedLock.o DoubleListEx.o LogMgr.o   LockBase.o  SafeLock.o adapter/Adapter.o adapter/CompatibleEvent.o   adapter/CompatibleThread.o
#OI_OBJS =  adapter/Adapter.o \
#		   adapter/CompatibleEvent.o \
#		  common/md5_new.o \
#		  tools/cJSON.o \
#		  tools/JsonParse.o \
#		  tools/XMLParse.o \
#		  CParseToolImpl.o \
#		  WBSDLL.o  \
#		  Global.o \
#		  UTF-8.o \
#		  SafeLock.o \
#		  LockBase.o \
#		 DoubleListEx.o \
#		 DerivedLock.o

OI_OBJS =  \
	CParseToolImpl.o \
	WBSDLL.o  \
	PushHelper.o  

ROBJS = \
	${WBS_ROOT}/00common/public/common/adapter/Adapter.o \
	${WBS_ROOT}/00common/public/common/adapter/CompatibleThread.o \
	${WBS_ROOT}/00common/public/common/adapter/CompatibleEvent.o \
	${WBS_ROOT}/00common/public/common/DerivedLock.o \
	${WBS_ROOT}/00common/public/common/LockBase.o \
	${WBS_ROOT}/00common/public/common/SafeLock.o \
	${WBS_ROOT}/00common/public/common/md5_new.o \
	${WBS_ROOT}/00common/public/common/UTF-8.o \
	${WBS_ROOT}/00common/public/common/DoubleListEx.o \
	${WBS_ROOT}/common/common/Global.o \
	${WBS_ROOT}/common/common/LogMgr.o \
	${WBS_ROOT}/common/common/tools/cJSON.o \
	${WBS_ROOT}/common/common/tools/JsonParse.o \
	${WBS_ROOT}/common/common/tools/XMLParse.o \






SO_LIB_LINK_NAME := $(WBS_ROOT)/run_env/out/lib/libparsetool.so
STATICLIB_NAME := $(WBS_ROOT)/run_env/out/lib/libparsetool.so_r.1.0.0.1

#BINARY_TEST := test
#OI_OBJS_TEST = test.o

all: 00common common $(STATICLIB_NAME) $(SO_LIB_LINK_NAME)
	@echo " **********************************************************************"
	@echo " * build successful  [$(STATICLIB_NAME) $(OI_OBJS_TEST)]               "
	@echo " **********************************************************************"
	@echo ""
	@echo ""
	@echo ""
	
.cpp.o:
	$(CXX) $(PLATFORM_CFLAGS) $(PLATFORM_INCL) -c $^ -o $@

$(STATICLIB_NAME): $(OI_OBJS)
	$(CXX) -shared -o			$@ \
	$(OI_OBJS) \
	$(ROBJS) 
$(SO_LIB_LINK_NAME): $(OI_OBJS)
	$(CXX) -shared -o			$@ \
	$(OI_OBJS)  \
	$(ROBJS) 

#$(AR) -rs $@ $(OI_OBJS)
#$(BINARY_TEST): $(OI_OBJS_TEST)
#	$(CXX) $(PLATFORM_CFLAGS) $(LIB_FLAGS) -o $@ $^ $(PLATFORM_LIBDIR) $(PLATFORM_LDFLAGS) $(PLATFORM_LIBS)

#install:
#	mkdir -p $(HOME)/include/odbcwapper/
#	cp -rf *.h $(HOME)/include/odbcwapper/
#	mv $(STATICLIB_NAME) $(HOME)/lib/

clean: clean_00common clean_common 
	rm -f $(OI_OBJS) $(BINARY_TEST) $(STATICLIB_NAME) $(SO_LIB_LINK_NAME)
	@echo "clean done."
	@echo -e "\n"

00common:
	cd ${WBS_ROOT}/00common/public/common/adapter && make GCCTYPE=X86_64
	@echo -e "\n"
	cd ${WBS_ROOT}/00common/public/common && make GCCTYPE=X86_64
	@echo -e "\n"

clean_00common:
	cd ${WBS_ROOT}/00common/public/common/adapter && make clean;
	@echo -e "\n"
	cd ${WBS_ROOT}/00common/public/common && make clean;
	@echo -e "\n"
common:
	cd ${WBS_ROOT}/common/common && make GCCTYPE=X86_64
	@echo -e "\n"
	cd ${WBS_ROOT}/common/common/tools && make GCCTYPE=X86_64
	@echo -e "\n"
clean_common:
	cd ${WBS_ROOT}/common/common && make clean 
	@echo -e "\n"
	cd ${WBS_ROOT}/common/common/tools && make clean
	@echo -e "\n"


print:
	@echo WBS_ROOT = $(WBS_ROOT)


#1、如果有修改 06wbs_std_linux\common\common 请一定重编libparsetool.so库，并上传SVN。
#2、如果有修改 06wbs_std_linux\00common\public\common  请一定重编libparsetool.so库，并上传SVN。
