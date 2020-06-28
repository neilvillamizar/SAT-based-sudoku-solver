
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <cstdlib>

class reduceSudokuToSAT {

public:

	//reduceSudokuToSAT();
	//~reduceSudokuToSAT();

	void reduceSudokuToSAT_(){

		getSudokuInput();

		getUnitClauses();

		getCompletenessClauses();

		getUniquenessClauses();

		getValidityClauses();

		if(!solvable){ 
			printSatConflict();
			return;
		}

		printSatInstance();

		return;
	}

private:

	int sudoku_order;
	int square_sud_order;
	bool solvable = true;
	std::vector< std::vector<int> > sudoku_grid;
	std::vector< std::vector<int> > clauses;

	int getVarId(int i, int j, int d){
		int sud_ord_to_4th = square_sud_order * square_sud_order;
		return i * sud_ord_to_4th + j * square_sud_order + d;
	}

	void getSudokuInput(){

		std::string sudoku_str;
		
		std::cin >> sudoku_order >> sudoku_str;

		square_sud_order = sudoku_order*sudoku_order;

		try{
			assert(square_sud_order*square_sud_order == (int) sudoku_str.size());
		}catch(...){
			throw " Format error: the size of the input string doesn't match the given sudoku order";
		}

		sudoku_grid.resize(square_sud_order, std::vector<int>(square_sud_order, 0));

		for(int i=0; i < square_sud_order; i++){
			for(int j=0; j < square_sud_order; j++){
				sudoku_grid[i][j] = sudoku_str[i * square_sud_order + j] - '0';

				try{
					assert(0 <= sudoku_grid[i][j] && sudoku_grid[i][j] <= square_sud_order);
				}catch(...){
					throw " Format error: the characters in the input string are out of range";
				}
			}
		}
		
	}

	void getUnitClauses(){

		for(int i=0; i < square_sud_order; i++){
			for(int j=0; j < square_sud_order; j++){
				// if the variable x(i,j,d) is already satisfied
				// (with d = sudoku_grid[i][j])
				// then add the unit clause
				if(sudoku_grid[i][j]){

					// the digit in the current position
					int d = sudoku_grid[i][j];

					// Generate the unit clause
					std::vector<int> unit_clause{
						getVarId(i, j, d)
					};

					// Add the unit clause
					clauses.push_back(unit_clause);

				}
			}
		}

	}

	void getCompletenessClauses(){			

		for(int i=0; i < square_sud_order; i++){
			for(int j=0; j < square_sud_order; j++){
				// if the clause is not already satisfied
				// then add the completeness clause
				if(!sudoku_grid[i][j]){

					// Generate the completeness clause
					std::vector<int> comp_clause(square_sud_order);

					for(int d=1; d <= square_sud_order; d++){
						comp_clause[d-1] = getVarId(i, j, d);
					}

					// Add the completeness clause
					clauses.push_back(comp_clause);
				}
			}
		}

	}

	void getUniquenessClauses(){
			
		for(int i = 0; i < square_sud_order; i++){
			for(int j = 0; j < square_sud_order; j++){

				// if the digit is not set then we add the uniqueness
				// clauses to ensure an unique assignment to this position
				if(!sudoku_grid[i][j]){

					for(int d1 = 1; d1 < square_sud_order; d1++){

						for(int d2 = d1+1; d2 <= square_sud_order; d2++){

							// Generate the uniqueness clause
							std::vector<int> uniq_clause{
								-getVarId(i,j,d1),
								-getVarId(i,j,d2)
							};

							// Add the uniqueness clause
							clauses.push_back(uniq_clause);
						
						}
					
					}

				}
			}
		}

		return;
	}

	void getValidityClausesRows(){
		
		for(int row=0; row < square_sud_order; row++){
			for(int d=1; d<=square_sud_order; d++){
				// Now we compare two positions in the row. They can't have the same digit d
				for(int c1=0; c1 < square_sud_order-1; c1++){
					for(int c2=c1+1; c2 < square_sud_order; c2++){
						
						// if they have the same digit, the puzzle is not solvable
						if(sudoku_grid[row][c1] && sudoku_grid[row][c1] == sudoku_grid[row][c2]){
							solvable = false;
							return;
						}

						// If any of the two positions is set to d, the other can't be set to d

						if(sudoku_grid[row][c1] == d){
							// Generate the validity clause
							std::vector<int> val_clause{
								-getVarId(row,c2,d)
							};

							// Add the validity clause
							clauses.push_back(val_clause);
							continue;
						}

						if(sudoku_grid[row][c2] == d){
							// Generate the validity clause
							std::vector<int> val_clause{
								-getVarId(row,c1,d)
							};

							// Add the validity clause
							clauses.push_back(val_clause);
							continue;
						}

						// case when no one is set to d

						// Generate the validity clause
						std::vector<int> val_clause{
							-getVarId(row,c1,d),
							-getVarId(row,c2,d)
						};

						// Add the validity clause
						clauses.push_back(val_clause);

					}
				}
			}
		}

	}

