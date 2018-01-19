call go32
call addprog cppcheck
call addprog verapp
call addqt


mkdir sonar
mkdir tmp
cd tmp

cmake -G "NMake Makefiles" -DSONAR=ON -DTMDB_API_KEY=123 -DCMAKE_BUILD_TYPE=Release ..

nmake cppcheck
nmake verastyle

del /S /Q *

cmake -G "Visual Studio 12 2013" -DSONAR=ON -DTMDB_API_KEY=123 -DCMAKE_BUILD_TYPE=Release ..

call vslog tmdb.sln %CD%\\..\\reports\\vs.log
del /S /Q *

cd ..
rmdir tmp

call sonar-scanner
