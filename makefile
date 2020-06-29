
all: retrieve_sudoku_from_SAT.cpp reduce_sudoku_to_SAT.cpp main_solver.o sat_solver.o zchaff.64bit.2007.3.12/zchaff64/zchaff
	g++ -std=c++17 retrieve_sudoku_from_SAT.cpp -o retrieve_sudoku_from_SAT
	g++ -std=c++17 reduce_sudoku_to_SAT.cpp -o reduce_sudoku_to_SAT
	mv zchaff.64bit.2007.3.12/zchaff64/zchaff zchaff 
	g++ -std=c++17 main_solver.o sat_solver.o -o sat_solver

main_solver.o: main_solver.cpp
	g++ -std=c++17 -c main_solver.cpp

sat_solver.o: sat_solver.cpp
	g++ -std=c++17 -c sat_solver.cpp

zchaff.64bit.2007.3.12/zchaff64/zchaff:
	cd zchaff.64bit.2007.3.12/zchaff64/ && $(MAKE)

clean:
	if find *.o; then rm *.o; fi
	if test -f "SolucionesSudoku_zchaff.txt"; then rm SolucionesSudoku_zchaff.txt; fi
	if test -f "times_zchaff.txt"; then rm times_zchaff.txt; fi
	if test -f "SolucionesSudoku_sat_solver.txt"; then rm SolucionesSudoku_sat_solver.txt; fi
	if test -f "times_sat_solver.txt"; then rm times_sat_solver.txt; fi
	if test -f "sat_solver"; then rm sat_solver; fi
	if test -f "zchaff"; then rm zchaff; fi
	if test -f "retrieve_sudoku_from_SAT"; then rm retrieve_sudoku_from_SAT; fi
	if test -f "reduce_sudoku_to_SAT"; then rm reduce_sudoku_to_SAT; fi