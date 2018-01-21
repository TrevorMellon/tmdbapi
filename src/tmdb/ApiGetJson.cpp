/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/

#include <tmdb/ApiGetJson.h>
#include <tmdb/config.h>

#include <vector>
#include <boost/scoped_ptr.hpp>
#include <boost/asio.hpp>

#if TMDB_USE_OPENSSL
# include <boost/asio/ssl/context.hpp>
# include <boost/asio/ssl/stream.hpp>
# include <boost/asio/ssl/rfc2818_verification.hpp>
#endif

#include <boost/locale.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>

#if TMDB_USE_CURL
# include <curl/curl.h>
#endif

namespace a = boost::asio;
namespace t = boost::asio::ip;

using namespace tmdb;

namespace tmdb
{

	std::string url_encode(const std::wstring &value, bool fixquery = false)
	{
#ifdef TMDB_USE_CURL
		CURL *h = curl_easy_init();

		std::string in = boost::locale::conv::utf_to_utf<char>(value);

		std::string s = curl_easy_escape(h, in.c_str(), in.size());

		curl_easy_cleanup(h);

		return s;
#else
		// Following code modified from:
		// https://stackoverflow.com/questions/154536/encode-decode-urls-in-c
		
		std::ostringstream escaped;
		escaped.fill('0');
		escaped << std::hex;

		std::string val = boost::locale::conv::utf_to_utf<char>(value);

		for (std::string::const_iterator i = val.begin(), n = val.end(); i != n; ++i)
		{
			char c = (*i);
			boost::locale::generator gen;
			std::locale loc = gen("UTF-8");

			bool isanum = false;

			try
			{
				isanum = std::isalnum(c, loc);
			}
			catch (...)
			{
				isanum = false;
			}

			if (isanum || c == '-' || c == '_' || c == '.' || c == '~')
			{
				escaped << c;
			}
			else if (c == ' ')
			{
				escaped << "%20";
			}
			else
			{
				escaped << std::uppercase;
				escaped << '%' << std::setw(2) << int((unsigned char)c);
				escaped << std::nouppercase;
			}
		}

		return escaped.str();
#endif
	}

	class ApiGetJsonPrivate
	{
	public:
		ApiGetJsonPrivate(ApiGetJson *q, bool usessl = TMDB_DEFAULT_SSL)
		{
			_q = q;
			_usessl = usessl;
		}
	public:
		std::string json(const std::string &url);
		std::string jsonhttp(const std::string &url);
		std::string jsonhttps(const std::string &url);
	public:
		std::string _url;
		std::vector<QueryOption> _options;
		ApiGetJson *_q;
		static boost::posix_time::ptime _tp;
		bool _usessl;
	};
	boost::posix_time::ptime ApiGetJsonPrivate::_tp = boost::posix_time::microsec_clock::local_time();


	std::string ApiGetJsonPrivate::json(const std::string &url)
	{
#if TMDB_USE_OPENSSL
		if (_usessl)
		{
			return jsonhttps(url);
		}
#endif
		return jsonhttp(url);
	}


