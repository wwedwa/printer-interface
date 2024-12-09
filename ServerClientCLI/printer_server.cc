#include "printer_server.h"
#include "config_reader.h"

#include <cstdlib>  // std::exit()
#include <cstring>  // std::strerror()

#include <string>
#include <iostream>
#include <fstream>

PrinterServer::PrinterServer(int port_num) : TCPSocket(port_num) {
  config_ = printconfig::ReadProperties();
}

void PrinterServer::Run() {
  std::cout << "SERVER STARTED" << std::endl;
  
  if (!Init()) {
    HandleError("TCPSocket::Init() failed: ");
  }
  if (!Bind()) {
    HandleError("TCPSocket::Bind() failed: ");
  }
  if (!Listen()) {
    HandleError("TCPSocket::Listen() failed: ");
  }

  while (1) {
    std::cout << "SERVER WAITING FOR CLIENTS..." << std::endl;
    int client_fd = 0;
    if (!Accept(&client_fd)) {
      HandleError("TCPSocket::Accept() failed: ");
    }
    std::string raw_file;
    int bytes_read = Read(&raw_file, client_fd);
    if (bytes_read <= 0) {
      HandleError("TCPSocket::Read(): ", 0);
      std::cout << "Moving to next client..." << std::endl;
      continue;
    }
    std::cout << "Read " << bytes_read << " bytes from client" << std::endl;
    WriteToFile(raw_file);
  }
}

void PrinterServer::WriteToFile(std::string raw_file) {
  std::ofstream file(config_["TMP_LOCATION"], std::ios::out | std::ios::binary);
  const char* c_raw_file = raw_file.c_str();
  file.write(c_raw_file, raw_file.length());
  system(config_["PRINT_COMMAND"].c_str());
  return;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Proper Usage: ./server <port number>" << std::endl;
    exit(1);
  }
  int port_num = std::stoi(argv[1]);
  
  PrinterServer server(port_num);
  server.Run();

  return 0;
}
