cc = g++  # use g++ compiler
path = .  # Path for includes should be current directory (plus any default paths)
standard = c++2a

flags = -std=$(standard)  # compile with standard
flags += -Wall  # compile with all warnings
flags += -pedantic  # even more warnings
flags += -I $(path)  # add parent to preprocessor include path
flags += -g  # include gdb instrumentation

link = $(cc) $(flags) -o  # final linked build to binary executable

compile = $(cc) $(flags) -c -o  # compilation to intermediary .o files

sourcepath = src/main/cpp # Path to cpp source files

all = target/printserver target/printclient

# NOTES:
# $@ evaluates to the target
# $^ evaluates to all prerequisites
# $< evalulates to the first prerequisite

# generate all executables
all : $(all)

# create directories
target/obj :
	mkdir -p target/obj

# executables
target/printserver : target/obj/tcp_socket.o target/obj/printer_server.o target/obj/config_reader.o | target/obj
	$(link) $@ $^

target/printclient : target/obj/tcp_socket.o target/obj/printer_client.o target/obj/config_reader.o | target/obj
	$(link) $@ $^

# objects
target/obj/printer_server.o : ServerClientCLI/printer_server.cc ServerClientCLI/printer_server.h | target/obj
	$(compile) $@ $<

target/obj/printer_client.o : ServerClientCLI/printer_client.cc ServerClientCLI/printer_client.h | target/obj
	$(compile) $@ $<

target/obj/tcp_socket.o : ServerClientCLI/tcp_socket.cc ServerClientCLI/tcp_socket.h | target/obj
	$(compile) $@ $<

target/obj/config_reader.o : ServerClientCLI/config_reader.cc ServerClientCLI/config_reader.h | target/obj
	$(compile) $@ $<
	
clean:
	$(RM) target/obj/* $(all)
