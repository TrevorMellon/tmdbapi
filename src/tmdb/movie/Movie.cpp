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
#include <tmdb/movie/CastNCrew.h>
#include <tmdb/movie/Videos.h>
#include <tmdb/movie/Keywords.h>
#include <tmdb/movie/AlternativeTitle.h>
#include <tmdb/Util.h>

#include <sstream>

# include <rapidjson/rapidjson.h>
# include <rapidjson/document.h>



namespace tmdb
{

	class MovieKPrivate
	{
	public:
		boost::mutex mtx;
	public:
		data::MovieCombined _data;
		std::vector<data::Movie> _searchResults;
		ApiGet* _tmdbapi;
		Movie *_q;
	public:
		MovieKPrivate(Movie *q)
		{
			_q = q;
			_tmdbapi = &ApiGetSingleton::get_mutable_instance();
			_data.zero();
			_searchResults.clear();
		}
	public:
		uint64_t searchFilename(std::wstring fn)
		{
			std::cout << std::endl;
			std::cout << "Searching for filename:";
			std::wcout << fn << std::endl;
			
			boost::cmatch m;
			boost::wregex r(L"\\.");

			boost::wsregex_token_iterator rb(fn.begin(), fn.end(), r, -1);
			boost::wsregex_token_iterator re;

			//break by separator, title should be first
			std::wstring nm1 = *rb;

			++rb;

			while (rb != re)
			{
				++rb;
			}
			
			//Remove underscores
			boost::wregex r2(L"_");

			boost::wsregex_token_iterator rb2(nm1.begin(), nm1.end(), r2, -1);

			std::wstringstream ss;
			ss << *rb2;
			rb2++;
			while (rb2 != re)
			{
				ss << " ";
				ss << *rb2;
				rb2++;
			}
			
			std::wstring tstr = ss.str();
			//find year

			std::wstring::iterator yeariter = tstr.end();
			--yeariter;
			int year;
			bool found = false;
			if (tstr.size() > 5)
			{
				wchar_t y[5];
				memset(y, 0, sizeof(wchar_t) * 5);

				if (*yeariter == ')')
				{
					int ct = 1;
					--yeariter;
					while (true)
					{

						if (*yeariter == '(')
						{
							break;
						}
						if (ct > 4)
						{
							break;
						}
						if (*(yeariter - 1) == '(' && ct == 4)
						{
							found = true;
						}
						y[3 - (ct - 1)] = *yeariter;
						++ct;
						--yeariter;
					}
				}
				y[4] = '\0';
				try
				{
					year = boost::lexical_cast<int>(y);
					tstr.assign(tstr.begin(), yeariter);
				}
				catch (...)
				{
					found = false;
				}
			}

			if (found)
			{
				std::wstring title = tstr;

				std::cout << "Search: " << "t=";
				std::wcout << title << " & y=" << year;
				std::cout << std::endl;
				
				return search(title, year);
			}
			else
			{
				std::wstring title = tstr;
				
				std::cout << "Search: " << "t=";
				std::wcout << title;
				std::cout << std::endl;

				return search(title, 0);
			}
		}

		uint64_t search(std::wstring title, int32_t year /*= -1*/)
		{
			mtx.lock();			

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
				return 0;
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
					searchParseData(j);
				}
				catch (...)
				{
					mtx.unlock();
					return 0;
				}
			}

			mtx.unlock();
			return i;
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

		void searchParseData(std::string j)
		{
			if (j.empty())
			{
				return;
			}

			_searchResults.clear();

			if (j.size() < 1)
			{
				return;
			}

			rapidjson::Document d;
			d.Parse<0>(j.c_str());
			//rapidjson::Value &r = d;
			
			if (d.HasMember("results"))
			{
				rapidjson::Value &r1 = d["results"];
				for (rapidjson::SizeType ii = 0; ii < r1.Size(); ++ii)
				{
					rapidjson::Value &r = r1[ii];
					data::Movie data = fromMovieJson(r);
					_searchResults.push_back(data);
				}
			}
		}

		std::shared_ptr<data::MovieCombined> scan(std::shared_ptr<data::MovieCombined> data, Movie::ScanTypes type = Movie::MovieScan)
		{
			mtx.lock();

			//data::MovieCombined data;
			if (!data->verifyID())
			{
				return data;
			}

			if (type == Movie::MovieScan || type == Movie::AllScan)
			{
				data::Movie m;
				m.zero();
				if (data->id() != 0)
				{
					m = scanMainMovieParse(data->id());
				}
				data->movie = m;
			}
			if (type == Movie::CastScan || type == Movie::AllScan)
			{
				data::CastNCrew ccz;
				ccz.zero();
				ccz.id = data->id();

				CastNCrew cc;
				if (data->id() != 0)
				{
					ccz = cc.get(data->id());
				}
				data->crew = ccz;
			}
			if (type == Movie::VideosScan || type == Movie::AllScan)
			{
				Videos v;
				data->videos = v.get(data->id());
			}
			if (type == Movie::KeywordsScan || type == Movie::AllScan)
			{
				Keywords k;
				data->keywords = k.get(data->id());
			}
			if (type == Movie::AltTitlesScan || type == Movie::AllScan)
			{
				data->alt_titles = AlternativesTitle::get(data->id());
			}			

			mtx.unlock();

			return data;
		}

		data::Movie scanMainMovieParse(uint64_t id)
		{
			std::stringstream urlss;
			urlss << "/3/movie/" << id;
			
			std::string j = _tmdbapi->json(urlss.str());

			data::Movie m;
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


		data::Movie fromMovieJson(rapidjson::Value &r)
		{
			data::Movie data;
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
				data::MovieCollection col;
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
					data::MovieGenre g;
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
					data::MovieProductionCompany t;
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
					data::MovieProductionCountry p;
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
				boost::gregorian::date d(boost::gregorian::from_string(data.release_date));
				boost::posix_time::ptime pt(d);
				data.release_date_t = boost::posix_time::to_time_t(pt);
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
					data::MovieLanguages lang;
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
	_p = new MovieKPrivate(this);
}

Movie::~Movie()
{
	if (_p)
	{
		delete _p;
	}
	_p = NULL;
}

uint64_t Movie::search(std::wstring title, int32_t year /*= -1*/)
{
	return _p->search(title, year);
}

std::vector<data::Movie> Movie::getSearchResults()
{
	return _p->_searchResults;
}

std::shared_ptr<data::MovieCombined> Movie::scan(std::shared_ptr<data::MovieCombined> data, ScanTypes type)
{
	return _p->scan(data, type);
}

uint64_t Movie::searchFilename(std::wstring fn)
{
	return _p->searchFilename(fn);
}
