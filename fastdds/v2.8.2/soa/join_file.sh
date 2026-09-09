
file_list=(
soa_on_dds_types.idl
thread_pool.hpp
threadsafe_queue.hpp
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
  echo "$file"
  cat $file
done
