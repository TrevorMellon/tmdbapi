/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/

#include <tmdb/movie/Movie.h>

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
#include <tmdb/common/CastAndCrew.h>
#include <tmdb/common/Videos.h>
#include <tmdb/movie/Keywords.h>
#include <tmdb/movie/AlternativeTitle.h>
#include <tmdb/Util.h>
#include <tmdb/common/types/Types.h>

#include <sstream>

# include <rapidjson/rapidjson.h>
# include <rapidjson/document.h>



namespace tmdb
{

	class MoviePrivate
	{
	public:
		boost::mutex mtx;
	public:
		movie::types::Combined _data;
		std::vector<movie::types::Movie> _searchResults;
		ApiGet* _tmdbapi;
		Movie *_q;
	public:
		MoviePrivate(Movie *q)
		{
			_q = q;
			_tmdbapi = &ApiGetSingleton::get_mutable_instance();
			_data.zero();
			_searchResults.clear();
		}
	public:
		std::vector<movie::types::Movie> search(std::wstring title, int32_t year /*= -1*/)
		{
			mtx.lock();	

			std::vector<movie::types::Movie> movies;

			std::string t = boost::locale::conv::utf_to_utf<char>(title);

			QueryOption opt("query", t);
			QueryOptions opts;
			opts.push_back(opt);

			if (year > 0)
			{
				std::stringstream ySS;
				ySS << year;
				QueryOption yopt("year", ySS.str());
				opts.push_back(yopt);
			}

			std::string url = "/3/search/movie";

			std::string j = _tmdbapi->json(url, opts);

			uint64_t i = 0;
			try
			{
				i = searchParse(j);
			}
			catch (...)
			{
				mtx.unlock();
				return movies;
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
					movies = searchParseData(j);
				}
				catch (std::exception &e)
				{
					std::cerr << e.what() << std::endl;
					mtx.unlock();
					return movies;
				}
			}

			mtx.unlock();
			return movies;
		}

		uint64_t searchParse(std::string j)
		{
			if (j.size() < 1)
			{
				return -1;
			}

			rapidjson::Document d;
			d.Parse(j.c_str());

			if (!d.IsObject())
			{
#if _DEBUG
				std::cout << j << std::endl;
#endif
				return 0;
			}
			
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

		std::vector<movie::types::Movie> searchParseData(std::string j)
		{
			std::vector<movie::types::Movie> movies;
			
			if (j.empty())
			{
				return movies;
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
					movie::types::Movie data = fromMovieJson(r);
					movies.push_back(data);
				}
			}

			return movies;
		}

		std::shared_ptr<movie::types::Combined> scan(std::shared_ptr<movie::types::Combined> &data, Movie::ScanTypes type = Movie::MovieScan)
		{
			mtx.lock();

			//data::MovieCombined data;
			if (!data->verify())
			{
				return data;
			}

			if (type == Movie::MovieScan || type == Movie::AllScan)
			{
				movie::types::Movie m;
				m.zero();
				if (data->id() != 0)
				{
					m = scanMainMovieParse(data->id());
				}
				data->movie = m;
			}
			if (type == Movie::CastScan || type == Movie::AllScan)
			{
				tmdb::types::CastAndCrew ccz;
				ccz.zero();
				ccz.id = data->id();

				CastAndCrew cc;
				if (data->id() != 0)
				{
					ccz = cc.get(data->id(), tmdb::types::ScanType::Movie);
				}
				data->crew = ccz;
			}
			if (type == Movie::VideosScan || type == Movie::AllScan)
			{
				tmdb::Videos v;
				data->videos = v.get(data->id(), tmdb::types::ScanType::Movie);
			}
			if (type == Movie::KeywordsScan || type == Movie::AllScan)
			{
				movie::Keywords k;
				data->keywords = k.get(data->id());
			}
			if (type == Movie::AltTitlesScan || type == Movie::AllScan)
			{
				data->alt_titles = movie::AlternativesTitle::get(data->id());
			}			

			mtx.unlock();

			return data;
		}

