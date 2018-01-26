
SET (SRC_T 
	${PROJECT_SOURCE_DIR}/src/tmdb/ApiGet.cpp
	${PROJECT_SOURCE_DIR}/src/tmdb/Configuration.cpp
	${PROJECT_SOURCE_DIR}/src/tmdb/Iso.cpp
	${PROJECT_SOURCE_DIR}/src/tmdb/ImageDownloader.cpp
)

SET (INC_T
	${PROJECT_SOURCE_DIR}/include/tmdb/ApiGet.h
	${PROJECT_SOURCE_DIR}/include/tmdb/Util.h
	${PROJECT_SOURCE_DIR}/include/tmdb/Configuration.h
	${PROJECT_SOURCE_DIR}/include/tmdb/Iso.h
	${PROJECT_SOURCE_DIR}/include/tmdb/IsoTypes.h
	${PROJECT_SOURCE_DIR}/include/tmdb/ImageDownloader.h
	
	${PROJECT_SOURCE_DIR}/include/tmdb/config.h	
)

SET (SRC_MOVIE_T
	${PROJECT_SOURCE_DIR}/src/tmdb/movie/AlternativeTitle.cpp	
	${PROJECT_SOURCE_DIR}/src/tmdb/movie/CastNCrew.cpp
	${PROJECT_SOURCE_DIR}/src/tmdb/movie/Keywords.cpp
	${PROJECT_SOURCE_DIR}/src/tmdb/movie/Movie.cpp
	${PROJECT_SOURCE_DIR}/src/tmdb/movie/Videos.cpp	
)

SET(INC_MOVIE_T
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/AlternativeTitle.h	
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/CastNCrew.h
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/Keywords.h
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/Movie.h
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/Types.h
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/Videos.h
)

SET (SOURCES ${SRC_T} ${INC_T} ${SRC_MOVIE_T} ${INC_MOVIE_T})

SOURCE_GROUP("include" FILES ${INC_T})
SOURCE_GROUP("source" FILES ${SRC_T})
SOURCE_GROUP("include\\movie" FILES ${INC_MOVIE_T})
SOURCE_GROUP("source\\movie" FILES ${SRC_MOVIE_T})