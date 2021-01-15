/*****************************************************
*                                                    
*  copyright (c) 2018 Trevor Mellon                  
*  For further information contact codified@ignifi.me        
*                                                    
*  Licensed under the BSD 3-clause license           
*  See LICENCE file for more information             
*                                                    
******************************************************/


#ifndef TMDB_CAST_AND_CREW_H_
#define TMDB_CAST_AND_CREW_H_

#include <stdint.h>
#include <string>
#include <vector>

#include <tmdb/common/types/Types.h>
#include <tmdb/common/types/CastAndCrew.h>


namespace tmdb
{
	class CastAndCrew
	{
	public:
		CastAndCrew(){}
	public:
		static tmdb::types::CastAndCrew get(uint64_t id, tmdb::types::ScanType scantype);
	};

}


#endif //TMDB_CAST_AND_CREW__H
