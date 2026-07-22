LIB = libmh
CC = g++
FLAG_DEBUG = -g -Wall
FLAG_RELEASE = -O2 -Wall
FALGS = -g -Wall

SRC_DIR = ./src
OBJ_DIR = ./obj

SRC = $(shell find $(SRC_DIR) -name '*.cpp')
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

.PHONY: default deploy undeploy reset build clean

default: build deploy

build: $(OBJ)
	$(CC) -shared -o $(LIB).so $^ $(FLAGS) -fPIC -g
	ar rcs $(LIB).a $^

reset: clean undeploy build deploy

clean:
	rm $(LIB).a $(LIB).so
	rm $(OBJ_DIR)/*.o
	rm $(OBJ_DIR)/core/*.o
	rm $(OBJ_DIR)/io/*.o

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(FLAGS) -fPIC -g

deploy:
	sudo mkdir -p /usr/include/mh
	sudo mkdir -p /usr/include/mh/core
	sudo mkdir -p /usr/include/mh/io	
	sudo cp ./headers/*.hpp /usr/include/mh
	sudo cp ./headers/core/*.hpp /usr/include/mh/core
	sudo cp ./headers/io/*.hpp /usr/include/mh/io
	sudo cp $(LIB).so /usr/lib
	sudo cp $(LIB).a /usr/lib
	sudo cp $(LIB).so /usr/lib64
	sudo cp $(LIB).a /usr/lib64

undeploy:
	sudo rm -rf /usr/include/mh
	sudo rm /usr/lib/$(LIB).so
	sudo rm /usr/lib/$(LIB).a
	sudo rm /usr/lib64/$(LIB).so
	sudo rm /usr/lib64/$(LIB).a
