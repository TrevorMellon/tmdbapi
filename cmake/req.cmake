SET (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

SET(Boost_USE_STATIC_LIBS ON)
ADD_DEFINITIONS("-DBOOST_ALL_NO_LIB")

find_package(Boost REQUIRED COMPONENTS serialization date_time regex thread locale)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    add_definitions( "-DHAS_BOOST" )
endif (Boost_FOUND)

if(WIN32)
	find_package(ICU COMPONENTS dt uc i18n)
elseif(WIN32)
	find_package(ICU COMPONENTS data uc i18n)
endif(WIN32)

find_package(CURL)

if(NOT MSVC)
	SET(RAPIDJSON_USE_SSE42 ON)
else(MSVC)
	SET(RAPIDJSON_USE_SSE42 OFF)
endif(NOT MSVC)

SET (RAPIDJSON_USE_SSE2 ON)

find_package(rapidjson)

include_directories("${RAPIDJSON_INCLUDE_DIRS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RAPIDJSON_CXX_FLAGS}")

find_package(OpenSSL)

find_package(CURL)
