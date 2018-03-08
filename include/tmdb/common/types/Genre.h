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