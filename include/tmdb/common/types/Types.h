/*****************************************************
*                                                    
*  copyright (c) 2018 Trevor Mellon                  
*  For further information contact codified@ignifi.me        
*                                                    
*  Licensed under the BSD 3-clause license           
*  See LICENCE file for more information             
*                                                    
******************************************************/

#ifndef TMDB_COMMON_TYPES_H
#define TMDB_COMMON_TYPES_H

namespace tmdb
{
	namespace types
	{
		enum class ScanType
		{
			Unset = 0,
			Movie,
			TVShow,
			Person
		};
	}
}

#endif
