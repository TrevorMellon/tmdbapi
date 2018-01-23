


find_package(GTest)

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(../include)
include_directories(../inl)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})

#add_definitions(-DTMDB_USE_OPENSSL=1)

SET(TEST_SRC ${PROJECT_SOURCE_DIR}/tests/test.cpp)

add_executable(tmdbtest ${TEST_SRC}
	${SOURCES}
)

target_link_libraries(tmdbtest 
			${GTEST_BOTH_LIBRARIES} 
			${Boost_LIBRARIES}
			${ICU_LIBRARIES}
			${CURL_LIBRARIES}
			${OPENSSL_LIBRARIES}
			)

add_test(NAME test1 COMMAND ${PROJECT_BINARY_DIR}/tmdbtest)

