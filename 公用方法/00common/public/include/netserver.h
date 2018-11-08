#ifdef WIN32
#include "IocpServer.h"
#include "nettypes.h"
#elif defined LINUX
#include "epollsvr.h"
#include "nettypes.h"
#endif