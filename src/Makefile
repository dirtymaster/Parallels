FLAGS = g++ -g -std=c++17 -Wall -Wextra -Werror

COMMON = Algorithms/AbstractAlgorithm.cpp DataStructures/Matrix/Matrix.cpp
GAUSS = ConsoleEngine/ConsoleForGauss/ConsoleForGauss.cpp Algorithms/Gauss/Gauss.cpp
WINOGRAD = ConsoleEngine/ConsoleForWinograd/*.cpp
TEST = Tests/Tests.cpp

all: gauss_build winograd_build

gauss_build:
	$(FLAGS) $(GAUSS) $(COMMON) ConsoleEngine/ConsoleForGauss/main.cpp -o gauss.out

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
	$(FLAGS) $(COMMON) $(GAUSS) ConsoleEngine/ConsoleForGauss/ConsoleForTestingGauss/ConsoleForTestingGauss.cpp \
	Algorithms/WinogradAlgorithm/WinogradAlgorithm.cpp $(TEST) -o test.out -lgtest
	./test.out

clean:
	rm -rf *.out *.out.dSYM