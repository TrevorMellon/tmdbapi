#include <gtest/gtest.h>

#include <string>

#include <tmdb/movie/Movie.h>
#include <tmdb/movie/Types.h>
#include <tmdb/Configuration.h>
#include <tmdb/ApiGet.h>

#include <boost/filesystem.hpp>

using namespace tmdb;

TEST(SearchApi, SearchMovie)
{
	Movie *m = new Movie();

	uint64_t id = m->search(std::wstring(L"Willow"), -1);

	std::vector<data::Movie> mv = m->getSearchResults();

	EXPECT_GT(mv.size(), 0);

	if (!mv.empty())
	{
		EXPECT_EQ(mv.at(0).id, id);
	}

	delete m;
}

TEST(MovieApi, Movie)
{
	Movie *m = new Movie();

	std::shared_ptr<data::MovieCombined> cb(new data::MovieCombined);
	cb->setId(101);

	auto mo = m->scan(cb, tmdb::Movie::MovieScan);

	EXPECT_EQ(101, mo->movie.id);

	ASSERT_NE(mo->movie.title, "");

	delete m;
}

TEST(MovieApi, CastAndCrew)
{
	Movie *m = new Movie();

	std::shared_ptr<data::MovieCombined> cb(new data::MovieCombined);
	cb->setId(101);

	auto mo = m->scan(cb, Movie::CastScan);

	EXPECT_EQ(101, mo->crew.id);

	ASSERT_NE(mo->crew.cast.at(0).name, "");

	delete m;
}

TEST(MovieApi, AlternativeTitle)
{
	Movie *m = new Movie();

	std::shared_ptr<data::MovieCombined> cb(new data::MovieCombined);
	cb->setId(101);

	auto mo = m->scan(cb, Movie::AltTitlesScan);

	data::AlternativeTitle alt = mo->alt_titles.titles.at(0);

	ASSERT_NE("", alt.iso_3166_1);

	ASSERT_NE(alt.title, "");

	delete m;
}

TEST(MovieApi, Videos)
{
	Movie *m = new Movie();

	std::shared_ptr<data::MovieCombined> cb(new data::MovieCombined);
	cb->setId(101);

	auto mo = m->scan(cb, Movie::VideosScan);

	data::VideosData &vid = mo->videos.videos.at(0);

	EXPECT_GT(vid.id, 0);

	ASSERT_NE(vid.name, "");

	delete m;
}

TEST(MovieApi, Keywords)
{
	Movie *m = new Movie();

	std::shared_ptr<data::MovieCombined> cb(new data::MovieCombined);
	cb->setId(101);

	auto mo = m->scan(cb, Movie::KeywordsScan);

	data::KeywordPair &kw = mo->keywords.at(0);

	EXPECT_GT(kw.first, 0);

	ASSERT_NE(kw.second, "");

	delete m;
}

TEST(Configuration, Configuration)
{
	Movie *m = new Movie();

	std::shared_ptr<data::MovieCombined> cb(new data::MovieCombined);
	cb->setId(101);

	auto ms = m->scan(cb, Movie::MovieScan);

	std::string partialurl = ms->movie.backdrop_path;

	Configuration &c = ConfigurationSingleton::get_mutable_instance();

	std::string url =
		c.getImageUrl(ImageTypeBackdrop, ImageMedium, partialurl);

	std::stringstream ss;

	ss << "http://image.tmdb.org/t/p/"
		<< "w780"
		<< "/mGN0lH2phYfesyEVqP2xvGUaxAQ.jpg";

	EXPECT_EQ(ss.str(), url);

	delete m;
}

TEST(ImageDownloader, ImageDownloader)
{
	Movie *m = new Movie();

	std::shared_ptr<data::MovieCombined> cb(new data::MovieCombined);
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