		movie::types::Movie scanMainMovieParse(uint64_t id)
		{
			std::stringstream urlss;
			urlss << "/3/movie/" << id;
			
			std::string j = _tmdbapi->json(urlss.str());

			movie::types::Movie m;
			m.zero();

			bool retry = false;

			//std::cout << "Scanning for id:" << id;

			try
			{
				rapidjson::Document d;

				d.Parse<0>(j.c_str());

				rapidjson::Value &r = d;

				m = fromMovieJson(r);
			}
			catch (...)
			{
				retry = true;
				m.zero();
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

					m = fromMovieJson(r);
				}
				catch (...)
				{
					m.zero();
					std::cout << "Retry Failed" << std::endl;
				}
			}
			return m;
		}


		movie::types::Movie fromMovieJson(rapidjson::Value &r)
		{
			movie::types::Movie data;
			data.zero();
			if (rjcheck(r, "id"))
			{
				data.id = r["id"].GetInt();

			}
			if (rjcheck(r, "adult"))
			{
				bool t = r["adult"].GetBool();
				data.adult = t;
			}
			if (rjcheck(r, "backdrop_path"))
			{
				data.backdrop_path = r["backdrop_path"].GetString();
			}
			if (rjcheck(r, "belongs_to_collection"))
			{
				rapidjson::Value &rr = r["belongs_to_collection"];
				movie::types::Collection col;
				col.id = 0;
				if (rjcheck(rr, "id"))
				{
					col.id = rr["id"].GetInt();

				}
				if (rjcheck(rr, "name"))
				{
					col.name = rr["name"].GetString();
				}
				if (rjcheck(rr, "poster_path"))
				{
					col.poster_path = rr["poster_path"].GetString();
				}
				if (rjcheck(rr, "backdrop_path"))
				{
					col.backdrop_path = rr["backdrop_path"].GetString();
				}
				data.belongs_to_collection = col;
			}
			if (rjcheck(r, "budget"))
			{
				data.budget = r["budget"].GetInt64();
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
			if (rjcheck(r, "imdb_id"))
			{
				data.imdb_id = r["imdb_id"].GetString();
			}
			if (rjcheck(r, "original_language"))
			{
				data.original_language = r["original_language"].GetString();
			}
			if (rjcheck(r, "original_title"))
			{
				data.original_title = r["original_title"].GetString();
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
			if (rjcheck(r, "production_countries"))
			{
				rapidjson::Value &rr = r["production_countries"];
				for (rapidjson::SizeType i = 0; i < rr.Size(); ++i)
				{
					rapidjson::Value &rrr = rr[i];
					tmdb::types::ProductionCountry p;
					if (rjcheck(rrr, "iso_3166_1"))
					{
						p.iso = rrr["iso_3166_1"].GetString();
					}
					if (rjcheck(rrr, "name"))
					{
						p.name = rrr["name"].GetString();
					}

					data.production_countries.push_back(p);
				}

			}
			if (rjcheck(r, "release_date"))
			{
				data.release_date = r["release_date"].GetString();
				if (!data.release_date.empty())
				{
					boost::gregorian::date d(boost::gregorian::from_string(data.release_date));
					boost::posix_time::ptime pt(d);
					data.release_date_t = boost::posix_time::to_time_t(pt);
				}
			}
			if (rjcheck(r, "revenue"))
			{
				data.revenue = r["revenue"].GetInt64();
			}
			if (rjcheck(r, "runtime"))
			{
				data.runtime = r["runtime"].GetInt();
			}
			if (rjcheck(r, "spoken_languages"))
			{
				rapidjson::Value &rr = r["spoken_languages"];
				for (rapidjson::SizeType i = 0; i < rr.Size(); ++i)
				{
					rapidjson::Value &rrr = rr[i];
					tmdb::types::Languages lang;
					if (rjcheck(rrr, "iso_639_1"))
					{
						lang.iso = rrr["iso_639_1"].GetString();
					}
					if (rjcheck(rrr, "name"))
					{
						lang.name = rrr["name"].GetString();
					}
					data.spoken_languages.push_back(lang);
				}
			}

			if (rjcheck(r, "original_title"))
			{
				data.original_title = r["original_title"].GetString();

			}
			if (rjcheck(r, "status"))
			{
				data.status = r["status"].GetString();
			}
			if (rjcheck(r, "tagline"))
			{
				data.tagline = r["tagline"].GetString();
			}
			if (rjcheck(r, "title"))
			{
				data.title = r["title"].GetString();
			}
			if (rjcheck(r, "video"))
			{
				data.video = r["video"].GetBool();
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

Movie::Movie()
{
	_p = new MoviePrivate(this);
}

Movie::~Movie()
{
	if (_p)
	{
		delete _p;
	}
	_p = NULL;
}

std::vector<movie::types::Movie> Movie::search(std::wstring title, int32_t year /*= -1*/)
{
	return _p->search(title, year);
}

std::shared_ptr<movie::types::Combined> Movie::scan(std::shared_ptr<movie::types::Combined> &data, ScanTypes type)
{
	return _p->scan(data, type);
}


