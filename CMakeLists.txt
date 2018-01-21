cmake_minimum_required (VERSION 2.8)
set( VENDOR jno.io )
project (tmdb)

option(SONAR "Building for sonar scanner" OFF)
option(USING_OPENSSL "Use openSSL" ON)
option(USING_CURL "Use CURL library" ON)
option(DEFAULT_SSL "Default option for ssl" ON)

if(NOT DEFINED TMDB_API_KEY)
	message(FATAL_ERROR "TMDB_API_KEY must be defined")
endif()

include(cmake/src.cmake)
include(cmake/req.cmake)

if(MSVC)
	add_definitions(-D_WIN32_WINNT=_WIN32_WINNT_WIN7)
	add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif(MSVC)

if(OPENSSL_FOUND AND USING_OPENSSL)
	SET(USING_OPENSSL 1)
else(OPENSSL_FOUND AND USING_OPENSSL)
	SET(USING_OPENSSL 0)
endif(OPENSSL_FOUND AND USING_OPENSSL)

if(OPENSSL_FOUND AND DEFAULT_SSL)
	SET(DEFAULT_SSL true)
else(OPENSSL_FOUND AND DEFAULT_SSL)
	SET(DEFAULT_SSL false)
endif(OPENSSL_FOUND AND DEFAULT_SSL)

if(CURL_FOUND AND USING_CURL)
	SET(USING_CURL 1)
else(CURL_FOUND AND USING_CURL)
	SET(USING_CURL 0)
endif(CURL_FOUND AND USING_CURL)

configure_file(templates/config.h.in ${PROJECT_SOURCE_DIR}/include/tmdb/config.h)

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})

#add_definitions(-DTMDB_USE_OPENSSL=1)

add_library(tmdb STATIC ${SOURCES} )

add_subdirectory(tests)

if(SONAR)
	include(cmake/sonar.cmake)
endif(SONAR)

