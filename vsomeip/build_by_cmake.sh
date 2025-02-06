cmake -H. -Bbuild -DENABLE_SIGNAL_HANDLING=1
cmake --build build --parallel 4 
cmake --install build --prefix ~/local/vsomeip

