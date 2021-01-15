/*****************************************************
*                                                    
*  copyright (c) 2018 Trevor Mellon                  
*  For further information contact codified@ignifi.me        
*                                                    
*  Licensed under the BSD 3-clause license           
*  See LICENCE file for more information             
*                                                    
******************************************************/

#ifndef TMDB_COMMON_PEOPLE_H
#define TMDB_COMMON_PEOPLE_H

#include <stdint.h>
#include <string>

namespace tmdb
{
	namespace types
	{
		enum class Gender
		{
			Unset = 0,
			Male,
			Female,
		};
		
		struct BasicPerson
		{
			uint64_t	id;
			std::string name;
			Gender gender;
			std::string profile_path;

			BasicPerson()
			{
				zero();
			}

			virtual void zero()
			{
				id = 0;
				name = "";
				gender = Gender::Unset;
				profile_path = "";
			}

			virtual bool valid()
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

#endif//TMDB_COMMON_PEOPLE_H
