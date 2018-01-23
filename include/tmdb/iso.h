#ifndef TMDB_ISO_H
#define TMDB_ISO_H

#include <utility>
#include <string>
#include <boost/serialization/singleton.hpp>

#include <tmdb/isoTypes.h>

namespace iso
{
	struct Languages
	{
		std::string iso_639_1_s;
		iso_639_1::iso iso_639_1;
		std::string iso_639_2_s;
		iso_639_2::iso iso_639_2;

		std::string name;
		std::string grouping;
	};

	extern Languages isotable[];

	class IsoManagerPrivate;

	class IsoManager
	{
	public:
		IsoManager();
		~IsoManager();
	public:
		iso_639_1::iso getIso(const iso_639_2::iso &i);
		iso_639_2::iso getIso(const iso_639_1::iso &i);

		std::string getString(const iso_639_1::iso &i);
		std::string getString(const iso_639_2::iso &i);

		iso_639_1::iso to_639_1(const std::string &i);
		iso_639_2::iso to_639_2(const std::string &i);
	private:
		IsoManagerPrivate *_p;
	};

	typedef boost::serialization::singleton<IsoManager> IsoManagerSingleton;
}//namespace iso

#endif//TMDB_ISO_H