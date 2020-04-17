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
#if TMDB_USE_CURL
    public:
        struct MemoryChunk
        {
            char *_chunk;
            size_t _sz;
            MemoryChunk()
            {
                _chunk = nullptr;
                _sz = 0;
            }
        };
#endif
	public:
		ApiGetPrivate(ApiGet *q, bool usessl = TMDB_DEFAULT_SSL)
		{
			_q = q;
			_usessl = usessl;
#if TMDB_USE_CURL
            _curl = curl_easy_init();
#endif
		}

        ~ApiGetPrivate()
        {
#if TMDB_USE_CURL
            curl_easy_cleanup(_curl);
#endif
        }
	public:
		std::string json(const std::string &url, const QueryOptions &opt = QueryOptions());
		std::string http(const HostParams &params, size_t &sz, const QueryOptions &opt = QueryOptions());
		std::string https(const HostParams &params, size_t &sz, const QueryOptions &opt = QueryOptions());
#if TMDB_USE_CURL
        std::string http_curl(const HostParams &params, size_t &sz, const QueryOptions &opt = QueryOptions());
        static size_t write_curl(char *ptr, size_t size, size_t nmemb, void *userdata);
#endif
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
#if TMDB_USE_CURL
        CURL  *_curl;
#endif
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

#if TMDB_USE_CURL
        data = http_curl(param, sz, opt);

#elif TMDB_USE_OPENSSL
		if (_usessl)
		{
			
			data = https(param, sz, opt);
		}
		else
		{
			data = http(param, sz, opt);
		}
#else
        data = http(param, sz, opt);
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
		std::stringstream respss;

		const size_t bufferlength = 512;

		size_t n = 0;
		size_t headersz = 0;

		do
		{
			a::streambuf tmpbuf;
			try
			{
				n = a::read(s, tmpbuf, a::transfer_at_least(1));

				char *b = new char[bufferlength + 1];
				tmpbuf.sgetn(b, n);
				b[n] = 0;

				respss << b;

				delete[] b;
			}
			catch (boost::exception& e)
			{
				//std::cout << boost::diagnostic_information(e) << std::endl;
				n = 0;
			}			
		} while (n > 0);

		std::string code;
		std::getline(respss, code);		
		headersz = code.size() + 1;
		std::stringstream codess;
		codess << code;
		std::string httpVersion;
		codess >> httpVersion;
		unsigned int statusCode;
		codess >> statusCode;
		std::string statusMessage;
		codess >> statusMessage;

		if (!respss || httpVersion.substr(0, 5) != "HTTP/")
		{
			return "";
		}

		if (statusCode != 200)
		{
			return "";
		}
		
		std::string header;
		std::getline(respss, header);
		headersz += header.size() + 1;
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
			std::getline(respss, header);
			headersz += header.size() + 1;
		}	

		boost::locale::generator g;
		std::locale loc = g.generate("en_US.UTF-8");

		std::string message = respss.str();

		std::string body(message.begin() + headersz, message.end());

		
#ifdef _DEBUG
		std::cout << body << std::endl;
#endif
		return body;		
#else
		return http(param, sz, opts);
#endif//TMDB_USE_OPENSSL
	}

#if TMDB_USE_CURL
    std::string ApiGetPrivate::http_curl(const HostParams &param, size_t &sz, const QueryOptions &opts)
    {
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

        std::stringstream getSS;

        getSS << "http://api.themoviedb.org";

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

        curl_easy_setopt(_curl, CURLOPT_URL, getSS.str().c_str());
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, &ApiGetPrivate::write_curl);

        MemoryChunk chunk;
        chunk._sz = 0;
        chunk._chunk = (char*)malloc(1);

        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &chunk);

        CURLcode hr =  curl_easy_perform(_curl);

        if(hr != CURLE_OK)
        {
            return "";
        }

        std::string ret(chunk._chunk);

        return ret;
    }

    size_t ApiGetPrivate::write_curl(char *ptr, size_t size, size_t nmemb, void *userdata)
    {
        size_t newSize = size * nmemb;

        MemoryChunk *chnk = (MemoryChunk*)userdata;

        char *out = (char*)realloc(chnk->_chunk,chnk->_sz + newSize +1);
        chnk->_chunk = out;
        memcpy(&(chnk->_chunk[chnk->_sz]), ptr, nmemb);
        chnk->_sz += newSize;
        chnk->_chunk[chnk->_sz] = 0;

        return newSize;
    }
#endif
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
