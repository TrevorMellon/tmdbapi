/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/

#ifndef TMDB_MOVIE_H_
#define TMDB_MOVIE_H_

#include <string>
#include <stdint.h>
#include <vector>
#include <memory>

#include <tmdb/movie/Types.h>
#include <tmdb/movie/Keywords.h>

namespace tmdb
{
	class MovieKPrivate;
	class ApiGet;

	class Movie
	{
	public:
		enum ScanTypes
		{
			AllScan = 0,
			MovieScan,
			CastScan,
			AltTitlesScan,
			VideosScan,
			KeywordsScan
		};
	public:
		Movie();
		~Movie();
	public:
		uint64_t searchFilename(::std::wstring filename);
	public:
		uint64_t search(::std::wstring title, int32_t year = -1);
		std::vector<data::Movie> getSearchResults();
	public:
		std::shared_ptr<data::MovieCombined> scan(std::shared_ptr<data::MovieCombined> data, ScanTypes type = MovieScan);
	public:
		/*
		std::shared_ptr<data::Movie>				scanMainMovie();
		std::shared_ptr<data::AlternativeTitles>	scanAlternativeTitles();
		std::shared_ptr<data::CastNCrew>			scanCastAndCrew();
		std::shared_ptr<data::VideosList>			scanVideos();
		std::shared_ptr<data::KeywordList>			scanKeywords();
		*/

	private:
		MovieKPrivate		*_p;
	};

}//namespace tmdb

#endif //GENESIS_TMDB_MOVIE__H