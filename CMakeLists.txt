cmake_minimum_required (VERSION 2.8)
set( VENDOR jno.io )
project (tmdb)

option(SONAR "Building for sonar scanner" OFF)
option(USING_OPENSSL "Use openSSL" ON)

if(NOT DEFINED TMDB_API_KEY)
	message(FATAL_ERROR "TMDB_API_KEY must be defined")
endif()

include(cmake/src.cmake)
include(cmake/req.cmake)

if(OPENSSL_FOUND AND USING_OPENSSL)
	SET(USING_OPENSSL 1)
else(OPENSSL_FOUND AND USING_OPENSSL)
	SET(USING_OPENSSL 0)
endif(OPENSSL_FOUND AND USING_OPENSSL)

configure_file(templates/config.h.in ${PROJECT_SOURCE_DIR}/include/tmdb/config.h)

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})

#add_definitions(-DTMDB_USE_OPENSSL=1)

add_library(tmdb STATIC ${SOURCES} )

add_subdirectory(tests)

if(SONAR)
	include(cmake/sonar.cmake)
endif(SONAR)

