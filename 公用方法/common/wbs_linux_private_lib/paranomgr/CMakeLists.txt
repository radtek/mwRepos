set(paranomgr_SRCS
  AccessMssql.cpp
  ParaNoMgr.cpp
  ParaNoMgrImpl.cpp
  )

include_directories(/home/wbs/include)
include_directories(/home/wbs/src/mw_http_linux_webservice)
include_directories(/home/wbs/src/mw_http_linux_webservice/adapter)

add_library(paranomgr ${paranomgr_SRCS})
target_link_libraries(paranomgr odbc odbc_wapper pthread rt dl m)

install(TARGETS paranomgr DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include/paranomgr)

#if(NOT CMAKE_BUILD_NO_EXAMPLES)
#add_subdirectory(tests)
#endif()
