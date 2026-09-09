
file_list=(
gen/soa_on_dds_types.cxx
gen/soa_on_dds_types.h
thread_pool.hpp
threadsafe_queue.hpp
worker_thread.hpp
timer.hpp
EprosimaSubWrapper.hpp
EprosimaPubWrapper.hpp
EprosimaRpcClient.hpp
EprosimaRpcServer.hpp
EprosimaRpcUtility.hpp
timer.cpp
EprosimaSubWrapper.cpp
EprosimaPubWrapper.cpp
EprosimaRpcClient.cpp
EprosimaRpcServer.cpp
EprosimaRpcUtility.cpp
)

for file in ${file_list[@]}
do
#  echo "$file"
  cat $file
done
