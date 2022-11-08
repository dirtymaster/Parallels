FLAGS = g++ -g -std=c++17 #-Wall -Wextra -Werror

COMMON = Algorithms/AbstractAlgorithm.cpp DataStructures/Matrix/Matrix.cpp
ANT = Algorithms/AntColonyAlgorithm/AntAlgorithm.cpp
GAUSS = ConsoleEngine/ConsoleForGauss/*.cpp Algorithms/Gauss/Gauss.cpp $(COMMON)
WINOGRAD = ConsoleEngine/ConsoleForWinograd/*.cpp

all: gauss_build winograd_build

ant_build:
	$(FLAGS) $(ANT) ConsoleEngine/ConsoleForAnt/*.cpp $(COMMON) -o ant.out

ant_start:
	./ant.out

ant: ant_build ant_start

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
	$(FLAGS) $(COMMON) Tests/Tests.cpp $(ANT) -o test.out

clean:
	rm -rf *.out.dSYM *.out