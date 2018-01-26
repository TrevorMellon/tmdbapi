/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/


#ifndef TMDB_API_GET_H
#define TMDB_API_GET_H

#include <tmdb/config.h>

#include <boost/serialization/singleton.hpp>

#include <string>
#include <memory>

namespace tmdb
{
	std::string url_encode(const std::string &value);

	typedef std::pair<std::string, std::string> QueryOption;

	class ApiGetPrivate;	

	class ApiGet
	{
	public:
		ApiGet(bool usessl = TMDB_DEFAULT_SSL);
		~ApiGet();
	public:
		std::string json(std::string partialurl = "");
		std::shared_ptr<uint8_t> getImage(std::string url, size_t &sz);
		void saveImage(std::string url, std::string filename);
		void addOption(const std::string &key, const std::string &value);
		void addOption(const QueryOption &option);
		void clearOptions();
		void setDefaultUrl(const std::string &url);
	public:
		void setSSL(const bool use);
		bool getSSL();
	private:
		ApiGetPrivate *_p;
	};

	typedef boost::serialization::singleton<ApiGet> ApiGetSingleton;

}//namespace tmdb


#endif//TMDB_API_GET_JSON_H