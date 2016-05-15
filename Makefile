SOURCE = ./src/eldo.cpp ./src/tools.cpp ./src/picontrol.cpp -lwiringPi
APP = ./bin/eldo
CC = g++

all: $(APP)

$(APP): $(SOURCE)
	mkdir ./bin
	chmod -R 755 *
	$(CC) $(SOURCE) -o $(APP)

clean:
	rm -r ./bin

install:
	cp ./bin/eldo /bin/eldo
