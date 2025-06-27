flags := -Wall -Werror -Wextra -pedantic -Wshadow -Wconversion -O3 -DNDEBUG
debug_flags:= -Wall -Werror -Wextra -pedantic -Wshadow -Wconversion -g -DDEBUG

loc: loc.o main.o
	g++ loc.o main.o $(flags) -lboost_program_options -o loc

loc.o: loc.hpp loc.cpp
	g++ loc.cpp -c $(flags) -o loc.o

main.o: main.cpp
	g++ main.cpp -c $(flags) -o main.o

debug: loc_debug.o main_debug.o
	g++ loc_debug.o main_debug.o $(debug_flags) -lboost_program_options -o loc_debug

loc_debug.o: loc.hpp loc.cpp
	g++ loc.cpp -c $(debug_flags) -o loc_debug.o

main_debug.o: main.cpp
	g++ main.cpp -c $(debug_flags) -o main_debug.o

clean:
	rm -f *.o *.exe loc loc_debug