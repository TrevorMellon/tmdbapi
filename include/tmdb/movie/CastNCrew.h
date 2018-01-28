/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/


#ifndef TMDB_CAST_AND_CREW_H_
#define TMDB_CAST_AND_CREW_H_

#include <stdint.h>
#include <string>
#include <vector>
#include <tmdb/movie/Types.h>


namespace tmdb
{
	namespace movie
	{
		class CastNCrew
		{
		public:
			CastNCrew(){}
		public:
			static movie::types::CastNCrew get(uint64_t movieid);
		};
	}
}


#endif //TMDB_CAST_AND_CREW__H