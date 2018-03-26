/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/

#include "tmdb/common/Videos.h"

#include "tmdb/ApiGet.h"

#include <sstream>

# include <rapidjson/rapidjson.h>
# include <rapidjson/document.h>

#include <tmdb/Util.h>

using namespace tmdb;

tmdb::types::VideosList Videos::get(uint64_t movie_id, tmdb::types::ScanType scantype)
{
	ApiGet &api = ApiGetSingleton::get_mutable_instance();

	std::stringstream urlss;
	if (scantype == types::ScanType::Movie)
	{
		
		urlss << "/3/movie/" << movie_id << "/videos";
	}
	else
	{
		urlss << "/3/tv/" << movie_id << "/videos";
	}
	std::string j = api.json(urlss.str());

	types::VideosList vl;
	vl.zero();

	if (j.empty())
	{
		return vl;
	}

	types::VideosData data;

	rapidjson::Document d;
	d.Parse<0>(j.c_str());

	if (d.HasMember("results"))
	{
		if (d["results"].IsArray())
		{
			rapidjson::Value &r1 = d["results"];
			for (auto &rr : r1.GetArray())
			{
				data.zero();
				if (rjcheck(rr, "id"))
				{
					data.id = rr["id"].GetInt64();
				}
				if (rjcheck(rr, "iso_639_1"))
				{
					data.iso_639_1 = rr["iso_639_1"].GetString();
				}
				if (rjcheck(rr, "iso_3166_1"))
				{
					data.iso_3166_1 = rr["iso_3166_1"].GetString();
				}
				if (rjcheck(rr, "key"))
				{
					data.key = rr["key"].GetString();
				}
				if (rjcheck(rr, "name"))
				{
					data.name = rr["name"].GetString();
				}
				if (rjcheck(rr, "site"))
				{
					data.site = rr["site"].GetString();
				}
				if (rjcheck(rr, "size"))
				{
					data.size = rr["size"].GetInt();
				}
				if (rjcheck(rr, "type"))
				{
					data.type = rr["type"].GetString();
				}
				vl.videos.push_back(data);
			}
		}

	}
	return vl;
}
