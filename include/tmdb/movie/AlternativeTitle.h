/*****************************************************
*                                                    
*  copyright (c) 2018 Trevor Mellon                  
*  For further information contact codified@ignifi.me        
*                                                    
*  Licensed under the BSD 3-clause license           
*  See LICENCE file for more information             
*                                                    
******************************************************/

#ifndef TMDB_ALTERNATIVE_TITLE_H_
#define TMDB_ALTERNATIVE_TITLE_H_

#include <string>
#include <vector>

#include <tmdb/movie/Types.h>

namespace tmdb
{
	namespace movie
	{
		class AlternativesTitle
		{
		public:
			AlternativesTitle(){}
		public:
			static types::AlternativeTitles get(const uint64_t id);
		};
	}//namespace movie
}//namepace tmdb

#endif//TMDB_ALTERNATIVE_TITLE__H
