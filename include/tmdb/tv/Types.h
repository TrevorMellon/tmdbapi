#ifndef TMDB_TV_TYPES_H
#define TMDB_TV_TYPES_H

#include <string>
#include <vector>

#include <tmdb/common/People.h>
#include <tmdb/common/Genre.h>
#include <tmdb/common/ProductionCompany.h>

namespace tmdb
{
	namespace tv
	{
		namespace types
		{
			//! /todo Networks, Seasons
			struct TV
			{
				std::string		backdrop_path;
				std::vector<tmdb::types::BasicPerson> created_by;
				uint32_t		episode_runtime;
				std::string		first_air_date;
				time_t			first_air_date_t;
				std::vector<tmdb::types::Genre> genres;
				std::string		homepage;
				uint64_t		id;
				bool			in_production;
				std::vector<std::string>	languages;
				std::string		last_air_date;
				time_t			last_air_date_t;
				std::string		name;
				std::vector<tmdb::types::ProductionCompany> production_companies;
				uint64_t		number_of_episodes;
				uint64_t		number_of_seasons;
				std::vector<std::string> origin_country;
				std::string		original_language;
				std::string		original_name;
				std::string		overview;
				float			popularity;
				std::string		poster_path;
				std::string		status;
				std::string		type;
				uint32_t		vote_average;
				uint64_t		vote_count;

				TV()
				{
					zero();
				}

				void zero()
				{
					backdrop_path = "";
					created_by.clear();
					episode_runtime = 0;
					first_air_date = "";
					first_air_date_t = 0;
					genres.clear();
					homepage = "";
					id = 0;
					in_production = false;
					languages.clear();
					last_air_date = "";
					last_air_date_t = 0;
					name = "";
					production_companies.clear();
					number_of_episodes = 0;
					number_of_seasons = 0;
					origin_country.clear();
					original_language = "";
					original_name = "";
					overview = "";
					popularity = 0.0f;
					poster_path = "";
					status = "";
					type = "";
					vote_average = 0;
					vote_count = 0;

				}

			};


			struct Combined
			{
				uint64_t id;
				
				tmdb::tv::types::TV		tv;

				Combined()
				{
					zero();
				}

				void zero()
				{
					tv.zero();
				}

				bool verifyID()
				{
					if (id == tv.id)
					{
						return true;
					}
					return false;
				}
			};
		}
	}
}

#endif//TMDB_TV_TYPES_H