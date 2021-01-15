/*****************************************************
*                                                    
*  copyright (c) 2018 Trevor Mellon                  
*  For further information contact codified@ignifi.me        
*                                                    
*  Licensed under the BSD 3-clause license           
*  See LICENCE file for more information             
*                                                    
******************************************************/

#ifndef TMDB_PRODUCTIONCOUNTRY_H
#define TMDB_PRODUCTIONCOUNTRY_H

namespace tmdb
{
	namespace types
	{

		struct ProductionCountry
		{
			std::string iso;//iso 3166 1
			std::string name;

			ProductionCountry()
			{
				zero();
			}

			void zero()
			{
				iso = "";
				name = "";
			}

			bool valid()
			{
				if (iso.empty() || name.empty())
				{
					return false;
				}
				return true;
			}
		};
	}
}

#endif//TMDB_PRODUCTIONCOUNTRY_H
