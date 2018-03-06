#ifndef TMDB_CASTANDCREW_H
#define TMDB_CASTANDCREW_H

#include <stdint.h>
#include <string>
#include <vector>

namespace tmdb
{
	namespace types
	{

		struct Crew
		{
			uint64_t id;
			std::string name;
			std::string department;
			std::string job;
			std::string credit_id;
			std::string profile_path;

			Crew()
			{
				zero();
			}

			void zero()
			{
				credit_id = "";
				department = "";
				id = 0;
				job = "";
				name = "";
				profile_path = "";
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

		struct Cast
		{
			uint64_t	id;
			std::string	name;
			std::string	character;
			uint64_t	cast_id;
			std::string	credit_id;
			uint32_t	order;
			std::string	profile_path;

			Cast()
			{
				zero();
			}

			void zero()
			{
				cast_id = 0;
				character = "";
				credit_id = "";
				id = 0;
				name = "";
				order = 0;
				profile_path = "";
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

		struct CastAndCrew
		{
			uint64_t	id;
			std::vector<Cast> cast;
			std::vector<Crew> crew;

			CastAndCrew()
			{
				zero();
			}

			void zero()
			{
				id = 0;
				cast.clear();
				crew.clear();
			}

			bool valid()
			{
				if (id == 0 || cast.empty())
				{
					return false;
				}
				return true;
			}
		};

	}
}

#endif//TMDB_CASTANDCREW_H