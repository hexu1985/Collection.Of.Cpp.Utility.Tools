
file_list=(
gen/soa_on_dds_types.cxx
gen/soa_on_dds_types.h
thread_pool.hpp
threadsafe_queue.hpp
worker_thread.hpp
timer.hpp
timer.cpp
EprosimaSubWrapper.hpp
EprosimaSubWrapper.cpp
EprosimaPubWrapper.hpp
EprosimaPubWrapper.cpp
EprosimaRpcUtility.hpp
EprosimaRpcUtility.cpp
EprosimaRpcClient.hpp
EprosimaRpcClient.cpp
EprosimaRpcServer.hpp
EprosimaRpcServer.cpp
)

#file_list=(
#    daytime_client_test.cpp
#    daytime_server_test.cpp
#    DaytimeClient.cpp
#    DaytimeClient.h
#    DaytimeServer.cpp
#    DaytimeServer.h
#)

for file in ${file_list[@]}
do
  echo "-- $file --"
  cat $file
done
