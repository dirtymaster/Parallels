FLAGS = g++ -g -std=c++17 -Wall -Wextra -Werror

COMMON = Algorithms/AbstractAlgorithm.cpp DataStructures/Matrix/Matrix.cpp
GAUSS = ConsoleEngine/ConsoleForGauss/*.cpp Algorithms/Gauss/Gauss.cpp $(COMMON)
WINOGRAD = ConsoleEngine/ConsoleForWinograd/*.cpp

all: gauss_build winograd_build

gauss_build:
	$(FLAGS) $(GAUSS) -o gauss.out

gauss_start:
	./gauss.out

gauss: gauss_build gauss_start

winograd_build:
	$(FLAGS) $(WINOGRAD) -o wingograd.out
	./wingograd.out

winograd_start:
	./wingograd.out

winograd: winograd_build winograd_start

test:
	$(FLAGS) -lgtest $(COMMON) Algorithms/WinogradAlgorithm/WinogradAlgorithm.cpp Tests/Tests.cpp -o test.out
	CK_FORKS=no leaks -atExit -- ./test.out

clean:
	rm -rf *.out *.out.dSYM