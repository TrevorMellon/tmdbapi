/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/

#ifndef TMDB_KEYWORDS_H
#define TMDB_KEYWORDS_H

#include <stdint.h>
#include <tuple>
#include <string>
#include <vector>

#include <tmdb/movie/Types.h>

namespace tmdb
{
	namespace movie
	{
		class Keywords
		{
		public:
			Keywords(){}
			~Keywords(){}
		public:
			static types::KeywordList get(uint64_t key);
		};
	}//namespace movie
}//namespace tmdb;

#endif //TMDB_KEYWORDS_H