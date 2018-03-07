/*****************************************************
*                                                    *
*  copyright (c) 2018 Trevor Mellon                  *
*  For further information contact t@jno.io          *
*                                                    *
*  Licensed under the BSD 3-clause license           *
*  See LICENCE file for more information             *
*                                                    *
******************************************************/

#include <tmdb/ApiGet.h>
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

#include <boost/filesystem.hpp>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>


#if TMDB_USE_CURL
# include <curl/curl.h>
#endif

namespace a = boost::asio;
namespace t = boost::asio::ip;



using namespace tmdb;

namespace tmdb
{
	template< typename T >
	struct data_deleter
	{
		void operator ()(T const * p)
		{
			delete[] p;
		}
	};

	struct HostParams
	{
		std::string host;
		std::string accept;
		std::string url;
		bool https;

		void fromUrl(std::string url)
		{
			std::vector<std::string> splitstr;
			boost::algorithm::split(splitstr, url, boost::is_any_of("/"));

			if (splitstr.size() > 2)
			{
				host = splitstr.at(2);
				if (splitstr[0] == "http:")
				{
					https = false;
				}
				else
				{
					https = true;
				}
			}

			auto iter = splitstr.begin();
			std::advance(iter, 3);

			std::stringstream ss;

			while (iter != splitstr.end())
			{
				ss << "/";
				ss << *iter;
				++iter;
			}

			this->url = ss.str();
		}
	};

