CC = cc
CXX = g++
AR = ar
PLATFORM = OS_LINUX
PLATFORM_LDFLAGS = -lodbc -pthread -ldl -lrt -lm
PLATFORM_CFLAGS = -g -D__POSIX__ -Wno-write-strings -fPIC -DLINUX
PLATFORM_INCL = -I. -I..
PLATFORM_LIBDIR = -L/usr/local/lib -L.
PLATFORM_LIBS = -lodbc_wapper
PLATFORM_SHARED_CFLAGS = -fPIC
PLATFORM_SHARED_EXT = so
PLATFORM_SHARED_LDFLAGS = -shared -Wl,-soname -Wl,
PLATFORM_SHARED_VERSIONED = true

.SUFFIXES: .o .cpp

OI_OBJS = DerivedLock.o DoubleListEx.o LogMgr.o   LockBase.o  SafeLock.o adapter/Adapter.o adapter/CompatibleEvent.o   adapter/CompatibleThread.o

STATICLIB_NAME := libLogMgr.a
#BINARY_TEST := test

#OI_OBJS_TEST = test.o

all: $(STATICLIB_NAME) 
	@echo " **********************************************************************"
	@echo " * build successful  [$(STATICLIB_NAME) $(OI_OBJS_TEST)]               "
	@echo " **********************************************************************"
	@echo ""
	@echo ""
	@echo ""
	
.cpp.o:
	$(CXX) $(PLATFORM_CFLAGS) $(PLATFORM_INCL) -c $^ -o $@

$(STATICLIB_NAME): $(OI_OBJS)
	$(AR) -rs $@ $(OI_OBJS)
 
#$(BINARY_TEST): $(OI_OBJS_TEST)
#	$(CXX) $(PLATFORM_CFLAGS) $(LIB_FLAGS) -o $@ $^ $(PLATFORM_LIBDIR) $(PLATFORM_LDFLAGS) $(PLATFORM_LIBS)

install:
	mkdir -p $(HOME)/include/odbcwapper/
	cp -rf *.h $(HOME)/include/odbcwapper/
	mv $(STATICLIB_NAME) $(HOME)/lib/

clean:
	rm -f *.o $(BINARY_TEST) $(STATICLIB_NAME)
