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

#include "tmdb/util.h"

#include <vector>
#include <sstream>

using namespace tmdb;


namespace tmdb
{
	class KeywordsPrivate
	{
	public:
		KeywordsPrivate(Keywords *parent)
		{
			_q = parent;
		}
	public:
		data::KeywordList get(uint64_t key)
		{
			data::KeywordList kl;
			ApiGet &api = ApiGetSingleton::get_mutable_instance();

			std::stringstream ssurl;

			ssurl << "/3/movie/" << key << "/keywords";

			std::string j = api.json(ssurl.str());

			return parse(j);
		}

		data::KeywordList parse(std::string j)
		{
			data::KeywordList kl;

			rapidjson::Document d;
			d.Parse<0>(j.c_str());

			if (rjcheck(d, "keywords"))
			{
				rapidjson::Value &r = d["keywords"];

				if (r.IsArray())
				{
					for (rapidjson::Value &rr : r.GetArray())
					{
						data::KeywordPair kp;
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
	public:
		std::vector<data::KeywordPair> _keywordList;
		Keywords *_q;
	};
}

Keywords::Keywords()
{
	_p = new KeywordsPrivate(this);
}

Keywords::~Keywords()
{
	delete _p;
	_p = nullptr;
}

data::KeywordList Keywords::get(uint64_t id)
{
	data::KeywordList kl;

	kl = _p->get(id);

	return kl;
}
