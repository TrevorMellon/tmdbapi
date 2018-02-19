SET (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

SET(Boost_USE_STATIC_LIBS ON)
ADD_DEFINITIONS("-DBOOST_ALL_NO_LIB")

find_package(Boost REQUIRED COMPONENTS serialization date_time regex thread locale filesystem)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    add_definitions( "-DHAS_BOOST" )
endif (Boost_FOUND)

if(WIN32)
	find_package(ICU OPTIONAL_COMPONENTS dt uc i18n)
else(WIN32)
	find_package(ICU OPTIONAL_COMPONENTS data uc i18n)
endif(WIN32)

if(NOT MSVC)
	SET(RAPIDJSON_USE_SSE42 ON)
else(NOT MSVC)
	SET(RAPIDJSON_USE_SSE42 OFF)
endif(NOT MSVC)

SET (RAPIDJSON_USE_SSE2 ON)

find_package(rapidjson REQUIRED)

include_directories("${RAPIDJSON_INCLUDE_DIRS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RAPIDJSON_CXX_FLAGS}")

find_package(OpenSSL)
if(OPENSSL_FOUND)
	include_directories("${OPENSSL_INCLUDE_DIR}")
endif(OPENSSL_FOUND)

find_package(CURL)
if(CURL_FOUND)
	include_directories("${CURL_INCLUDE_DIRS}")
endif(CURL_FOUND)
