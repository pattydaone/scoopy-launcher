files := main.cpp desktop_file_parser.cpp utilities.cpp finder.cpp event.cpp
obj_files := main.o desktop_file_parser.o utilities.o finder.o event.o

debug_files := main_debug.cpp desktop_file_parser.cpp utilities.cpp finder.cpp event.cpp
debug_obj_files := main_debug.o desktop_file_parser.o utilities.o finder.o event.o

current_dir := $(shell pwd)

all: bin build clean

bin:
	mkdir -p ../bin

build: $(files)
	g++ -std=c++23 -Wall -g -I $(current_dir) -c $(files)
	g++ $(obj_files) -o ../bin/main
clean:
	rm -f $(obj_files)
run:
	../bin/main
debug: $(debug_files)
	g++ -std=c++23 -Wall -g -I $(current_dir) -c $(debug_files)
	g++ $(debug_obj_files) -o ../bin/main_debug
	rm -f $(debug_obj_files)
