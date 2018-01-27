
#include <tmdb/ImageDownloader.h>
#include <tmdb/Iso.h>
#include <tmdb/Configuration.h>
#include <tmdb/config.h>
#include <tmdb/ApiGet.h>

#include <stdint.h>

namespace tmdb
{
	class ImageDownloaderPrivate
	{
	public:
		ImageDownloaderPrivate()
		{
			data = nullptr;
		}
	public:
		void Download(ImageType &type, ImageSize &size, std::string partialurl)
		{
			std::string url;
			Configuration &conf = ConfigurationSingleton::get_mutable_instance();

			url = conf.getImageUrl(type, size, partialurl);

			if (data)
			{
				delete data;
			}
			data = nullptr;

		}

		void getImage(std::string url)
		{

		}
	public:
		uint8_t *data;
	};
}

using namespace tmdb;

ImageDownloader::ImageDownloader()
{
	_p = new ImageDownloaderPrivate();
}

ImageDownloader::~ImageDownloader()
{
	if (_p)
	{
		delete _p;
	}
	_p = nullptr;
}
