#ifndef PRINTER_SERVER_H_
#define PRINTER_SERVER_H_

#include "tcp_socket.h"

#include <string>
#include <map>

class PrinterServer : public TCPSocket {
 public:
  PrinterServer(int portnum);

  void Run();

  void WriteToFile(std::string raw_file);

 private:
  std::map<std::string, std::string> config_;
};

#endif  // PRINTER_SERVER_H_
