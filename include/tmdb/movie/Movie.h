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
	class MoviePrivate;
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
		std::vector<movie::types::Movie> search(::std::wstring title, int32_t year = -1);
	public:
		std::shared_ptr<movie::types::Combined> scan(std::shared_ptr<movie::types::Combined> &data, ScanTypes type = MovieScan);
	private:
		MoviePrivate		*_p;
	};

}//namespace tmdb

#endif //GENESIS_TMDB_MOVIE__H