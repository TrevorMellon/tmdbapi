/*****************************************************
*                                                    
*  copyright (c) 2018 Trevor Mellon                  
*  For further information contact codified@ignifi.me        
*                                                    
*  Licensed under the BSD 3-clause license           
*  See LICENCE file for more information             
*                                                    
******************************************************/

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