	void getValidityClausesColumns(){
		
		for(int col=0; col < square_sud_order; col++){
			for(int d=1; d<=square_sud_order; d++){
				// Now we compare two positions in the column. They can't have the same digit d
				for(int r1=0; r1 < square_sud_order-1; r1++){
					for(int r2=r1+1; r2 < square_sud_order; r2++){
						
						// if they have the same digit, the puzzle is not solvable
						if(sudoku_grid[r1][col] && sudoku_grid[r1][col] == sudoku_grid[r2][col]){
							solvable = false;
							return;
						}

						// If any of the two positions is set to d, the other can't be set to d

						if(sudoku_grid[r1][col] == d){
							// Generate the validity clause
							std::vector<int> val_clause{
								-getVarId(r2,col,d)
							};

							// Add the validity clause
							clauses.push_back(val_clause);
							continue;
						}

						if(sudoku_grid[r2][col] == d){
							// Generate the validity clause
							std::vector<int> val_clause{
								-getVarId(r1,col,d)
							};

							// Add the validity clause
							clauses.push_back(val_clause);
							continue;
						}

						// case when no one is set to d

						// Generate the validity clause
						std::vector<int> val_clause{
							-getVarId(r1,col,d),
							-getVarId(r2,col,d)
						};

						// Add the validity clause
						clauses.push_back(val_clause);

					}
				}
			}
		}

	}

	void getValidityClausesBlocks(){	

		for(int block_r=0; block_r < square_sud_order; block_r += sudoku_order){ // choose the row in the top-left-most position in block
			for(int block_c=0; block_c < square_sud_order; block_c += sudoku_order){ // choose the column in the top-left-most position in block

				for(int d=1; d <= square_sud_order; d++){
					// iterate over the positions in the block
					// pairwise compare them. They can't have the same digit d
					for(int position1 = 0; position1 < square_sud_order-1; position1++){
						for(int position2 = position1+1; position2 < square_sud_order; position2++){
							
							// get rows and columns from positions number
							int r1 = block_r + position1 / sudoku_order, c1 = block_c + position1 % sudoku_order,
								r2 = block_r + position2 / sudoku_order, c2 = block_c + position2 % sudoku_order;

							// if they have the same digit, the puzzle is not solvable
							if(sudoku_grid[r1][c1] && sudoku_grid[r1][c1] == sudoku_grid[r2][c2]){
								solvable = false;
								return;
							}

							// If any of the two positions is set to d, the other can't be set to d

							if(sudoku_grid[r1][c1] == d){
								// Generate the validity clause
								std::vector<int> val_clause{
									-getVarId(r2,c2,d)
								};

								// Add the validity clause
								clauses.push_back(val_clause);
								continue;
							}

							if(sudoku_grid[r2][c2] == d){
								// Generate the validity clause
								std::vector<int> val_clause{
									-getVarId(r1,c1,d)
								};

								// Add the validity clause
								clauses.push_back(val_clause);
								continue;
							}

							// case when no one is set to d

							// Generate the validity clause
							std::vector<int> val_clause{
								-getVarId(r1,c1,d),
								-getVarId(r2,c2,d)
							};

							// Add the validity clause
							clauses.push_back(val_clause);

						}
					}
				}
				
			} 
		}

	}

	void getValidityClauses(){
		
		getValidityClausesRows();

		if(solvable) getValidityClausesColumns();

		if(solvable) getValidityClausesBlocks();

		return;
	}

	void printSatConflict(){

		std::cout << "c This cnf is not satisfiable\n";
		std::cout << "p cnf 1 2\n";
		std::cout << "1 0 -1\n"; // p ^ !p == false

	}

	int getNumberOfVars(){

		return square_sud_order * square_sud_order * square_sud_order;

	}

	void printSatInstance(){

		// avoid repeated clauses
		std::sort(clauses.begin(), clauses.end());
		clauses.resize(std::distance(clauses.begin(),std::unique(clauses.begin(), clauses.end())));

		int number_of_vars = getNumberOfVars(),
			number_of_clauses = clauses.size();

		std::cout << "c This cnf is traducted from an sudoku instance. \n";
		std::cout << "p cnf " << number_of_vars << " " << number_of_clauses << "\n";

		for(auto clause : clauses){
			
			for(auto var : clause){
				std::cout << var << " ";
			}

			std::cout << "0\n";
		}

	}

	
};


int main(int argc, char const *argv[]){

	if(argc > 2){
		std::cerr << "USAGE:\n./reduce_sudoku_to_SAT < file_name\n./reduce_sudoku_to_SAT <file_name>\n";
		return 0;
	}

	if(argc == 2){
		//std::cout << "reading from: "<< (std::string) argv[1] << std::endl;
		std::freopen(argv[1],"r",stdin);
	}

	reduceSudokuToSAT reductor;
	reductor.reduceSudokuToSAT_();

	return 0;
}