cmake_minimum_required (VERSION 3.28)
set( VENDOR ignifi )
project (tmdbapi)

enable_testing()

option(USING_OPENSSL "Use openSSL" OFF)
option(USING_CURL "Use CURL library" ON)
option(DEFAULT_SSL "Use ssl by default" ON)
option(BUILD_STATIC "Build the static library" ON)
option(BUILD_SHARED "Build the shared library" OFF)
option(BUILD_TEST "Building for testing" OFF)
set(SSL_PEM_FILE "../etc/tmdb.pem")

if(BUILD_SHARED AND BUILD_STATIC)
    message(ERROR "Can't build shared and static at the same time")
endif(BUILD_SHARED AND BUILD_STATIC)

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

if(TMDB_TEST)
	SET(SSL_PEM_FILE ${PROJECT_SOURCE_DIR}/etc/tmdb.pem)
endif(TMDB_TEST)

configure_file(templates/config.h.in ${PROJECT_SOURCE_DIR}/include/tmdb/config.h)

include_directories(include)

if(BUILD_STATIC)
    add_library(${PROJECT_NAME} STATIC ${SOURCES} )
endif(BUILD_STATIC)

if(BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
endif(BUILD_SHARED)

if(BUILD_TEST)
	include(cmake/tests.cmake)
	include(cmake/sonar.cmake)
endif(BUILD_TEST)

