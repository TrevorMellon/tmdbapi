
#include <tmdb/config.h>
#include <tmdb/configuration.h>
#include <tmdb/ApiGet.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <tmdb/util.h>

namespace rj = rapidjson;

namespace tmdb
{
	class ConfigurationPrivate
	{
	public:
		ConfigurationPrivate(const bool usessl = TMDB_DEFAULT_SSL)
		{
			ApiGet &api = ApiGetSingleton::get_mutable_instance();			
			std::string j = api.json("/3/configuration");
			parse(j);
		}

		void parse(std::string j)
		{
			rj::Document doc;
			doc.Parse<0>(j.c_str());

			if (doc.HasMember("images"))
			{

				rj::Value &dd = doc["images"];

				if (rjcheck(dd, "base_url"))
				{
					baseurl = dd["base_url"].GetString();
				}
				if (rjcheck(dd, "secure_base_url"))
				{
					secure_baseurl = dd["secure_base_url"].GetString();
				}
				if (rjcheck(dd, "backdrop_sizes"))
				{
					_backdropMap.clear();

					rj::Value &t = dd["backdrop_sizes"];

					if (t.IsArray())
					{
						for (auto &n : t.GetArray())
						{
							std::string val = n.GetString();

							if (val == "w300")
							{
								_backdropMap.insert(std::pair<uint32_t, std::string>(ImageTiny, val));
								_backdropMap.insert(std::pair<uint32_t, std::string>(ImageSmall, val));
							}
							else if (val == "w780")
							{
								_backdropMap.insert(std::pair<uint32_t, std::string>(ImageMedium, val));
							}
							else if (val == "w1280")
							{
								_backdropMap.insert(std::pair<uint32_t, std::string>(ImageLarge, val));
								_backdropMap.insert(std::pair<uint32_t, std::string>(ImageXLarge, val));
								_backdropMap.insert(std::pair<uint32_t, std::string>(ImageHuge, val));
							}
							else if (val == "original")
							{
								_backdropMap.insert(std::pair<uint32_t, std::string>(ImageOriginal, val));
							}
						}
					}
				}
				if (rjcheck(dd, "logo_sizes"))
				{
					_logoMap.clear();

					rj::Value &t = dd["logo_sizes"];

					if (t.IsArray())
					{
						for (auto &n : t.GetArray())
						{
							std::string val = n.GetString();

							if (val == "w45")
							{
								_logoMap.insert(std::pair<uint32_t, std::string>(ImageTiny, val));
							}
							else if (val == "w92")
							{
								_logoMap.insert(std::pair<uint32_t, std::string>(ImageSmall, val));
							}
							else if (val == "w154")
							{
								_logoMap.insert(std::pair<uint32_t, std::string>(ImageMedium, val));
							}
							else if (val == "w185")
							{
								_logoMap.insert(std::pair<uint32_t, std::string>(ImageLarge, val));
							}
							else if (val == "w300")
							{
								_logoMap.insert(std::pair<uint32_t, std::string>(ImageXLarge, val));
							}
							else if (val == "w500")
							{
								_logoMap.insert(std::pair<uint32_t, std::string>(ImageHuge, val));
							}
							else if (val == "original")
							{
								_logoMap.insert(std::pair<uint32_t, std::string>(ImageOriginal, val));
							}
						}
					}
				}

				if (rjcheck(dd, "poster_sizes"))
				{
					_posterMap.clear();

					rj::Value &t = dd["poster_sizes"];

					if (t.IsArray())
					{
						for (auto &n : t.GetArray())
						{
							std::string val = n.GetString();


							if (val == "w92")
							{
								_posterMap.insert(std::pair<uint32_t, std::string>(ImageTiny, val));
							}
							else if (val == "w154")
							{
								_posterMap.insert(std::pair<uint32_t, std::string>(ImageSmall, val));
							}
							else if (val == "w185")
							{
								_posterMap.insert(std::pair<uint32_t, std::string>(ImageMedium, val));
							}
							else if (val == "w300")
							{
								_posterMap.insert(std::pair<uint32_t, std::string>(ImageLarge, val));
							}
							else if (val == "w500")
							{
								_posterMap.insert(std::pair<uint32_t, std::string>(ImageXLarge, val));
							}
							else if (val == "w780")
							{
								_posterMap.insert(std::pair<uint32_t, std::string>(ImageHuge, val));
							}
							else if (val == "original")
							{
								_posterMap.insert(std::pair<uint32_t, std::string>(ImageOriginal, val));
							}
						}
					}
				}

				if (rjcheck(dd, "profile_sizes"))
				{
					_profileMap.clear();

					rj::Value &t = dd["profile_sizes"];

					if (t.IsArray())
					{
						for (auto &n : t.GetArray())
						{
							std::string val = n.GetString();


							if (val == "w45")
							{
								_profileMap.insert(std::pair<uint32_t, std::string>(ImageTiny, val));
								_profileMap.insert(std::pair<uint32_t, std::string>(ImageSmall, val));
							}
							else if (val == "w185")
							{
								_profileMap.insert(std::pair<uint32_t, std::string>(ImageMedium, val));
							}
							else if (val == "h632")
							{
								_profileMap.insert(std::pair<uint32_t, std::string>(ImageLarge, val));
								_profileMap.insert(std::pair<uint32_t, std::string>(ImageXLarge, val));
								_profileMap.insert(std::pair<uint32_t, std::string>(ImageHuge, val));
							}
							else if (val == "original")
							{
								_profileMap.insert(std::pair<uint32_t, std::string>(ImageOriginal, val));
							}
						}
					}
				}

				if (rjcheck(dd, "still_sizes"))
				{
					_profileMap.clear();

					rj::Value &t = dd["still_sizes"];

					if (t.IsArray())
					{
						for (auto &n : t.GetArray())
						{
							std::string val = n.GetString();


							if (val == "w45")
							{
								_profileMap.insert(std::pair<uint32_t, std::string>(ImageTiny, val));
								_profileMap.insert(std::pair<uint32_t, std::string>(ImageSmall, val));
							}
							else if (val == "w185")
							{
								_profileMap.insert(std::pair<uint32_t, std::string>(ImageMedium, val));
							}
							else if (val == "h632")
							{
								_profileMap.insert(std::pair<uint32_t, std::string>(ImageLarge, val));
								_profileMap.insert(std::pair<uint32_t, std::string>(ImageXLarge, val));
								_profileMap.insert(std::pair<uint32_t, std::string>(ImageHuge, val));
							}
							else if (val == "original")
							{
								_profileMap.insert(std::pair<uint32_t, std::string>(ImageOriginal, val));
							}
						}
					}
				}
			}
		}

