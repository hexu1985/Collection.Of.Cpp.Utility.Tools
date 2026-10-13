
file_list=(
socket_address.hpp
socket_epoll.hpp
socket_exception.hpp
socket_host.hpp
socket.hpp
socket_poll.hpp
socket_select.hpp
socket_selector.hpp
socket_socket.hpp
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