	std::string ApiGetJsonPrivate::jsonhttp(const std::string &url)
	{
		boost::system::error_code ec;

		if (ec.value() != boost::system::errc::success)
		{
			std::cout << ec.message() << std::endl;
		}

		boost::posix_time::ptime start, stop;
		boost::posix_time::time_duration diff;

		boost::posix_time::seconds secs(10);
		secs /= 30;

		start = boost::posix_time::microsec_clock::local_time();

		diff = start - _tp;

		while (diff < secs)
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(5));
			start = boost::posix_time::microsec_clock::local_time();
			diff = start - _tp;
		}

		_tp = boost::posix_time::microsec_clock::local_time();

		boost::asio::io_service service;

		t::tcp::resolver res(service);
		t::tcp::resolver::query q("api.themoviedb.org", "http");
		t::tcp::resolver::iterator endpointiter = res.resolve(q);
		
		t::tcp::socket s(service);

		boost::asio::connect(s.lowest_layer(), endpointiter, ec);

		if (ec.value() != boost::system::errc::success)
		{
			std::cout << ec.message() << std::endl;
			exit(1);
		}
		s.lowest_layer().set_option(t::tcp::no_delay(true));

		std::stringstream getSS;
		if (url != "")
		{
			getSS << url;
		}
		else
		{
			getSS << _url;
		}
		getSS << "?api_key=" << TMDB_API_KEY;


		std::vector<QueryOption>::iterator iter;
		for (iter = _options.begin(); iter != _options.end(); ++iter)
		{
			std::string key = iter->first;
			std::string value = iter->second;
			std::wstring v2 = boost::locale::conv::to_utf<wchar_t>(value, "UTF-8");
			std::stringstream optSS;
			optSS << "&" << key;
			optSS << "=" << url_encode(v2);

			getSS << optSS.str();
		}

		std::string getHeader = getSS.str();

		a::streambuf req;
		std::ostream reqStrm(&req);
		reqStrm << "GET " << getHeader << " HTTP/1.0\r\n";
		reqStrm << "Host: " << "api.themoviedb.org" << "\r\n";
		reqStrm << "Accept: " << "application/json" << "\r\n";
		reqStrm << "Connection: close" << "\r\n\r\n";

		a::write(s, req);

		a::streambuf resp;

		a::read_until(s, resp, "\r\n");

		std::istream respStrm(&resp);
		size_t headersz = resp.size();
		std::string httpVersion;
		respStrm >> httpVersion;
		unsigned int statusCode;
		respStrm >> statusCode;
		std::string statusMessage;
		std::getline(respStrm, statusMessage);

		if (!respStrm || httpVersion.substr(0, 5) != "HTTP/")
		{
			return "";
		}

		if (statusCode != 200)
		{
			return "";
		}

		a::read_until(s, resp, "\r\n\r\n");

		std::string header;
		std::getline(respStrm, header);
		size_t contentlength = 0;
		while (header != "\r")
		{
#ifdef _DEBUG
			//std::cout << header << std::endl;
#endif
			std::string key;
			std::string value;

			std::vector<std::string> vcs;

			boost::split(vcs, header, boost::is_any_of(":"));
			if (vcs.size() == 2)
			{
				key = vcs.at(0);
				value = vcs.at(1);

				if (key == "Content-Length")
				{
					boost::trim(value);
					contentlength = boost::lexical_cast<int>(value);
				}
			}
			std::getline(respStrm, header);
		}


		boost::locale::generator g;
		std::locale loc = g.generate("en_US.UTF-8");

		std::stringstream bodyss;
		bodyss.imbue(loc);
		headersz = resp.size();
		bodyss << &resp;

		try
		{
			a::read(s, resp, a::transfer_exactly(contentlength - headersz));
		}
		catch (boost::exception &e)
		{
			std::cout << boost::diagnostic_information(e) << std::endl;
		}

		bodyss << &resp;


#ifdef _DEBUG
		std::cout << bodyss.str() << std::endl;
