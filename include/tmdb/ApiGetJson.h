/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/


#ifndef TMDB_API_GET_JSON_H_
#define TMDB_API_GET_JSON_H_

#include <tmdb/config.h>

#include <boost/serialization/singleton.hpp>

#include <string>

namespace tmdb
{
	std::string url_encode(const std::string &value);

	typedef std::pair<std::string, std::string> QueryOption;

	class ApiGetJsonPrivate;

	class ApiGetJson
	{
	public:
		ApiGetJson(bool usessl = TMDB_DEFAULT_SSL);
		~ApiGetJson();
	public:
		std::string json(std::string url = "");
		void addOption(const std::string &key, const std::string &value);
		void addOption(const QueryOption &option);
		void clearOptions();
		void setDefaultUrl(const std::string &url);
	public:
		void setSSL(const bool use);
		bool getSSL();
	private:
		ApiGetJsonPrivate *_p;
	};

	typedef boost::serialization::singleton<ApiGetJson> ApiGetJsonSingleton;

}//namespace tmdb


#endif//TMDB_API_GET_JSON__H