/*****************************************************
*                                                    
*  copyright (c) 2018 Trevor Mellon                  
*  For further information contact codified@ignifi.me        
*                                                    
*  Licensed under the BSD 3-clause license           
*  See LICENCE file for more information             
*                                                    
******************************************************/

#include <boost/date_time.hpp>

# include <rapidjson/rapidjson.h>
# include <rapidjson/document.h>

#include <tmdb/movie/Movie.h>

namespace tmdb
{

	inline std::string mysqlblankstrtonull(std::string in)
	{
		if (in.size() == 0)
		{
			return "NULL";
		}

		std::stringstream ss;
		ss << "'" << in << "'";
		return ss.str();

	}

	inline std::string mysqlescapestring(std::string in)
	{
		return boost::algorithm::replace_all_copy(in, "'", "\\'");
	}

	inline uint64_t ptimetou64(boost::posix_time::ptime in)
	{
		boost::posix_time::ptime epoch = boost::posix_time::from_time_t(0);
		boost::posix_time::time_duration w = in - epoch;
		return w.total_seconds();
	}

	inline bool rjcheck(rapidjson::Value &d, std::string val)
	{

		if (d.HasMember(val.c_str()) && !d[val.c_str()].IsNull())
		{
			return true;
		}

		return false;
	}
}
