
SET (SRC_T 
	${PROJECT_SOURCE_DIR}/src/tmdb/ApiGetJson.cpp
	${PROJECT_SOURCE_DIR}/src/tmdb/movie/AlternativeTitle.cpp	
	${PROJECT_SOURCE_DIR}/src/tmdb/movie/CastNCrew.cpp
	${PROJECT_SOURCE_DIR}/src/tmdb/movie/Keywords.cpp
	${PROJECT_SOURCE_DIR}/src/tmdb/movie/Movie.cpp
	${PROJECT_SOURCE_DIR}/src/tmdb/movie/Videos.cpp	
)

SET (INC_T
	${PROJECT_SOURCE_DIR}/include/tmdb/ApiGetJson.h
	${PROJECT_SOURCE_DIR}/include/tmdb/util.h
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/AlternativeTitle.h	
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/CastNCrew.h
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/Keywords.h
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/Movie.h
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/Types.h
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/Videos.h
	${CMAKE_BINARY_DIR}/include/tmdb/config.h
)

include_directories(${CMAKE_BINARY_DIR}/include/)
include_directories(inl)

SET (SOURCES ${SRC_T} ${INC_T} )