#ifndef PRINTER_CLIENT_H_
#define PRINTER_CLIENT_H_

#include "tcp_socket.h"

#include <string>

class PrinterClient : public TCPSocket {
 public:
  using TCPSocket::TCPSocket;

  void Run();

  std::string ReadFile();
};

#endif  // PRINTER_CLIENT_H_
