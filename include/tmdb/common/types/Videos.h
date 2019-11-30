#ifndef TMDB_COMMON_TYPES_VIDEOS_H
#define TMDB_COMMON_TYPES_VIDEOS_H

#include <string>
#include <vector>
#include <stdint.h>

namespace tmdb
{
	namespace types
	{

		struct VideosData
		{
            std::string	id;
			std::string iso_639_1;
			std::string iso_3166_1;
			std::string key;
			std::string name;
			std::string site;
			uint32_t size;
			std::string type;

			VideosData()
			{
				zero();
			}

			void zero()
			{
                id = "";
				iso_639_1 = "";
				iso_3166_1 = "";
				key = "";
				name = "";
				site = "";
				size = 0;
				type = "";
			}

			bool valid()
			{
                if (id.empty() || name.empty())
				{
					return false;
				}
				return true;
			}
		};

		struct VideosList
		{
			std::vector<VideosData> videos;

			VideosList()
			{
				zero();
			}

			void zero()
			{
				videos.clear();
			}

			bool valid()
			{
				if (videos.empty())
				{
					return false;
				}
				return true;
			}
		};
	}
}

#endif //TMDB_COMMON_TYPES_VIDEOS_H
