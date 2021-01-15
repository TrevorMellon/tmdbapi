/*****************************************************
*                                                    
*  copyright (c) 2018 Trevor Mellon                  
*  For further information contact codified@ignifi.me        
*                                                    
*  Licensed under the BSD 3-clause license           
*  See LICENCE file for more information             
*                                                    
******************************************************/

#ifndef TMDB_COMMON_GENRE_H
#define TMDB_COMMON_GENRE_H

#include <stdint.h>
#include <string>

namespace tmdb
{
	namespace types
	{
		struct Genre
		{
			uint64_t		id;
			std::string		name;

			Genre()
			{
				zero();
			}

			void zero()
			{
				id = 0;
				name = "";
			}

			bool valid()
			{
				if (id == 0 || name.empty())
				{
					return false;
				}
				return true;
			}
		};
	}
}

#endif//TMDB_COMMON_GENRE_H
