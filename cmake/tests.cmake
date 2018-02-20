find_package(GTest)

#add_definitions(-DTMDB_USE_OPENSSL=1)

SET(TEST_SRC ${PROJECT_SOURCE_DIR}/tests/test.cpp)

add_executable(tmdbtest ${TEST_SRC}
	${SOURCES}
)

target_link_libraries(tmdbtest 
			${GTEST_BOTH_LIBRARIES} 
			${Boost_LIBRARIES}
			${ICU_LIBRARIES}
			)
	
target_include_directories(tmdbtest PUBLIC ${GTEST_INCLUDE_DIRS})
target_include_directories(tmdbtest PUBLIC ../include)
target_include_directories(tmdbtest PUBLIC ${Boost_INCLUDE_DIRS})

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads)

if(NOT MSVC)
	target_link_libraries(tmdbtest Threads::Threads)
endif(NOT MSVC)
	
if(USING_OPENSSL)
	target_link_libraries(tmdbtest 
			${OPENSSL_LIBRARIES}
			)
	target_include_directories(tmdbtest PUBLIC ${OPENSSL_INCLUDE_DIR})		
endif()
	
if(USING_CURL)
	target_link_libraries(tmdbtest 
			${CURL_LIBRARIES}
			)
	target_include_directories(tmdbtest PUBLIC ${CURL_INCLUDE_DIR})
endif()

add_test(NAME test1 COMMAND ${PROJECT_BINARY_DIR}/tmdbtest)

