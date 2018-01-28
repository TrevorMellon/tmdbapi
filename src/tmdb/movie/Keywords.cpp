/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/

#include "tmdb/movie/Keywords.h"
#include "tmdb/ApiGet.h"

# include <rapidjson/rapidjson.h>
# include <rapidjson/document.h>

#include "tmdb/Util.h"

#include <vector>
#include <sstream>

using namespace tmdb;

movie::types::KeywordList movie::Keywords::get(uint64_t id)
{
	movie::types::KeywordList kl;
	ApiGet &api = ApiGetSingleton::get_mutable_instance();

	std::stringstream ssurl;

	ssurl << "/3/movie/" << id << "/keywords";

	std::string j = api.json(ssurl.str());

	rapidjson::Document d;
	d.Parse<0>(j.c_str());

	if (rjcheck(d, "keywords"))
	{
		rapidjson::Value &r = d["keywords"];

		if (r.IsArray())
		{
			for (rapidjson::Value &rr : r.GetArray())
			{
				movie::types::KeywordPair kp;
				if (rjcheck(rr, "id"))
				{
					kp.first = rr["id"].GetInt();
				}

				if (rjcheck(rr, "name"))
				{
					kp.second = rr["name"].GetString();
				}
				kl.push_back(kp);
			}
		}
	}

	return kl;
}
