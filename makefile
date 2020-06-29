all: Translators/retrieve_sudoku_from_SAT.cpp Translators/reduce_sudoku_to_SAT.cpp  obj/sat_solver.o obj/main_solver.o zChaff/zchaff64/zchaff
	g++ -std=c++17 Translators/retrieve_sudoku_from_SAT.cpp -o retrieve_sudoku_from_SAT
	g++ -std=c++17 Translators/reduce_sudoku_to_SAT.cpp -o reduce_sudoku_to_SAT
	mv zChaff/zchaff64/zchaff zchaff 
	g++ -g -std=c++17 obj/main_solver.o obj/sat_solver.o -o sat_solver

obj/main_solver.o: SAT/src/main_solver.cpp 
	g++ -std=c++17 -g -I SAT/src/headers -c $< -o obj/main_solver.o

obj/sat_solver.o: SAT/src/sat_solver.cpp
	g++ -std=c++17 -g -I SAT/src/headers -c $< -o obj/sat_solver.o

zChaff/zchaff64/zchaff:
	cd zChaff/zchaff64/ && $(MAKE)

clean:
	if find obj/*.o; then rm obj/*.o; fi
	if test -f "SolucionesSudoku_zchaff.txt"; then rm SolucionesSudoku_zchaff.txt; fi
	if test -f "times_zchaff.txt"; then rm times_zchaff.txt; fi
	if test -f "SolucionesSudoku_sat_solver.txt"; then rm SolucionesSudoku_sat_solver.txt; fi
	if test -f "times_sat_solver.txt"; then rm times_sat_solver.txt; fi
	if test -f "sat_solver"; then rm sat_solver; fi
	if test -f "zchaff"; then rm zchaff; fi
	if test -f "retrieve_sudoku_from_SAT"; then rm retrieve_sudoku_from_SAT; fi
	if test -f "reduce_sudoku_to_SAT"; then rm reduce_sudoku_to_SAT; fi