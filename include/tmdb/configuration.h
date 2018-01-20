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
	private:
		ConfigurationPrivate *_p;
	};
}

#endif //TMDB_CONFIGURATION_H