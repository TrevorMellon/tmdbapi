
#include <tmdb/configuration.h>
#include <tmdb/ApiGetJson.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <tmdb/util.h>

namespace rj = rapidjson;

namespace tmdb
{
	class ConfigurationPrivate
	{
	public:
		ConfigurationPrivate()
		{
			ApiGetJson &api = ApiGetJsonSingleton::get_mutable_instance();
			api.clearOptions();
			std::string j = api.json("/3/configuration");
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
								_backdropMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageTiny, val));
								_backdropMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageSmall, val));
							}
							else if (val == "w780")
							{
								_backdropMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageMedium, val));
							}
							else if (val == "w1280")
							{
								_backdropMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageLarge, val));
								_backdropMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageXLarge, val));
								_backdropMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageHuge, val));
							}
							else if (val == "original")
							{
								_backdropMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageOriginal, val));
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
								_logoMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageTiny, val));
							}
							else if (val == "w92")
							{
								_logoMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageSmall, val));
							}
							else if (val == "w154")
							{
								_logoMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageMedium, val));
							}
							else if (val == "w185")
							{
								_logoMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageLarge, val));
							}
							else if (val == "w300")
							{
								_logoMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageXLarge, val));
							}
							else if (val == "w500")
							{
								_logoMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageHuge, val));
							}
							else if (val == "original")
							{
								_logoMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageOriginal, val));
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
								_posterMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageTiny, val));
							}
							else if (val == "w154")
							{
								_posterMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageSmall, val));
							}
							else if (val == "w185")
							{
								_posterMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageMedium, val));
							}
							else if (val == "w300")
							{
								_posterMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageLarge, val));
							}
							else if (val == "w500")
							{
								_posterMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageXLarge, val));
							}
							else if (val == "w780")
							{
								_posterMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageHuge, val));
							}
							else if (val == "original")
							{
								_posterMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageOriginal, val));
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
								_profileMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageTiny, val));
								_profileMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageSmall, val));
							}
							else if (val == "w185")
							{
								_profileMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageMedium, val));
							}
							else if (val == "h632")
							{
								_profileMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageLarge, val));
								_profileMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageXLarge, val));
								_profileMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageHuge, val));
							}
							else if (val == "original")
							{
								_profileMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageOriginal, val));
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
								_profileMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageTiny, val));
								_profileMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageSmall, val));
							}
							else if (val == "w185")
							{
								_profileMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageMedium, val));
							}
							else if (val == "h632")
							{
								_profileMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageLarge, val));
								_profileMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageXLarge, val));
								_profileMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageHuge, val));
							}
							else if (val == "original")
							{
								_profileMap.insert(std::pair<uint32_t, std::string>(Configuration::ImageOriginal, val));
							}
						}
					}
				}
			}
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

Configuration::Configuration()
{
	_p = new ConfigurationPrivate();
}

Configuration::~Configuration()
{
	delete _p;
	_p = nullptr;
}