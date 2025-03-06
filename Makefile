CXX = g++
CXXFLAGS = -Wall -std=c++11

# Source files
CLIENT_SRC = ./cpp/client.cpp
SERVER_SRC = ./cpp/server.cpp

# Output executables
CLIENT_BIN = ./target/client
SERVER_BIN = ./target/server

# Compilation rules
all: $(CLIENT_BIN) $(SERVER_BIN)

$(CLIENT_BIN): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC)

$(SERVER_BIN): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o $(SERVER_BIN) $(SERVER_SRC)

# Run commands
run-client: $(CLIENT_BIN)
	./$(CLIENT_BIN)

run-server: $(SERVER_BIN)
	./$(SERVER_BIN)

# Clean rule
clean:
	rm -f $(CLIENT_BIN) $(SERVER_BIN)