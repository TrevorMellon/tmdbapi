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
#include "tmdb/ApiGet.h"
#include "tmdb/Util.h"

#include <sstream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

using namespace tmdb;

std::vector<movie::types::Cast> castparse(rapidjson::Value &r)
{
	std::vector<movie::types::Cast> ret;

	if (r.IsArray())
	{
		for (auto &rr : r.GetArray())
		{
			movie::types::Cast cz;
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

std::vector<movie::types::Crew> crewparse(rapidjson::Value &r)
{
	std::vector<movie::types::Crew> ret;

	if (r.IsArray())
	{
		for (auto &rr : r.GetArray())
		{
			movie::types::Crew cz;
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

movie::types::CastNCrew castncrewparse(std::string j, uint64_t movieid)
{
	rapidjson::Document d;
	d.Parse<0>(j.c_str());

	rapidjson::Value &r = d;
	movie::types::CastNCrew ccz;
	ccz.zero();
	ccz.id = movieid;

	if (rjcheck(r, "id"))
	{
		if (rjcheck(r, "crew"))
		{
			rapidjson::Value &crewr = d["crew"];
			std::vector<movie::types::Crew> cwz = crewparse(crewr);
			ccz.crew.assign(cwz.begin(), cwz.end());
		}
		if (rjcheck(r, "cast"))
		{
			rapidjson::Value &castr = d["cast"];
			std::vector<movie::types::Cast> ctz = castparse(castr);
			ccz.cast.assign(ctz.begin(), ctz.end());
		}
	}
	return ccz;
}

movie::types::CastNCrew movie::CastNCrew::get(uint64_t movieid)
{
	std::stringstream ss;

	ss << "/3/movie/" << movieid << "/credits";

	ApiGet &api = ApiGetSingleton::get_mutable_instance();
	
	std::string j = api.json(ss.str());

	movie::types::CastNCrew ccz = castncrewparse(j, movieid);

	return ccz;
}


