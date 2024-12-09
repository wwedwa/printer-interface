// Copyright 2024 William Edwards
#include "tcp_socket.h"

#include <cstdint>

TCPSocket::TCPSocket(int port_num, std::string ip_addr) {
  socket_fd_ = 0;
  port_num_ = port_num;

  sock_addr_ = {};  // initialize struct
  sock_addr_.sin_family = AF_INET;  // set to IPv4 tcp socket
  sock_addr_.sin_port = htons(port_num);  // set to given port number
  // Set addr to proper address based on given ip_addr
  if (ip_addr != "") {
    ::inet_pton(AF_INET, ip_addr.c_str(), &(sock_addr_.sin_addr));
  } else {
    sock_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
  }
}


bool TCPSocket::Init() {
  socket_fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd_ > 0) {
    return true;
  }
  return false;
}


bool TCPSocket::Bind() {
  if (!bind(socket_fd_,  // returns 0 if successful
            reinterpret_cast<const sockaddr*>(&sock_addr_),
            sizeof(sock_addr_))) {
    return true;
  }
  return false;
}


bool TCPSocket::Listen(std::size_t max_connections) const {
  if (!listen(socket_fd_, max_connections)) { // returns 0 if successful
    return true;
  }
  return false;
}


bool TCPSocket::Accept(int* req_socket_fd) const {
    *req_socket_fd = accept(socket_fd_, nullptr, nullptr);
    if (req_socket_fd) {
      return true;
    }
    return false;
}


bool TCPSocket::Connect() const {
  if (!connect(socket_fd_,  // returns 0 if successful
              reinterpret_cast<const sockaddr*>(&sock_addr_),
              sizeof(sock_addr_))) {
    return true;
  }
  return false;
}


::ssize_t TCPSocket::Read(std::string* output,
                             int socket_fd) const {
  if (!socket_fd) {
    socket_fd = socket_fd_;
  }

  const std::size_t kBufferSize = 64;  // buffer is in stack, keep it small
  char buffer[kBufferSize];

  // First 4 bytes read is the size of the remaining message
  // Unsigned so that bit shifting doesn't leave trailing 1s
  unsigned char size_chars[4];

  // First, read size from stream
  ::ssize_t total_bytes_read, bytes_read;
  total_bytes_read = bytes_read = ::read(socket_fd, size_chars, 4);

  // writer disconnected or error occured
  if (bytes_read <= 0) {
    return total_bytes_read;
  }
  // Order is big endian (Write() will prepend the size in the same way)
  std::uint32_t size = (size_chars[0] << 24) |
                       (size_chars[1] << 16) |
                       (size_chars[2] << 8) |
                       size_chars[3];
  size = size + 4;  // +4 for bytes already read
  std::size_t num_bytes = kBufferSize;

  // if there is more to read, accumulate and repeat
  while (total_bytes_read < size) {
    bytes_read = ::read(socket_fd, buffer, num_bytes);
    if (bytes_read <= 0) {
      return total_bytes_read;
    }
    total_bytes_read += bytes_read;

    output->insert(output->length(), buffer, bytes_read);

    // Shrink buffer size if remaining bytes is less than current size
    if (size - total_bytes_read < num_bytes) {
      num_bytes = size - total_bytes_read;
    }
  }

  return total_bytes_read;
}


::ssize_t TCPSocket::Write(const std::string& bytes,
                           int socket_fd) const {
  if (!socket_fd) {
    socket_fd = socket_fd_;
  }

  if (socket_fd_ <= 0) {
    return -1;
  }

  ::ssize_t bytes_wrote, total_bytes_wrote = 0;

  // Ensure size is 32 bits (will be first 4 bytes in msg)
  std::uint32_t size = bytes.length();
  char size_chars[4];

  // Note the big endian order
  size_chars[0] = (size >> 24) & 0xFF;
  size_chars[1] = (size >> 16) & 0xFF;
  size_chars[2] = (size >> 8) & 0xFF;
  size_chars[3] = size & 0xFF;

  std::string to_send = bytes;
  to_send.insert(0, size_chars, 4);

  do {
    bytes_wrote = ::write(socket_fd,
                          to_send.c_str() + total_bytes_wrote,
                          to_send.size());
    if (bytes_wrote <= 0) {
      return bytes_wrote;
    }
    total_bytes_wrote += bytes_wrote;  // accumulate bytes written
  } while (total_bytes_wrote < static_cast<::ssize_t>(to_send.size()));

  return total_bytes_wrote;
}

void TCPSocket::Close() const {
  ::close(socket_fd_);
}

void TCPSocket::HandleError(std::string error_msg, bool exit) const {
  std::cerr << error_msg << std::strerror(errno) << " (errno: "
            << errno << ")" << std::endl;
  if (exit) {
    std::exit(errno);
  }
}
