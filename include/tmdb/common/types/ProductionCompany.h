#ifndef TMDB_PRODUCTIONCOMPANY_H
#define TMDB_PRODUCTIONCOMPANY_H

#include <string>
#include <stdint.h>

namespace tmdb
{
	namespace types
	{
		struct ProductionCompany
		{
			uint64_t id;
			std::string name;

			ProductionCompany()
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

#endif//TMDB_PRODUCTIONCOMPANY_H