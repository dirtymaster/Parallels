FLAGS = g++ -g -std=c++17 -Wall -Wextra -Werror

MATRIX = DataStructures/Matrix/Matrix.cpp
MATRIX_H = DataStructures/Matrix/Matrix.h
GAUSS_ALGO = Algorithms/GaussAlgorithm/GaussAlgorithm.cpp
GAUSS_ALGO_H = Algorithms/GaussAlgorithm/GaussAlgorithm.h
GAUSS_CONSOLE =  ConsoleEngine/ConsoleForGauss/ConsoleForGauss.cpp
GAUSS_CONSOLE_H =  ConsoleEngine/ConsoleForGauss/ConsoleForGauss.h
GAUSS_CONSOLE_FOR_TESTING = ConsoleEngine/ConsoleForGauss/ConsoleForTestingGauss/ConsoleForTestingGauss.cpp
GAUSS_CONSOLE_FOR_TESTING_H = ConsoleEngine/ConsoleForGauss/ConsoleForTestingGauss/ConsoleForTestingGauss.h
ANT_ALGO = Algorithms/AntColonyAlgorithm/AntAlgorithm.cpp
ANT_ALGO_H = Algorithms/AntColonyAlgorithm/AntAlgorithm.h
ANT_CONSOLE = ConsoleEngine/ConsoleForAnt/ConsoleForAnt.cpp
ANT_CONSOLE_H = ConsoleEngine/ConsoleForAnt/ConsoleForAnt.h
WINOGRAD_CONSOLE = ConsoleEngine/ConsoleForWinograd/ConsoleForWinograd.cpp
WINOGRAD_CONSOLE_H = ConsoleEngine/ConsoleForWinograd/ConsoleForWinograd.h
WINOGRAD_ALGO = Algorithms/WinogradAlgorithm/WinogradAlgorithm.cpp
WINOGRAD_ALGO_H = Algorithms/WinogradAlgorithm/WinogradAlgorithm.h
MAIN = ConsoleEngine/main.cpp
TEST = Tests/Tests.cpp
ANT_BINARY = ant.out
GAUSS_BINARY = gauss.out
WINOGRAD_BINARY = winograd.out
TEST_BINARY = test.out

all: ant_build gauss_build winograd_build

ant_build:
	$(FLAGS) -DANT $(ANT_ALGO) $(MATRIX) $(ANT_CONSOLE) $(MAIN) -o $(ANT_BINARY)

ant_start:
	./$(ANT_BINARY)

ant: ant_build ant_start

gauss_build:
	$(FLAGS) -DGAUSS $(GAUSS_ALGO) $(GAUSS_CONSOLE) $(MATRIX) $(MAIN) -o $(GAUSS_BINARY)

gauss_start:
	./$(GAUSS_BINARY)

gauss: gauss_build gauss_start

winograd_build:
	$(FLAGS) -DWINOGRAD $(WINOGRAD_ALGO) $(WINOGRAD_CONSOLE) $(MATRIX) $(MAIN) -o $(WINOGRAD_BINARY)

winograd_start:
	./$(WINOGRAD_BINARY)

winograd: winograd_build winograd_start

test:
	$(FLAGS) $(MATRIX) \
	$(GAUSS_ALGO) $(GAUSS_CONSOLE) $(GAUSS_CONSOLE_FOR_TESTING) \
	$(WINOGRAD_ALGO) \
	$(ANT_ALGO) $(TEST) -o $(TEST_BINARY) -lgtest
	./$(TEST_BINARY)

leaks_check:
	CK_FORKS=no leaks -atExit -- $(TEST_BINARY)

style_check:
	cp ../materials/.clang-format .
	clang-format -i \
	$(MATRIX) $(MATRIX_H)  $(GAUSS_ALGO) $(GAUSS_ALGO_H) $(GAUSS_CONSOLE) $(GAUSS_CONSOLE_H)   \
    $(GAUSS_CONSOLE_FOR_TESTING) $(GAUSS_CONSOLE_FOR_TESTING_H) $(ANT_ALGO) $(ANT_ALGO_H)      \
    $(ANT_CONSOLE) $(ANT_CONSOLE_H) $(WINOGRAD_CONSOLE) $(WINOGRAD_CONSOLE_H) $(WINOGRAD_ALGO) \
    $(WINOGRAD_ALGO_H) $(MAIN) $(TEST)

clean:
	rm -rf *.out *.out.dSYM
