add_executable(odbc_test odbc_test.cpp ConnPoolMgr.cpp)
add_executable(timeout_test timeout_test.cpp)
add_executable(bigtable_test bigtable_test.cpp ConnPoolMgr.cpp)

target_link_libraries(odbc_test odbc_wapper)
target_link_libraries(timeout_test odbc_wapper)
target_link_libraries(bigtable_test odbc_wapper)

install(TARGETS odbc_test timeout_test bigtable_test DESTINATION bin)
