set(odbc_SRCS
  Connection.cpp
  Exception.cpp
  PreparedStatement.cpp
  ResultSet.cpp
  Thread.cpp)

add_library(odbc_wapper ${odbc_SRCS})
target_link_libraries(odbc_wapper odbc pthread rt dl m)

install(TARGETS odbc_wapper DESTINATION lib)
file(GLOB HEADERS "*.h")

install(FILES ${HEADERS} DESTINATION include/odbc)

if(NOT CMAKE_BUILD_NO_EXAMPLES)
  add_subdirectory(tests)
endif()
