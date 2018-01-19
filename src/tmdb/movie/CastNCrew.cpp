/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/

#include "tmdb/movie/CastNCrew.h"
#include "tmdb/ApiGetJson.h"
#include "tmdb/util.h"

#include <sstream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

using namespace tmdb;

std::vector<data::Cast> castparse(rapidjson::Value &r)
{
	std::vector<data::Cast> ret;

	if (r.IsArray())
	{
		for (auto &rr : r.GetArray())
		{
			data::Cast cz;
			if (rjcheck(rr, "id"))
			{
				cz.id = rr["id"].GetInt64();
			}
			if (rjcheck(rr, "cast_id"))
			{
				cz.cast_id = rr["cast_id"].GetInt64();
			}
			if (rjcheck(rr, "credit_id"))
			{
				cz.credit_id = rr["credit_id"].GetString();
			}
			if (rjcheck(rr, "name"))
			{
				cz.name = rr["name"].GetString();
			}
			if (rjcheck(rr, "character"))
			{
				cz.character = rr["character"].GetString();
			}
			if (rjcheck(rr, "order"))
			{
				cz.order = rr["order"].GetInt();
			}
			if (rjcheck(rr, "profile_path"))
			{
				cz.profile_path = rr["profile_path"].GetString();
			}
			ret.push_back(cz);
		}
	}
	return ret;
}

std::vector<data::Crew> crewparse(rapidjson::Value &r)
{
	std::vector<data::Crew> ret;

	if (r.IsArray())
	{
		for (auto &rr : r.GetArray())
		{
			data::Crew cz;
			cz.zero();
			if (rjcheck(rr, "id"))
			{
				cz.id = rr["id"].GetInt64();
			}
			if (rjcheck(rr, "credit_id"))
			{
				cz.credit_id = rr["credit_id"].GetString();
			}
			if (rjcheck(rr, "name"))
			{
				cz.name = rr["name"].GetString();
			}
			if (rjcheck(rr, "job"))
			{
				cz.job = rr["job"].GetString();
			}
			if (rjcheck(rr, "profile_path"))
			{
				cz.profile_path = rr["profile_path"].GetString();
			}
			if (rjcheck(rr, "department"))
			{
				cz.department = rr["department"].GetString();
			}
			ret.push_back(cz);
		}
	}
	return ret;
}

data::CastNCrew castncrewparse(std::string j, uint64_t movieid)
{
	rapidjson::Document d;
	d.Parse<0>(j.c_str());

	rapidjson::Value &r = d;
	data::CastNCrew ccz;
	ccz.zero();
	ccz.id = movieid;

	if (rjcheck(r, "id"))
	{
		if (rjcheck(r, "crew"))
		{
			rapidjson::Value &crewr = d["crew"];
			std::vector<data::Crew> cwz = crewparse(crewr);
			ccz.crew.assign(cwz.begin(), cwz.end());
		}
		if (rjcheck(r, "cast"))
		{
			rapidjson::Value &castr = d["cast"];
			std::vector<data::Cast> ctz = castparse(castr);
			ccz.cast.assign(ctz.begin(), ctz.end());
		}
	}
	return ccz;
}

data::CastNCrew CastNCrew::get(uint64_t movieid)
{
	std::stringstream ss;

	ss << "/3/movie/" << movieid << "/credits";

	ApiGetJson &api = ApiGetJsonSingleton::get_mutable_instance();
	api.clearOptions();
	
	std::string j = api.json(ss.str());

	data::CastNCrew ccz = castncrewparse(j, movieid);

	return ccz;
}


