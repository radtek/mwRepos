CC = cc
CXX = g++
AR = ar
PLATFORM = OS_LINUX
PLATFORM_LDFLAGS = -lodbc -pthread -ldl -lrt -lm -lfilebuffer
PLATFORM_CFLAGS = -g -D__POSIX__ -Wno-write-strings -fPIC -DLINUX  -D_X64_ -DSQL_WCHART_CONVERT -DNDEBUG
PLATFORM_INCL = -I. -I.. -I../../../../common/common 
PLATFORM_LIBDIR = -L/usr/local/lib -L. -L../../../run_env/out/lib/
PLATFORM_SHARED_CFLAGS = -fPIC
PLATFORM_SHARED_EXT = so
PLATFORM_SHARED_LDFLAGS = -shared -Wl,-soname -Wl,
PLATFORM_SHARED_VERSIONED = true

.SUFFIXES: .o .cpp

#OI_OBJS = DerivedLock.o DoubleListEx.o LogMgr.o   LockBase.o  SafeLock.o adapter/Adapter.o adapter/CompatibleEvent.o   adapter/CompatibleThread.o
OI_OBJS = ../../../../00common/public/common/adapter/Adapter.o \
		  ../../../../00common/public/common/adapter/CompatibleEvent.o \
		  ../../../../00common/public/common/adapter/CompatibleThread.o \
		  ../../../../00common/public/common/SafeLock.o \
		  ../../../../00common/public/common/md5_new.o \
		  ../../../../00common/public/common/UTF-8.o \
		  ../../../../00common/public/common/LockBase.o \
		  ../../../../00common/public/common/DerivedLock.o \
		  ../../../../common/common/LogMgr.o \
		  filebuffer.o \
		  FileBufferManage.o
		 #../../../src/mw_http_linux_webservice/LogMgr.o 

SO_LIB_LINK_NAME := ../../../../run_env/out/lib/libfilebuffer.so_r.1.0.0.1
STATICLIB_NAME := ../../../../run_env/out/lib/libfilebuffer.so

BINARY_TEST := test

OI_OBJS_TEST = test.o

all: $(STATICLIB_NAME)  $(SO_LIB_LINK_NAME)
	@echo " **********************************************************************"
	@echo " * build successful  [$(STATICLIB_NAME) $(OI_OBJS_TEST)]               "
	@echo " **********************************************************************"
	@echo ""
	@echo ""
	@echo ""
	
.cpp.o:
	$(CXX) $(PLATFORM_CFLAGS) $(PLATFORM_INCL) -c $^ -o $@

$(STATICLIB_NAME): $(OI_OBJS)
	$(CXX) -shared -o $@ $(OI_OBJS)

$(SO_LIB_LINK_NAME): $(OI_OBJS)
	$(CXX) -shared -o $@ $(OI_OBJS)
	
#$(BINARY_TEST): $(OI_OBJS_TEST)
#	$(CXX) $(PLATFORM_CFLAGS) $(LIB_FLAGS) -o $@ $^ $(PLATFORM_LIBDIR) $(PLATFORM_LDFLAGS) $(PLATFORM_LIBS)
 
clean:
	rm -f *.o ../../../../src/mw_http_linux_webservice/*.o ../../../../src/mw_http_linux_webservice/adapter/*.o  $(STATICLIB_NAME)  $(BINARY_TEST)
