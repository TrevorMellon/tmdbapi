/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/

#include <tmdb/tv/TV.h>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>
#include <boost/thread.hpp>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <tmdb/ApiGet.h>

#include <tmdb/tv/Types.h>

#include <tmdb/common/CastAndCrew.h>
#include <tmdb/common/Videos.h>
//#include <tmdb/movie/Keywords.h>
//#include <tmdb/movie/AlternativeTitle.h>
#include <tmdb/Util.h>

#include <sstream>

# include <rapidjson/rapidjson.h>
# include <rapidjson/document.h>



namespace tmdb
{

	class TVPrivate
	{
	public:
		boost::mutex mtx;
	public:
		tv::types::Combined _data;
		std::vector<tv::types::TV> _searchResults;
		ApiGet* _tmdbapi;
		TV *_q;
	public:
		TVPrivate(TV *q)
		{
			_q = q;
			_tmdbapi = &ApiGetSingleton::get_mutable_instance();
			_data.zero();
			_searchResults.clear();
		}
	public:
		std::vector<tv::types::TV> search(std::wstring title, int32_t year /*= -1*/)
		{
			mtx.lock();	

			std::vector<tv::types::TV> tv;

			std::string t = boost::locale::conv::utf_to_utf<char>(title);

			QueryOption opt("query", t);
			QueryOptions opts;
			opts.push_back(opt);

			if (year > 0)
			{
				std::stringstream ySS;
				ySS << year;
				QueryOption yopt("first_air_date_year", ySS.str());
				opts.push_back(yopt);
			}

			std::string url = "/3/search/tv";

			std::string j = _tmdbapi->json(url, opts);

			uint64_t i = 0;
			try
			{
				i = searchParse(j);
			}
			catch (...)
			{
				mtx.unlock();
				return tv;
			}			

			if (i == 0 && year>0)
			{

				opts.clear();

				QueryOption opt("query", t);

				opts.push_back(opt);

				j = _tmdbapi->json(url, opts);

				i = searchParse(j);
			}
			
			if (i > 0)
			{
				try
				{
					tv = searchParseData(j);
				}
				catch (...)
				{
					mtx.unlock();
					return tv;
				}
			}

			mtx.unlock();
			return tv;
		}

		uint64_t searchParse(std::string j)
		{
			if (j.size() < 1)
			{
				return -1;
			}

			rapidjson::Document d;
			d.Parse<0>(j.c_str());
			
			if (rjcheck(d, "results"))
			{
				if (d["results"].IsArray())
				{
					rapidjson::Value &r1 = d["results"];
					for (auto &rr : r1.GetArray())
					{
						if (rjcheck(rr, "id"))
						{
							int id = rr["id"].GetInt();
							return id;
						}
					}
				}
			}
			return 0;
		}

		std::vector<tv::types::TV> searchParseData(std::string j)
		{
			std::vector<tv::types::TV> tvshows;
			
			if (j.empty())
			{
				return tvshows;
			}

			_searchResults.clear();			

			rapidjson::Document d;
			d.Parse<0>(j.c_str());
			//rapidjson::Value &r = d;
			
			if (d.HasMember("results"))
			{
				rapidjson::Value &r1 = d["results"];
				for (rapidjson::SizeType ii = 0; ii < r1.Size(); ++ii)
				{
					rapidjson::Value &r = r1[ii];
					tv::types::TV data = fromTVJson(r);
					tvshows.push_back(data);
				}
			}

			return tvshows;
		}

