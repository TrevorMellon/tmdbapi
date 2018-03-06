#ifndef TMDB_LANGUAGES_H
#define TMDB_LANGUAGES_H

#include <string>

namespace tmdb
{
	namespace types
	{
		struct Languages
		{
			std::string iso;//iso 639 1
			std::string name;

			Languages()
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

#endif//TMDB_LANGUAGES_H