/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/

#include <sstream>
#include <string>

#include <rapidjson/document.h>

#include <tmdb/movie/AlternativeTitle.h>
#include <tmdb/ApiGetJson.h>

#include "tmdb/util.h"

using namespace tmdb;
using namespace rapidjson;

data::AlternativeTitles AlternativesTitle::get(const uint64_t id)
{
	ApiGetJson &api = ApiGetJsonSingleton::get_mutable_instance();

	api.clearOptions();

	QueryOption opt;

	opt.first = "movieid";
	
	std::stringstream ss;
	ss << id;
	opt.second = ss.str();

	api.addOption(opt);

	ss.str("");
	ss << "/3/movie/" << id << "/alternative_titles";

	std::string j = api.json(ss.str());

	data::AlternativeTitle alttitle;
	data::AlternativeTitles alttitles;

	Document d;
	d.Parse<0>(j.c_str());

	Value &r = d;
	
	if (rjcheck(r, "id"))
	{
		if (rjcheck(r, "titles"))
		{
			Value &ts = r["titles"];

			if (ts.IsArray())
			{
				for (auto &rr : ts.GetArray())
				{
					alttitle.zero();

					bool bset = false;
					if (rjcheck(rr, "iso_3166_1"))
					{
						alttitle.iso_3166_1 = rr["iso_3166_1"].GetString();
					}
					if (rjcheck(rr, "title"))
					{
						alttitle.title = rr["title"].GetString();						
					}
					alttitles.titles.push_back(alttitle);
				}
			}
		}
	}

	return alttitles;
}