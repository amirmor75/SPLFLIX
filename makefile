# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
LFLAGS  = -L/usr/libz

# All Targets
all: splflix

# Tool invocations
# Executable "splflix" depends on
splflix: bin/Main.o bin/Action.o bin/Session.o bin/User.o bin/Watchable.o bin/json.o
	@echo 'Building target: splflix'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/splflix bin/Main.o bin/Action.o bin/Session.o bin/User.o bin/Watchable.o bin/json.o $(LFLAGS)
	@echo 'Finished building target: splflix'
	@echo ' '

#target objects files
bin/Main.o: src/Main.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Main.o src/Main.cpp

bin/Action.o: src/Action.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp
 #include/Action.h include/Session.h include/User.h

bin/Session.o: src/Session.cpp
#include/Session.h include/Watchable.h include/json.hpp include/Action.h include/User.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Session.o src/Session.cpp

bin/User.o: src/User.cpp
 #include/Session.h include/User.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/User.o src/User.cpp

bin/Watchable.o: src/Watchable.cpp
#include/Session.h include/Watchable.h
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Watchable.o src/Watchable.cpp

bin/json.o: include/json.hpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/json.o -x c++ include/json.hpp

#Clean the build directory
clean:
	rm -f bin/*


