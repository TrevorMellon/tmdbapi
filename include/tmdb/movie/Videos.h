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
	class VideosPrivate;
	
	class Videos
	{
	public:
		Videos();
		~Videos();
	public:
		data::VideosList get(uint64_t movie_id);
	private:
		VideosPrivate *_p;
	};
}


#endif//TMDB_VIDEOS_H