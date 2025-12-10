g++ -o test test.cpp -Wl,-rpath,${HOME}/local/gperftools/lib -Wl,--enable-new-dtags -L${HOME}/local/gperftools/lib -ltcmalloc
g++ -o tmtest tmtest.cpp -Wl,-rpath,${HOME}/local/gperftools/lib -Wl,--enable-new-dtags -L${HOME}/local/gperftools/lib -ltcmalloc