		std::shared_ptr<tv::types::Combined> scan(std::shared_ptr<tv::types::Combined> &data, TV::ScanTypes type = TV::TVScan)
		{
			mtx.lock();

			//data::MovieCombined data;
			if (!data->verify())
			{
				return data;
			}

			if (type == TV::TVScan || type == TV::AllScan)
			{
				tv::types::TV tv;
				tv.zero();
				if (data->Id() != 0)
				{
					tv = scanMainTVParse(data->Id());
				}
				data->tv = tv;
			}
			if (type == TV::CastScan || type == TV::AllScan)
			{
				tmdb::types::CastAndCrew ccz;
				ccz.zero();
				ccz.id = data->Id();

				tmdb::CastAndCrew cc;
				if (data->Id() != 0)
				{
					ccz = cc.get(data->Id(), tmdb::types::ScanType::TVShow);
				}
				data->crew = ccz;
			}
			if (type == TV::VideosScan || type == TV::AllScan)
			{
				tmdb::Videos v;
				data->videos = v.get(data->Id(), tmdb::types::ScanType::TVShow);
			}
			if (type == TV::KeywordsScan || type == TV::AllScan)
			{
				/*movie::Keywords k;
				data->keywords = k.get(data->id());*/
			}
			if (type == TV::AltTitlesScan || type == TV::AllScan)
			{
				//data->alt_titles = movie::AlternativesTitle::get(data->id());
			}			

			mtx.unlock();

			return data;
		}

		tv::types::TV scanMainTVParse(uint64_t id)
		{
			std::stringstream urlss;
			urlss << "/3/tv/" << id;
			
			std::string j = _tmdbapi->json(urlss.str());

			tv::types::TV tv;
			tv.zero();

			bool retry = false;

			//std::cout << "Scanning for id:" << id;

			try
			{
				rapidjson::Document d;

				d.Parse<0>(j.c_str());

				rapidjson::Value &r = d;

				tv = fromTVJson(r);
			}
			catch (...)
			{
				retry = true;
				tv.zero();
			}

			if (retry)
			{
				std::cout << "Retrying scan" << std::endl;
				boost::this_thread::sleep(boost::posix_time::milliseconds(500));
				try
				{
					std::string j = _tmdbapi->json(urlss.str());
					rapidjson::Document d;

					d.Parse<0>(j.c_str());

					rapidjson::Value &r = d;

					tv = fromTVJson(r);
				}
				catch (...)
				{
					tv.zero();
					std::cout << "Retry Failed" << std::endl;
				}
			}
			return tv;
		}


