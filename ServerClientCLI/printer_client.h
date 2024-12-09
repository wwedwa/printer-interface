#ifndef PRINTER_CLIENT_H_
#define PRINTER_CLIENT_H_

#include "tcp_socket.h"

#include <string>

class PrinterClient : public TCPSocket {
 public:
  PrinterClient(int port_num, std::string ip_addr, std::string file_to_print);

  void Run();

  std::string ReadFile();

 private:
  std::string file_to_print_;
};

#endif  // PRINTER_CLIENT_H_
