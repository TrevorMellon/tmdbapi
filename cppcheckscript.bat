rem %1 output
rem %2 src
rem %3 hdrs

call addprog cppcheck

cppcheck --force %3  --xml --library=windows --platform=win32A --std=c++11 --enable=all %2 2> %1