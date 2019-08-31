#include <gtest/gtest.h>

#include <string>

#include <tmdb/movie/Movie.h>
#include <tmdb/movie/Types.h>
#include <tmdb/tv/TV.h>
#include <tmdb/tv/Types.h>
#include <tmdb/Configuration.h>
#include <tmdb/ApiGet.h>

#include <boost/filesystem.hpp>

using namespace tmdb;

TEST(SearchApi, SearchMovie)
{
	Movie *m = new Movie();

	auto movies = m->search(std::wstring(L"Willow"), -1);

	EXPECT_GT(movies.size(), 0);

	delete m;
}

TEST(MovieApi, Movie)
{
	Movie *m = new Movie();

	std::shared_ptr<movie::types::Combined> cb(new movie::types::Combined);
	cb->setId(101);

	auto mo = m->scan(cb, tmdb::Movie::MovieScan);

	ASSERT_EQ(101, mo->movie.id);

	ASSERT_NE(mo->movie.title, "");

	delete m;
}

TEST(MovieApi, CastAndCrew)
{
	Movie *m = new Movie();

	std::shared_ptr<movie::types::Combined> cb(new movie::types::Combined);
	cb->setId(101);

	auto mo = m->scan(cb, Movie::CastScan);

	EXPECT_EQ(101, mo->crew.id);

	ASSERT_NE(mo->crew.cast.at(0).name, "");

	delete m;
}

TEST(MovieApi, AlternativeTitle)
{
	Movie *m = new Movie();

	std::shared_ptr<movie::types::Combined> cb(new movie::types::Combined);
	cb->setId(101);

	auto mo = m->scan(cb, Movie::AltTitlesScan);

	movie::types::AlternativeTitle alt = mo->alt_titles.titles.at(0);

	ASSERT_NE("", alt.iso_3166_1);

	ASSERT_NE(alt.title, "");

	delete m;
}

TEST(MovieApi, Videos)
{
	Movie *m = new Movie();

	std::shared_ptr<movie::types::Combined> cb(new movie::types::Combined);
	cb->setId(101);

	auto mo = m->scan(cb, Movie::VideosScan);

	types::VideosData &vid = mo->videos.videos.at(0);

	EXPECT_GT(vid.id, 0);

	ASSERT_NE(vid.name, "");

	delete m;
}

TEST(MovieApi, Keywords)
{
	Movie *m = new Movie();

	std::shared_ptr<movie::types::Combined> cb(new movie::types::Combined);
	cb->setId(101);

	auto mo = m->scan(cb, Movie::KeywordsScan);

	movie::types::KeywordPair &kw = mo->keywords.at(0);

	EXPECT_GT(kw.first, 0);

	ASSERT_NE(kw.second, "");

	delete m;
}

TEST(ImageDownloader, ImageDownloader)
{
	Movie *m = new Movie();

	std::shared_ptr<movie::types::Combined> cb(new movie::types::Combined);
	cb->setId(101);

	auto ms = m->scan(cb, Movie::MovieScan);

	std::string partialurl = ms->movie.backdrop_path;

	Configuration &c = ConfigurationSingleton::get_mutable_instance();

	std::string url =
		c.getImageUrl(ImageTypeBackdrop, ImageMedium, partialurl);

	ApiGet &api = ApiGetSingleton::get_mutable_instance();	

	std::string filename("test.jpg");
	boost::filesystem::path p(filename);
	api.saveImage(url, p.string());	

	bool exists = boost::filesystem::exists(p);

	ASSERT_NE(exists, false);

	if ( exists && boost::filesystem::is_regular_file(p))
	{
		uint64_t sz = boost::filesystem::file_size(p);

		ASSERT_GT(sz, 0);
	}	

	delete m;
}

TEST(TVApi, Search)
{
	tmdb::TV tv;
	auto r = tv.search(L"moomins");
	size_t sz = r.size();
	ASSERT_GT(sz, 0);
}

TEST(TVApi, TV)
{
	std::shared_ptr<tmdb::tv::types::Combined> c(new tmdb::tv::types::Combined);
	c->setId(13792l);
	tmdb::TV tv;
	tv.scan(c);

	ASSERT_EQ(c->tv.name, "The Moomins");

}

TEST(TVApi, Seasons)
{
	std::shared_ptr<tmdb::tv::types::Combined> c(new tmdb::tv::types::Combined);
	c->setId(60735);
	tmdb::TV tv;
	tv.scan(c);

	size_t n = c->tv.seasons.size();

	ASSERT_GT(n, 0);
}