		tv::types::TV fromTVJson(rapidjson::Value &r)
		{
			tv::types::TV data;
			data.zero();
			if (rjcheck(r, "id"))
			{
				data.id = r["id"].GetInt();

			}
			
			if (rjcheck(r, "backdrop_path"))
			{
				data.backdrop_path = r["backdrop_path"].GetString();
			}
			if (rjcheck(r, "created_by"))
			{
				rapidjson::Value &rr = r["created_by"];
				for (rapidjson::SizeType i = 0; i < rr.Size(); ++i)
				{
					rapidjson::Value &rrr = rr[i];
					tmdb::types::BasicPerson person;
					
					if (rjcheck(rrr, "id"))
					{
						person.id = rrr["id"].GetUint64();
					}
					if (rjcheck(rrr, "name"))
					{
						person.name = rrr["name"].GetString();
					}
					if (rjcheck(rrr, "gender"))
					{
						person.gender = (tmdb::types::Gender)rrr["gender"].GetInt();
					}
					if (rjcheck(rrr, "profile_path"))
					{
						person.profile_path = rrr["profile_path"].GetString();
					}
					data.created_by.push_back(person);
				}
			}
			if (rjcheck(r, "episode_runtime"))
			{
				rapidjson::Value &rr = r["episode_runtime"];
				if (rr.IsArray())
				{
					rapidjson::Value::ConstValueIterator itr = rr.Begin();
					data.episode_runtime = itr->GetInt();
				}
			}
			
			if (rjcheck(r, "genres"))
			{
				rapidjson::Value &rr = r["genres"];
				for (rapidjson::SizeType i = 0; i < rr.Size(); ++i)
				{
					tmdb::types::Genre g;
					rapidjson::Value &rrr = rr[i];

					if (rjcheck(rrr, "id"))
					{
						g.id = rrr["id"].GetInt64();
					}
					if (rjcheck(rrr, "name"))
					{
						g.name = rrr["name"].GetString();
					}
					data.genres.push_back(g);
				}
			}
			if (rjcheck(r, "homepage"))
			{
				data.homepage = r["homepage"].GetString();
			}
			if (rjcheck(r, "in_production"))
			{
				data.in_production = r["in_production"].GetBool();
			}
			if (rjcheck(r, "languages"))
			{
				rapidjson::Value &rr = r["languages"];
				if (rr.IsArray())
				{
					for (auto &i : rr.GetArray())
					{
						std::string s = i.GetString();
						data.languages.push_back(s);
					}
				}
			}
			if (rjcheck(r, "last_air_date"))
			{
				data.last_air_date = r["last_air_date"].GetString();
				boost::gregorian::date d(boost::gregorian::from_string(data.last_air_date));
				boost::posix_time::ptime pt(d);
				data.last_air_date_t = boost::posix_time::to_time_t(pt);
			}
			if (rjcheck(r, "name"))
			{
				data.name = r["name"].GetString();
			}
			//! /todo networks
			if (rjcheck(r, "number_of_episodes"))
			{
				data.number_of_episodes = r["number_of_episodes"].GetUint64();
			}
			if (rjcheck(r, "number_of_seasons"))
			{
				data.number_of_seasons = r["number_of_seasons"].GetUint64();
			}
			if (rjcheck(r, "origin_country"))
			{
				if (r["origin_country"].IsArray())
				{
					rapidjson::Value &rr = r["origin_country"];
					for (auto &i : rr.GetArray())
					{
						std::string s = i.GetString();
						data.origin_country.push_back(s);
					}
				}
			}
			if (rjcheck(r, "original_language"))
			{
				data.original_language = r["original_language"].GetString();
			}
			if (rjcheck(r, "original_name"))
			{
				data.original_name = r["original_name"].GetString();
			}
			if (rjcheck(r, "overview"))
			{
				data.overview = r["overview"].GetString();
			}
			if (rjcheck(r, "popularity"))
			{
				data.popularity = r["popularity"].GetDouble();
			}
			if (rjcheck(r, "poster_path"))
			{
				data.poster_path = r["poster_path"].GetString();
			}
			if (rjcheck(r, "production_companies"))
			{
				rapidjson::Value &rr = r["production_companies"];
				for (rapidjson::SizeType i = 0; i < rr.Size(); ++i)
				{
					tmdb::types::ProductionCompany t;
					rapidjson::Value &rrr = rr[i];

					if (rjcheck(rrr, "name"))
					{
						t.name = rrr["name"].GetString();
					}
					if (rjcheck(rrr, "id"))
					{
						t.id = rrr["id"].GetInt64();
					}

					data.production_companies.push_back(t);

				}
			}
			//! /todo seasons
			if (rjcheck(r, "status"))
			{
				data.status = r["status"].GetString();
			}
			if (rjcheck(r, "type"))
			{
				data.type = r["type"].GetString();
			}
			if (rjcheck(r, "vote_average"))
			{
				data.vote_average = r["vote_average"].GetDouble();
			}
			if (rjcheck(r, "vote_count"))
			{
				data.vote_count = r["vote_count"].GetInt();
			}
			
			
			return data;
		}
	};
}//namespace tmdb


using namespace tmdb;

TV::TV()
{
	_p = new TVPrivate(this);
}

TV::~TV()
{
	if (_p)
	{
		delete _p;
	}
	_p = NULL;
}

std::vector<tv::types::TV> TV::search(std::wstring title, int32_t year /*= -1*/)
{
	return _p->search(title, year);
}

std::shared_ptr<tv::types::Combined> TV::scan(std::shared_ptr<tv::types::Combined> &data, ScanTypes type)
{
	return _p->scan(data, type);
}


