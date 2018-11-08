#ifdef WIN32
#include "IocpClient.h"
#include "nettypes.h"
#elif defined LINUX
#include "EpollClient.h"
#include "nettypes.h"
#endif