# printer-interface

### Description
This is a simple printer interface which includes a server and CLI client. The server acts as a proxy server between client and printer. Together, users can print files from a printer they are not directly connected to through the proxy server. This bypasses frustration often caused by connecting to a printer.

### Requirements
The server and client rely on POSIX TCP sockets. Thus, they must be run on a Unix-like OS (any flavor of Unix or Mac).

### Configuration
The .printconfig file holds the configurations for the server and client.
- TMP_LOCATION is where files transfered to the server temporarily live while they are being printed
- PRINT_COMMAND stores the command you want to use to print the file. For example "lp -d \<printer_name\> $(TMP_LOCATION)"
- 
### Building
Use the makefile to create executables for both the server and client. The "make" command will build executables for the server and client called printserver and printclient. These will be stored in ./target. The "make" command will also build the object files stored in ./target/obj.

# Running
These steps assume you are in the root directory of this project. If you are elsewhere, adjust paths accordingly.
- To run the server, enter the following command into a terminal: ./target/printserver \<port-num\>
- To run the client, enter the following command into a terminal: ./target/printclient \<ip-addr\> \<port-num\>
