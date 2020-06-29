all: zchaff sat-sud sud-sat sat 

sat-sud: src/Translators/retrieve_sudoku_from_SAT.cpp   
	@g++ -std=c++17 src/Translators/retrieve_sudoku_from_SAT.cpp -o retrieve_sudoku_from_SAT

sud-sat: src/Translators/reduce_sudoku_to_SAT.cpp 
	@g++ -std=c++17 src/Translators/reduce_sudoku_to_SAT.cpp -o reduce_sudoku_to_SAT

zchaff: src/zChaff/zchaff64/zchaff
	@mv src/zChaff/zchaff64/zchaff zchaff

sat: obj/sat_solver.o obj/main_solver.o
	@g++ -g -std=c++17 obj/main_solver.o obj/sat_solver.o -o sat_solver

obj/main_solver.o: src/SAT/main_solver.cpp 
	@g++ -std=c++17 -I src/SAT/headers -c $< -o obj/main_solver.o

obj/sat_solver.o: src/SAT/sat_solver.cpp
	@g++ -std=c++17 -I src/SAT/headers -c $< -o obj/sat_solver.o

src/zChaff/zchaff64/zchaff:
	@cd src/zChaff/zchaff64/ && $(MAKE)

clean:
	if find obj/*.o; then rm obj/*.o; fi
	if test -f "sat_solver"; then rm sat_solver; fi
	if test -f "zchaff"; then rm zchaff; fi
	if test -f "retrieve_sudoku_from_SAT"; then rm retrieve_sudoku_from_SAT; fi
	if test -f "reduce_sudoku_to_SAT"; then rm reduce_sudoku_to_SAT; fi