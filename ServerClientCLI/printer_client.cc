#include "printer_client.h"

#include <fstream>

PrinterClient::PrinterClient(int port_num,
                            std::string ip_addr,
                            std::string file_to_print) :
                            TCPSocket(port_num, ip_addr) {
  file_to_print_ = file_to_print;
  }

void PrinterClient::Run() {
  if (!Init()) {
    HandleError("TCPSocket::Init() failed: ");
  }
  if (!Connect()) {
    HandleError("TCPSocket::Connect() failed: ");
  }
  std::string to_send = ReadFile();
  int bytes_sent = Write(to_send);
  std::cout << "Sent " << bytes_sent << " bytes" << std::endl;
  if (bytes_sent < 0) {
    HandleError("TCPSocket::Write() failed: ");
  }
}

std::string PrinterClient::ReadFile() {
  std::ifstream file(file_to_print_);
  file.seekg(0, file.end);
  int length = file.tellg();
  file.seekg(0, file.beg);
  char* c_raw_file = new char[length];
  file.read(c_raw_file, length);
  std::string raw_file(c_raw_file, length);
  delete[] c_raw_file;  // Clean up char array

  return raw_file;
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Proper Usage: ./client <IP addr> <port number> <path to file to print>" << std::endl;
    exit(1);
  }
  std::string ip_addr(argv[1]);
  int port_num = std::stoi(argv[2]);
  std::string file_to_print(argv[3]);
  
  PrinterClient client(port_num, ip_addr, file_to_print);
  client.Run();

  return 0;
}
