APP = eldo-multicast
SOURCE_STATIC = -lpthread
CLIENT_SOURCE = ./src/eldo.cpp ./src/tools.cpp ./src/picontrol.cpp -lwiringPi
SERVER_SOURCE = ./src/server.cpp
CLIENT_APP = ./bin/eldo-client
SERVER_APP = ./bin/eldo-server
CC = g++ -Wall -W

all: $(APP)

$(APP): $(CLIENT_SOURCE)
	mkdir ./bin 2> /dev/null; chmod -R 755 *
	$(CC) $(CLIENT_SOURCE) -o $(CLIENT_APP)
	$(CC) $(SERVER_SOURCE) -o $(SERVER_APP)

static:
	mkdir ./bin 2> /dev/null; chmod -R 755 *
	$(CC) -static $(CLIENT_SOURCE) $(SOURCE_STATIC) -o $(CLIENT_APP)-static
	$(CC) -static $(SERVER_SOURCE) -o $(SERVER_APP)-static
	
clean:
	rm -r ./bin

install:
	cp $(CLIENT_APP) /bin/eldo-client
	cp $(SERVER_APP) /bin/eldo-server

install-static:
	cp $(CLIENT_APP)-static /bin/eldo-client
	cp $(SERVER_APP)-static /bin/eldo-server
