#ifndef TMDB_CASTANDCREW_H
#define TMDB_CASTANDCREW_H

#include <stdint.h>
#include <string>
#include <vector>
#include <tmdb/common/types/People.h>

namespace tmdb
{
	namespace types
	{

		struct Crew : public tmdb::types::BasicPerson
		{
						
			std::string department;
			std::string job;
			std::string credit_id;
			

			Crew()
			{
				zero();
			}

			void zero()
			{
				credit_id = "";
				department = "";
				job = "";
				
				types::BasicPerson::zero();
			}

			bool valid()
			{
				if (!types::BasicPerson::valid())
				{
					return false;
				}
				return true;
			}

			void operator= (const types::BasicPerson &other)
			{
				zero();
				this->id = other.id;
				this->gender = other.gender;
				this->name = other.name;
				this->profile_path = other.profile_path;
			}

			void operator+= (const types::BasicPerson &other)
			{
				this->id = other.id;
				this->gender = other.gender;
				this->name = other.name;
				this->profile_path = other.profile_path;
			}
		};

		struct Cast : public tmdb::types::BasicPerson
		{
			std::string	character;
			uint64_t	cast_id;
			std::string	credit_id;
			uint32_t	order;
			

			Cast()
			{
				zero();
			}

			void zero()
			{
				cast_id = 0;
				character = "";
				credit_id = "";

				order = 0;
				types::BasicPerson::zero();
			}

			bool valid()
			{
				if (!types::BasicPerson::valid())
				{
					return false;
				}
				return true;
			}

			void operator=(const types::BasicPerson &other)
			{
				zero();
				this->id = other.id;
				this->gender = other.gender;
				this->name = other.name;
				this->profile_path = other.profile_path;
			}

			void operator+=(const types::BasicPerson &other)
			{
				this->id = other.id;
				this->gender = other.gender;
				this->name = other.name;
				this->profile_path = other.profile_path;
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