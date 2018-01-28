/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/

#ifndef TMDB_VIDEOS_H
#define TMDB_VIDEOS_H

#include <string>
#include <stdint.h>

#include <tmdb/movie/Types.h>

namespace tmdb
{
	namespace movie
	{
		class Videos
		{
		public:
			Videos(){}
			~Videos(){}
		public:
			static movie::types::VideosList get(uint64_t movie_id);		
		};
	}
}


#endif//TMDB_VIDEOS_H