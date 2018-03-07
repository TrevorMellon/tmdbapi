/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/

#ifndef TMDB_TV_H
#define TMDB_TV_H

#include <string>
#include <stdint.h>
#include <vector>
#include <memory>

#include <tmdb/tv/Types.h>


namespace tmdb
{
	class TVPrivate;
	class ApiGet;

	class TV
	{
	public:
		enum ScanTypes
		{
			AllScan = 0,
			TVScan,
			CastScan,
			AltTitlesScan,
			VideosScan,
			KeywordsScan
		};
	public:
		TV();
		~TV();
	public:
		std::vector<tv::types::TV> search(::std::wstring title, int32_t year = -1);
	public:
		std::shared_ptr<tv::types::Combined> scan(std::shared_ptr<tv::types::Combined> &data, ScanTypes type = TVScan);
	private:
		TVPrivate		*_p;
	};

}//namespace tmdb

#endif //TMDB_TV_H