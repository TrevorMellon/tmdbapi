
call addvs
call vcvars64

call addboost
call addicu
call addz
call addrapidjson
call addcurl
call addopenssl

mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DTMDB_API_KEY=%TMDB_KEY% -DBUILD_TEST=ON -DCURL_NO_CURL_CMAKE=ON ..

msbuild /p:Configuration=Release tmdbapi.sln
cd Release
tmdbtest.exe
