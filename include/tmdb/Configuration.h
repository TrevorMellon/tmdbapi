#ifndef TMDB_CONFIGURATION_H
#define TMDB_CONFIGURATION_H

#include <tmdb/config.h>

#include <string>

#include <boost/serialization/singleton.hpp>

namespace tmdb
{
	enum ImageSize
	{
		ImageTiny = 1,
		ImageSmall,
		ImageMedium,
		ImageLarge,
		ImageXLarge,
		ImageHuge,
		ImageOriginal,
	};

	enum ImageType
	{
		ImageTypeBackdrop = 1,
		ImageTypeLogo,
		ImageTypePoster,
		ImageTypeProfile,
		ImageTypeStill,
	};

	class ConfigurationPrivate;

	class Configuration
	{
	public:
		
	public:
		Configuration(const bool usessl = TMDB_DEFAULT_SSL);
		~Configuration();
	public:
		std::string getImageUrl(const ImageType &type, const ImageSize &size, const std::string &partialurl);
		std::string getImageUrlSecure(const ImageType &type, const ImageSize &size, const std::string &partialurl);
	private:
		ConfigurationPrivate *_p;
	};

	typedef boost::serialization::singleton<Configuration> ConfigurationSingleton;
}


#endif //TMDB_CONFIGURATION_H