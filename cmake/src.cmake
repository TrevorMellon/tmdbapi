
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

SET (COMMON_SRC_T
	${PROJECT_SOURCE_DIR}/src/tmdb/common/CastAndCrew.cpp
)

SET(COMMON_INC_T
	${PROJECT_SOURCE_DIR}/include/tmdb/common/CastAndCrew.h
)

SET (COMMON_TYPES_INC_T
	${PROJECT_SOURCE_DIR}/include/tmdb/common/types/ProductionCompany.h
	${PROJECT_SOURCE_DIR}/include/tmdb/common/types/ProductionCountry.h
	${PROJECT_SOURCE_DIR}/include/tmdb/common/types/Languages.h
	${PROJECT_SOURCE_DIR}/include/tmdb/common/types/CastAndCrew.h
	${PROJECT_SOURCE_DIR}/include/tmdb/common/types/People.h
	${PROJECT_SOURCE_DIR}/include/tmdb/common/types/Genre.h
	${PROJECT_SOURCE_DIR}/include/tmdb/common/types/Types.h
)

SET (SRC_MOVIE_T
	${PROJECT_SOURCE_DIR}/src/tmdb/movie/AlternativeTitle.cpp	
	#${PROJECT_SOURCE_DIR}/src/tmdb/movie/CastAndCrew.cpp
	${PROJECT_SOURCE_DIR}/src/tmdb/movie/Keywords.cpp
	${PROJECT_SOURCE_DIR}/src/tmdb/movie/Movie.cpp
	${PROJECT_SOURCE_DIR}/src/tmdb/movie/Videos.cpp	
)

SET(INC_MOVIE_T
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/AlternativeTitle.h	
	#${PROJECT_SOURCE_DIR}/include/tmdb/movie/CastAndCrew.h
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/Keywords.h
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/Movie.h
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/Types.h
	${PROJECT_SOURCE_DIR}/include/tmdb/movie/Videos.h
)

SET(SRC_TV_T
	${PROJECT_SOURCE_DIR}/src/tmdb/tv/TV.cpp
)

SET (INC_TV_T
	${PROJECT_SOURCE_DIR}/include/tmdb/tv/TV.h
	${PROJECT_SOURCE_DIR}/include/tmdb/tv/Types.h
)

SET (SOURCES ${SRC_T} ${INC_T} ${SRC_MOVIE_T} ${INC_MOVIE_T}
	${SRC_TV_T} ${INC_TV_T} ${COMMON_TYPES_INC_T}
	${COMMON_SRC_T} ${COMMON_INC_T}
	)

SOURCE_GROUP("include" FILES ${INC_T})
SOURCE_GROUP("source" FILES ${SRC_T})
SOURCE_GROUP("include\\movie" FILES ${INC_MOVIE_T})
SOURCE_GROUP("source\\movie" FILES ${SRC_MOVIE_T})
SOURCE_GROUP("include\\tv" FILES ${INC_TV_T})
SOURCE_GROUP("source\\tv" FILES ${SRC_TV_T})
SOURCE_GROUP("include\\common\\types" FILES ${COMMON_TYPES_INC_T})