		std::string getImageUrl(const ImageType &type, const ImageSize &size, const std::string url, bool usessl = false)
		{
			std::map<uint32_t, std::string> *map = nullptr;

			if (type == ImageTypeBackdrop)
			{
				map = &_backdropMap;
			}
			else if (type == ImageTypeLogo)
			{
				map = &_logoMap;
			}
			else if (type == ImageTypePoster)
			{
				map = &_posterMap;
			}
			else if (type == ImageTypeProfile)
			{
				map = &_profileMap;
			}
			else if (type == ImageTypeStill)
			{
				map = &_stillMap;
			}
			else
			{
				return "";
			}

			std::string sz = map->find(size)->second;

			std::stringstream ss;
			if (!usessl)
			{
				ss << baseurl << sz << url;
			}
			else
			{
				ss << secure_baseurl << sz << url;
			}

			return ss.str();
		}
	public:
		std::string baseurl;
		std::string secure_baseurl;
		std::map<uint32_t, std::string> _backdropMap;
		std::map<uint32_t, std::string> _logoMap;
		std::map<uint32_t, std::string> _posterMap;
		std::map<uint32_t, std::string> _profileMap;
		std::map<uint32_t, std::string> _stillMap;
	};
}


using namespace tmdb;

Configuration::Configuration(const bool usessl)
{
	_p = new ConfigurationPrivate(usessl);
}

Configuration::~Configuration()
{
	delete _p;
	_p = nullptr;
}

std::string Configuration::getImageUrl(const ImageType &type, const ImageSize &size, const std::string &partialurl)
{
	return _p->getImageUrl(type, size, partialurl, false);
}

std::string Configuration::getImageUrlSecure(const ImageType &type, const ImageSize &size, const std::string &partialurl)
{
	return _p->getImageUrl(type, size, partialurl, true);
}