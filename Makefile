SOURCE = ./src/eldo.cpp ./src/tools.cpp ./src/picontrol.cpp -lwiringPi
SOURCE_STATIC = -lpthread
APP = ./bin/eldo
CC = g++

all: $(APP)

$(APP): $(SOURCE)
	mkdir ./bin 2> /dev/null; chmod -R 755 *
	$(CC) $(SOURCE) -o $(APP)

static:
	mkdir ./bin 2> /dev/null; chmod -R 755 *
	$(CC) -static $(SOURCE) $(SOURCE_STATIC) -o $(APP)-static

clean:
	rm -r ./bin

install:
	cp $(APP) /bin/eldo

install-static:
	cp $(APP)-static /bin/eldo
