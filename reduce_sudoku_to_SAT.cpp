
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cctype>

// A class to reduce a sudoku instance to a SAT instance
class reduceSudokuToSAT {

public:

	// Main function of the class
	// Reduce a sudoku instance to a SAT instance
	void reduceSudokuToSAT_(){

		// get the sudoku instance
		getSudokuInput();

		// Get the SAT clauses from the sudoku instance
		getUnitClauses();

		getCompletenessClauses();

		getUniquenessClauses();

		getValidityClauses();

		// Output the SAT instance
		printSatInstance();

		return;
	}

private:

	int sudoku_order;								// The order of the sudoku instance
	int square_sud_order;							// The square of the sudoku order
	std::vector< std::vector<int> > sudoku_grid;	// The sudoku board
	std::vector< std::vector<int> > clauses;		// The set of clauses of the SAT instance

	// Given the position and the digit of a variable x(i,j,d), get its integer id.
	int getVarId(int i, int j, int d){
		int sud_ord_to_4th = square_sud_order * square_sud_order;
		return i * sud_ord_to_4th + j * square_sud_order + d;
	}

	// Get the sudoku instance
	void getSudokuInput(){

		std::string sudoku_str;		// the input string representing the sudoku board initial state.
		
		std::cin >> sudoku_order >> sudoku_str;

		square_sud_order = sudoku_order*sudoku_order;

		// Check that the size of the input string match the given sudoku order
		try{
			assert(square_sud_order*square_sud_order == (int) sudoku_str.size());
		}catch(...){
			throw " Format error: the size of the input string doesn't match the given sudoku order";
		}

		sudoku_grid.resize(square_sud_order, std::vector<int>(square_sud_order, 0));

		// Fill the sudoku grid with the initial state using the sudoku input string
		for(int i=0; i < square_sud_order; i++){
			for(int j=0; j < square_sud_order; j++){

				char digit = sudoku_str[i * square_sud_order + j];
				
				// Assign the corresponding digit integer, 0 if empty. -1 if is out of range
				if(std::isdigit(digit))
					sudoku_grid[i][j] = digit - '0';
				else if(std::isalpha(digit) && std::isupper(digit))
					sudoku_grid[i][j] = digit - 'A' + 10;
				else if(digit == '.')
					sudoku_grid[i][j] = 36;
				else if(std::isalpha(digit) && std::islower(digit))
					sudoku_grid[i][j] = digit - 'a' + 37;
				else if(digit == '#')
					sudoku_grid[i][j] = 63;
				else if(digit == '@')
					sudoku_grid[i][j] = 64;
				else
					sudoku_grid[i][j] = -1;

				// Verify that the digits aren't out of range
				try{
					assert(0 <= sudoku_grid[i][j] && sudoku_grid[i][j] <= square_sud_order);
				}catch(...){
					throw " Format error: the characters in the input string are out of range";
				}
			}
		}
		
	}

	// Get the unit clauses of the position in the sudoku that are already filled
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

	// Get the clauses that guarantee all cells will be filled
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

	// Get the clauses that guarantee all the cells are filled by exactly one SAT variable
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

	}

	// Get the clauses that guarantee all the rules related to rows won't be broken
	void getValidityClausesRows(){
		
		for(int row=0; row < square_sud_order; row++){
			for(int d=1; d<=square_sud_order; d++){
				// Now we compare two positions in the row. They can't have the same digit d
				for(int c1=0; c1 < square_sud_order-1; c1++){
					for(int c2=c1+1; c2 < square_sud_order; c2++){
						

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

	// Get the clauses that guarantee all the rules related to columns won't be broken
	void getValidityClausesColumns(){
		
		for(int col=0; col < square_sud_order; col++){
			for(int d=1; d<=square_sud_order; d++){
				// Now we compare two positions in the column. They can't have the same digit d
				for(int r1=0; r1 < square_sud_order-1; r1++){
					for(int r2=r1+1; r2 < square_sud_order; r2++){

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

	// Get the clauses that guarantee all the rules related to blocks won't be broken
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

	// Get the clauses that guarantee all the rules won't be broken.
	// It calls three functions to get the 3 types of validity clauses. 
	void getValidityClauses(){
		
		getValidityClausesRows();

		getValidityClausesColumns();

		getValidityClausesBlocks();

		return;
	}

	// Get the total number of SAT variables
	// which is the sudoku order raised to 6.
	int getNumberOfVars(){

		return square_sud_order * square_sud_order * square_sud_order;

	}

	// Output the traducted SAT instance
	void printSatInstance(){

		// avoid repeated clauses
		std::sort(clauses.begin(), clauses.end());
		clauses.resize(std::distance(clauses.begin(),std::unique(clauses.begin(), clauses.end())));

		// Get the number of SAT variables and clauses.
		int number_of_vars = getNumberOfVars(),
			number_of_clauses = clauses.size();

		// Lines starting with c are comments. The one starting with p is the header
		std::cout << "c This cnf is traducted from an sudoku instance. \n"; 
		std::cout << "p cnf " << number_of_vars << " " << number_of_clauses << "\n";

		// Output all the 'number_of_clauses' clauses in separate lines with a '0' at the end
		for(auto clause : clauses){
			
			for(auto var : clause){
				std::cout << var << " ";
			}

			std::cout << "0\n";
		}

	}

	
};


int main(int argc, char const *argv[]){

	// Check the number of command line arguments is ok
	if(argc > 2){
		std::cerr << "USAGE:\n./reduce_sudoku_to_SAT < file_name\n./reduce_sudoku_to_SAT <file_name>\n";
		return 0;
	}

	// If a file is given then redirect the std input buffer to that file
	if(argc == 2){
		std::freopen(argv[1],"r",stdin);
	}

	// Create an intance of the reduceSudokuToSAT class
	// and reduce a sudoku instance to a SAT instance
	reduceSudokuToSAT reductor;
	reductor.reduceSudokuToSAT_();

	return 0;
}