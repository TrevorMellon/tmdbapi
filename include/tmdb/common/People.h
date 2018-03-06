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
			Male = 0,
			Female,
			Unset,

		};
		
		struct BasicPerson
		{
			uint64_t	id;
			std::string name;
			Gender gender;
			std::string profile_path;
		};
	}
}

#endif//TMDB_COMMON_PEOPLE_H