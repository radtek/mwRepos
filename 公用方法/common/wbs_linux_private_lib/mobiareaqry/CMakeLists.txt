set(mobiareaqry_SRCS
  MobiAreaQry.cpp
  )

include_directories(/home/developer/src/nginx_layer/mw_http_linux_webservice)
include_directories(/home/developer/src/nginx_layer/mw_http_linux_webservice/adapter)

add_library(MobiAreaQry64 SHARED ${mobiareaqry_SRCS})
#target_link_libraries(MobiAreaQry64 pthread rt dl m)

install(TARGETS MobiAreaQry64 DESTINATION lib)

file(GLOB HEADERS "*.h")
install(FILES ${HEADERS} DESTINATION include)

#if(NOT CMAKE_BUILD_NO_EXAMPLES)
#add_subdirectory(tests)
#endif()
