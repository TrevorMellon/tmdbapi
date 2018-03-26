/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/

#ifndef TMDB_COMMON_VIDEOS_H
#define TMDB_COMMON_VIDEOS_H

#include <string>
#include <stdint.h>

#include <tmdb/common/types/Types.h>
#include <tmdb/common/types/Videos.h>

namespace tmdb
{

	class Videos
	{
	public:
		Videos(){}
		~Videos(){}
	public:
		static tmdb::types::VideosList get(uint64_t movie_id, tmdb::types::ScanType scantype);
	};
	
}


#endif//TMDB_COMMON_VIDEOS_H