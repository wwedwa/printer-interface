// Copyright 2024 William Edwards
#ifndef TCP_SOCKET_H_
#define TCP_SOCKET_H_

#include <sys/socket.h>  // header for sockets, using socket
#include <sys/types.h>  // For getaddrinfo and its dependents
#include <netdb.h>  // For getaddrinfo and its dependents
#include <netinet/in.h>  // Contains sockaddr_in struct
#include <arpa/inet.h>  // Contains inet_pton()
#include <unistd.h>  // Unix standard header, using close

#include <cassert>  // using assert
#include <cerrno>  // using errno
#include <cstddef>  // using std::size_t
#include <cstdlib>  // exit
#include <cstring>  // using strncpy, strerror

#include <string>
#include <iostream>


class TCPSocket {
 public:
  /// Initializes the socket address struct (::sockaddr_un)
  explicit TCPSocket(int port_num, std::string ip_addr = "");


  bool Init();


  bool Bind();


  bool Listen(std::size_t max_connections = 1) const;


  // Parameter int* is an output parameter returning a file descriptor to a
  // client request socket connection.
  bool Accept(int* client_request_socket_filedesc) const;


  bool Connect() const;


  // buffer is an output parameter
  // If sock fildes 0, reads from the object's socket_fd_
  ::ssize_t Read(std::string* buffer,
                 int socket_file_descriptor = 0) const;


  // If sock fildes 0, writes to the object's socket_fd_
  ::ssize_t Write(const std::string& bytes,
                  int socket_file_descriptor = 0) const;


  void Close() const;


  void HandleError(std::string error_msg, bool exit = 1) const;

 protected:
  int socket_fd_;  // file descriptor for socket this class wraps
  ::sockaddr_in sock_addr_;  // socket address from sys/un.h

  int port_num_;  // port number to bind socket to
};

#endif  // TCP_SOCKET_H_
