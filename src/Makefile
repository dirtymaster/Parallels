FLAGS = g++ -g -std=c++17 -Wall -Wextra -Werror

WINOGRAD = ConsoleEngine/ConsoleForWinograd/*.cpp

all: winograd

winograd:
	$(FLAGS) $(WINOGRAD) -o wingograd.out
	./wingograd.out

qwerty:
	$(FLAGS) Algorithms/WinogradALgorithm/*.cpp DataStructures/Matrix/Matrix.cpp -o qwerty.out
	./qwerty.out

clean:
	rm -rf *.out