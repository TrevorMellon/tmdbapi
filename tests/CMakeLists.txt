
include(../cmake/src.cmake)
include(../cmake/req.cmake)

if(CMAKE_BUILD_TYPE MATCHES "Release")

	SET(GTEST_MSVC_SEARCH "MD")
	find_package(GTest)

	include_directories(${GTEST_INCLUDE_DIRS})
	include_directories(../include)
	include_directories(../inl)
	include_directories(${Boost_INCLUDE_DIRS})
	include_directories(${OPENSSL_INCLUDE_DIR})

	#add_definitions(-DTMDB_USE_OPENSSL=1)
	
	add_executable(tmdbtest test.cpp
		${SOURCES}
	)

	target_link_libraries(tmdbtest 
				${GTEST_BOTH_LIBRARIES} 
				${Boost_LIBRARIES}
				${ICU_LIBRARIES}
				${CURL_LIBRARIES}
				${OPENSSL_LIBRARIES}
				)
	
	add_test(test1 tmdbtest)

endif(CMAKE_BUILD_TYPE MATCHES "Release")