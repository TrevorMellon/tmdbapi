#ifndef TMDB_CONFIGURATION_H
#define TMDB_CONFIGURATION_H

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
		Configuration();
		~Configuration();
	public:
		std::string getImageUrl(const ImageType &type, const ImageSize &size, const std::string &partialurl, const bool usessl = false);
	private:
		ConfigurationPrivate *_p;
	};
}

#endif //TMDB_CONFIGURATION_H