#ifndef TMDB_IMAGE_DOWNLOADER_H
#define TMDB_IMAGE_DOWNLOADER_H

namespace tmdb
{
	class ImageDownloaderPrivate;

	class ImageDownloader
	{
	public:
		ImageDownloader();
		~ImageDownloader();
	private:
		ImageDownloaderPrivate *_p;
	};
}

#endif//TMDB_IMAGE_DOWNLOADER_H