#endif
		return bodyss.str();
		//return bodySS.str();
	}


	std::string ApiGetJsonPrivate::jsonhttps(const std::string &url)
	{
#if TMDB_USE_OPENSSL
		boost::system::error_code ec;

		boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23_client);
		ctx.set_verify_mode(boost::asio::ssl::verify_peer);
		//ctx.set_default_verify_paths(ec);
		ctx.use_certificate_chain_file("../etc/tmdb.pem");
		ctx.load_verify_file("../etc/tmdb.pem");

		if (ec.value() != boost::system::errc::success)
		{
			std::cout << ec.message() << std::endl;
		}

		boost::posix_time::ptime start, stop;
		boost::posix_time::time_duration diff;

		boost::posix_time::seconds secs(10);
		secs /= 30;

		start = boost::posix_time::microsec_clock::local_time();

		diff = start - _tp;

		while (diff < secs)
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(5));
			start = boost::posix_time::microsec_clock::local_time();
			diff = start - _tp;
		}

		_tp = boost::posix_time::microsec_clock::local_time();

		boost::asio::io_service service;

		t::tcp::resolver res(service);
		t::tcp::resolver::query q("api.themoviedb.org", "https");
		t::tcp::resolver::iterator endpointiter = res.resolve(q);

		boost::asio::ssl::stream<t::tcp::socket> s(service, ctx);
		//t::tcp::socket s(service);



		boost::asio::connect(s.lowest_layer(), endpointiter, ec);

		if (ec.value() != boost::system::errc::success)
		{
			std::cout << ec.message() << std::endl;
			exit(1);
		}
		s.lowest_layer().set_option(t::tcp::no_delay(true));

		s.set_verify_mode(boost::asio::ssl::verify_peer);
		s.set_verify_callback(boost::asio::ssl::rfc2818_verification("api.themoviedb.org"));
		
		try
		{
			s.handshake(boost::asio::ssl::stream<t::tcp::socket>::client);
		}
		catch (boost::exception &e)
		{
			std::cout << boost::diagnostic_information(e);
		}
		
		std::stringstream getSS;
		if (url != "")
		{
			getSS << url;
		}
		else
		{
			getSS << _url;
		}
		getSS << "?api_key=" << TMDB_API_KEY;


		std::vector<QueryOption>::iterator iter;
		for (iter = _options.begin(); iter != _options.end(); ++iter)
		{
			std::string key = iter->first;
			std::string value = iter->second;

			std::wstring v2 = boost::locale::conv::to_utf<wchar_t>(value, "UTF-8");

			std::stringstream optSS;
			optSS << "&" << key;
			optSS << "=" << url_encode(v2);

			getSS << optSS.str();
		}

		std::string getHeader = getSS.str();

		a::streambuf req;
		std::ostream reqStrm(&req);
		reqStrm << "GET " << getHeader << " HTTP/1.0\r\n";
		reqStrm << "Host: " << "api.themoviedb.org" << "\r\n";
		reqStrm << "Accept: " << "application/json" << "\r\n";
		reqStrm << "Connection: close" << "\r\n\r\n";

		a::write(s, req);

		a::streambuf resp;

		a::read_until(s, resp, "\r\n");

		std::istream respStrm(&resp);
		size_t headersz = resp.size();
		std::string httpVersion;
		respStrm >> httpVersion;
		unsigned int statusCode;
		respStrm >> statusCode;
		std::string statusMessage;
		std::getline(respStrm, statusMessage);

		if (!respStrm || httpVersion.substr(0, 5) != "HTTP/")
		{
			return "";
		}

		if (statusCode != 200)
		{
			return "";
		}

		a::read_until(s, resp, "\r\n\r\n");

		std::string header;
		std::getline(respStrm, header);
		size_t contentlength = 0;
		while (header != "\r")
		{
#ifdef _DEBUG
			//std::cout << header << std::endl;
#endif
			std::string key;
			std::string value;

			std::vector<std::string> vcs;

			boost::split(vcs, header, boost::is_any_of(":"));
			if (vcs.size() == 2)
			{
				key = vcs.at(0);
				value = vcs.at(1);

				if (key == "Content-Length")
				{
					boost::trim(value);
					contentlength = boost::lexical_cast<int>(value);

					//a::read(s,resp,l);
				}
			}
			std::getline(respStrm, header);
		}


		boost::locale::generator g;
		std::locale loc = g.generate("en_US.UTF-8");

		std::stringstream bodyss;
		bodyss.imbue(loc);
		headersz = resp.size();
		bodyss << &resp;

		try
		{
			a::read(s, resp, a::transfer_exactly(contentlength - headersz));
		}
		catch (boost::exception &e)
		{
			std::cout << boost::diagnostic_information(e) << std::endl;
		}

		bodyss << &resp;


#ifdef _DEBUG
		std::cout << bodyss.str() << std::endl;
#endif
		return bodyss.str();
		//return bodySS.str();
#else
		return jsonhttp(url);
#endif//TMDB_USE_OPENSSL
	}
}//namespace tmdb





ApiGetJson::ApiGetJson(bool usessl)
{
	_p = new ApiGetJsonPrivate(this, usessl);
	_p->_url = "/movie/";
}

ApiGetJson::~ApiGetJson()
{
	delete _p;
	_p = NULL;

}

std::string ApiGetJson::json(std::string url)
{
	return _p->json(url);
}

void ApiGetJson::addOption(const std::string &key, const std::string &value)
{
	QueryOption opt;
	opt.first = key;
	opt.second = value;

	_p->_options.push_back(opt);
}

void ApiGetJson::addOption(const QueryOption &option)
{
	_p->_options.push_back(option);
}

void ApiGetJson::clearOptions()
{
	_p->_options.clear();
}

void ApiGetJson::setDefaultUrl(const std::string &url)
{
	_p->_url = url;
}

void ApiGetJson::setSSL(const bool use)
{
	_p->_usessl = use;
}

bool ApiGetJson::getSSL()
{
	return _p->_usessl;
}