	std::string url_encode(const std::wstring &value, bool fixquery = false)
	{
#if TMDB_USE_CURL
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

		boost::locale::generator gen;
		std::locale loc = gen("UTF-8");

		std::string val = boost::locale::conv::utf_to_utf<char>(value);

		for (std::string::const_iterator i = val.begin(), n = val.end(); i != n; ++i)
		{
			char c = (*i);	

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

	

	class ApiGetPrivate
	{
	public:
		ApiGetPrivate(ApiGet *q, bool usessl = TMDB_DEFAULT_SSL)
		{
			_q = q;
			_usessl = usessl;
		}
	public:
		std::string json(const std::string &url, const QueryOptions &opt = QueryOptions());
		std::string http(const HostParams &params, size_t &sz, const QueryOptions &opt = QueryOptions());
		std::string https(const HostParams &params, size_t &sz, const QueryOptions &opt = QueryOptions());
		std::shared_ptr<uint8_t> getImage(std::string url, size_t &sz);
		void saveImage(std::string url, std::string filename);
	public:
		std::string _url;
		std::vector<QueryOption> _options;
		ApiGet *_q;
		static boost::posix_time::ptime _tp;
		bool _usessl;
		boost::mutex mtx;
		boost::mutex imagesaveMtx;
	};

	boost::posix_time::ptime ApiGetPrivate::_tp = boost::posix_time::microsec_clock::local_time();

	

	std::string ApiGetPrivate::json(const std::string &partialurl, const QueryOptions &opt)
	{
		mtx.lock();
		std::string data;
		HostParams param;

		param.host = "api.themoviedb.org";
		param.accept = "application/json";
		param.url = partialurl;

		size_t sz = 0;

#if TMDB_USE_OPENSSL
		if (_usessl)
		{
			
			data = https(param, sz, opt);
		}
		else
		{
#endif
			data = http(param, sz, opt);
#if TMDB_USE_OPENSSL
		}
#endif
		mtx.unlock();

		return data;
	}

	std::shared_ptr<uint8_t> ApiGetPrivate::getImage(std::string url, size_t &sz)
	{
		mtx.lock();
		HostParams param;
		param.fromUrl(url);
		param.accept = "image/jpeg";
		std::string t;
		
		if (param.https)
		{
			t = https(param, sz);
		}
		else
		{
			t = http(param, sz);
		}

		std::shared_ptr<uint8_t> ret(new uint8_t[sz], data_deleter<uint8_t>());

		uint8_t *r = ret.get();
		const char *ptr = t.c_str();

		for (size_t i = 0; i < sz; ++i)
		{
			r[i] = ptr[i];
		}

		mtx.unlock();

		return ret;
	}

	void ApiGetPrivate::saveImage(std::string url, std::string filename)
	{
		imagesaveMtx.lock();
		size_t sz = 0;
		auto r = getImage(url, sz);

		boost::filesystem::path p(filename);
		boost::filesystem::path parent = p.parent_path();
		if (!parent.empty())
		{
			boost::filesystem::create_directories(parent);
		}

		std::ofstream ofs(filename, std::ios::binary);
		ofs.write((char*)r.get(), sz);
		ofs.close();
		imagesaveMtx.unlock();
	}


	std::string ApiGetPrivate::http(const HostParams &param, size_t &sz, const QueryOptions &opts)
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
		t::tcp::resolver::query q(param.host, "http");
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
		if (param.url != "")
		{
			getSS << param.url;
		}
		else
		{
			getSS << _url;
		}
		getSS << "?api_key=" << TMDB_API_KEY;
		
		for (auto i: opts)
		{
			std::string key = i.first;
			std::string value = i.second;
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
		reqStrm << "Host: " << param.host << "\r\n";
		reqStrm << "Accept: " << param.accept << "\r\n";
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

		headersz = resp.size();		

		size_t datalen = contentlength - headersz;
		

		boost::locale::generator g;
		std::locale loc = g.generate("en_US.UTF-8");

		std::stringstream bodyss;
		bodyss.imbue(loc);
		

		try
		{
			a::read(s, resp, a::transfer_exactly(datalen));
		}
		catch (boost::exception &e)
		{
			std::cout << boost::diagnostic_information(e) << std::endl;
		}	

		bodyss << &resp;
		sz = datalen;

#ifdef _DEBUG
		std::cout << bodyss.str() << std::endl;
#endif
		return bodyss.str();		
	}


	std::string ApiGetPrivate::https(const HostParams &param, size_t &sz, const QueryOptions &opts)
	{
#if TMDB_USE_OPENSSL
		boost::system::error_code ec;

		boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23_client);
		ctx.set_verify_mode(boost::asio::ssl::verify_peer);
		//ctx.set_default_verify_paths(ec);
		ctx.use_certificate_chain_file(TMDB_PEM_FILE);
		ctx.load_verify_file(TMDB_PEM_FILE);

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
		t::tcp::resolver::query q(param.host, "https");
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
		if (param.url != "")
		{
			getSS << param.url;
		}
		else
		{
			getSS << _url;
		}
		getSS << "?api_key=" << TMDB_API_KEY;

		for (QueryOption i: opts)
		{
			std::string key = i.first;
			std::string value = i.second;

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
		reqStrm << "Host: " << param.host << "\r\n";
		reqStrm << "Accept: " << param.accept << "\r\n";
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

		sz = contentlength - headersz;

#ifdef _DEBUG
		std::cout << bodyss.str() << std::endl;
#endif
		return bodyss.str();		
#else
		return http(param, sz, opts);
#endif//TMDB_USE_OPENSSL
	}
}//namespace tmdb

ApiGet::ApiGet(bool usessl)
{
	_p = new ApiGetPrivate(this, usessl);
	_p->_url = "/movie/";
}

ApiGet::~ApiGet()
{
	delete _p;
	_p = NULL;

}

std::string ApiGet::json(std::string url, const QueryOptions &opts)
{
	return _p->json(url, opts);
}

void ApiGet::setSSL(const bool use)
{
	_p->_usessl = use;
}

bool ApiGet::getSSL()
{
	return _p->_usessl;
}

std::shared_ptr<uint8_t> ApiGet::getImage(std::string url, size_t &sz)
{
	
	return _p->getImage(url, sz);
}

void ApiGet::saveImage(std::string url, std::string filename)
{
	_p->saveImage(url, filename);
}