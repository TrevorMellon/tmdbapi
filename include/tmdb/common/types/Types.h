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