/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/


#ifndef GENESIS_TMDB_TYPES_H_
#define GENESIS_TMDB_TYPES_H_

#include <string>
#include <stdint.h>
#include <vector>

namespace tmdb
{
	namespace movie
	{
		namespace types
		{

			//***************************************
			//
			//			Movie Main Types
			//
			//***************************************

			struct Collection
			{
				uint64_t			id;
				std::string		name;
				std::string		poster_path;
				std::string		backdrop_path;

				Collection()
				{
					zero();
				}

				void zero()
				{
					id = 0;
					name = "";
					poster_path = "";
					backdrop_path = "";
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

			//***************************************
			//
			//		Languages Type
			//
			//***************************************

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

			//***************************************
			//
			//		Movie Type
			//
			//***************************************


			struct Movie
			{
				bool _scanned = false;

				uint64_t	id;
				std::string	imdb_id;

				bool adult;
				std::string backdrop_path;

				Collection belongs_to_collection;

				uint64_t		budget;

				std::vector<Genre> genres;

				std::string homepage;

				std::string original_language;
				std::string original_title;

				std::string overview;

				double popularity;

				std::string poster_path;

				std::vector<ProductionCompany> production_companies;
				std::vector<ProductionCountry> production_countries;

				std::string	release_date;//2012-11-31
				time_t		release_date_t;

				uint64_t revenue;
				uint32_t runtime;

				std::vector<Languages> spoken_languages;

				std::string status;// "Released"

				std::string tagline;
				std::string title;

				bool video;

				double vote_average;
				uint64_t vote_count;

				Movie()
				{
					zero();
				}

				void zero()
				{
					adult = false;
					backdrop_path = "";

					belongs_to_collection.zero();

					budget = 0;

					genres.clear();

					id = 0;
					imdb_id = "";

					original_language = "";
					original_title = "";

					overview = "";

					popularity = 0.0f;

					poster_path = "";

					production_companies.clear();
					production_countries.clear();

					release_date = "";

					release_date_t = 0;

					revenue = 0;
					runtime = 0;

					video = false;

					vote_average = 0.0f;
					vote_count = 0;

					spoken_languages.clear();

					status = "";

					tagline = "";
					title = "";
				}

				bool valid()
				{
					if (id == 0 || title.empty())
					{
						return false;
					}
					return true;
				}
			};

			//***************************************
			//
			//		Alternative Titles Types
			//
			//***************************************

			struct AlternativeTitle
			{
				std::string iso_3166_1;
				std::string title;

				AlternativeTitle()
				{
					zero();
				}

				void zero()
				{
					iso_3166_1 = "";
					title = "";
				}

				bool valid()
				{
					if (iso_3166_1.empty() || title.empty())
					{
						return false;
					}
					return true;
				}
			};

			struct AlternativeTitles
			{
				std::vector<AlternativeTitle> titles;

				AlternativeTitles()
				{
					zero();
				}

				void zero()
				{
					titles.clear();
				}
			};

			//***************************************
			//
			//		Cast And Crew Types
			//
			//***************************************

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

			struct CastNCrew
			{
				uint64_t	id;
				std::vector<Cast> cast;
				std::vector<Crew> crew;

				CastNCrew()
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

			//***************************************
			//
			//		Videos Type
			//
			//***************************************

			struct VideosData
			{
				uint64_t	id;
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
					id = 0;
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
					if (id == 0 || name.empty())
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

			//***************************************
			//
			//		Keyword Type
			//
			//***************************************

			typedef std::pair<int32_t, std::string> KeywordPair;

			typedef std::vector<KeywordPair> KeywordList;


			//***************************************
			//
			//		Combined Type
			//
			//***************************************

			struct Combined
			{
				Movie				movie;
				CastNCrew			crew;
				AlternativeTitles	alt_titles;
				VideosList			videos;
				KeywordList			keywords;

				::std::string			queryString;
				int32_t				year;

				Combined()
				{
					zero();
				}

				void zero()
				{
					movie.zero();
					crew.zero();
					alt_titles.zero();
					videos.zero();

					queryString = "";
					year = -1;
				}

				bool verifyID()
				{
					uint64_t id = movie.id;
					if (id == crew.id)
					{
						return true;
					}
					return false;
				}

				uint64_t id()
				{
					if (verifyID())
					{
						return movie.id;
					}
					return 0;
				}

				void setId(const uint64_t &id)
				{
					movie.id = id;
					crew.id = id;
				}

				Combined &operator = (const Combined &other)
				{
					this->movie = other.movie;
					this->crew = other.crew;
					this->alt_titles = other.alt_titles;
					this->videos = other.videos;
					this->queryString = other.queryString;
					this->year = other.year;
				}
			};
		}//namespace types;
	}//namespace movie
}//namespace tmdb;

#endif //TMDB_TYPES__H