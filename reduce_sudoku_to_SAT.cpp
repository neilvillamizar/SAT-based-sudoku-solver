
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

int get_var_id(int i, int j, int d, int n){
	int N2 = n * n;
	int N4 = N2 * N2;
	return i * N4 + j * N2 + d;
}

/*void test_get_var_id(int n){
	for(int i=0;i<n*n; i++){
		for(int j=0; j<n*n; j++){
			for(int d=1; d<=n*n; d++){
				std::cout << get_var_id(i,j,d,n) << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}*/

void get_sudoku_input(int & n, std::vector< std::vector<int> > & sudoku_grid){

	std::string sudoku_str;
	int N2;
	
	std::cin >> n >> sudoku_str;

	N2 = n*n;

	sudoku_grid.resize(N2, std::vector<int>(N2, 0));

	for(int i=0; i < N2; i++){
		for(int j=0; j < N2; j++){
			sudoku_grid[i][j] = sudoku_str[i * N2 + j] - '0';
			std::cout << sudoku_grid[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void get_completeness_clauses(int n, std::vector< std::vector<int> > & clauses, std::vector< std::vector<int> > & sudoku_grid){
	
	int N2 = n * n;	

	for(int i=0; i < N2; i++){
		for(int j=0; j < N2; j++){
			// if the clause is not already satisfied
			// then add the completeness clause
			if(!sudoku_grid[i][j]){

				// Generate the completeness clause
				std::vector<int> comp_clause(N2);

				for(int d=1; d <= N2; d++){
					comp_clause[d-1] = get_var_id(i, j, d, n);
				}

				// Add the completeness clause
				clauses.push_back(comp_clause);
			}
		}
	}

}

void get_uniqueness_clauses(int n, std::vector< std::vector<int> > & clauses, std::vector< std::vector<int> > & sudoku_grid){
	
	int N2 = n * n;	

	for(int i = 0; i < N2; i++){
		for(int j = 0; j < N2; j++){

			// if the digit is not set then we add the uniqueness
			// clauses to ensure an unique assignment to this position
			if(!sudoku_grid[i][j]){

				for(int d1 = 1; d1 < N2; d1++){

					for(int d2 = d1+1; d2 <= N2; d2++){

						// Generate the uniqueness clause
						std::vector<int> uniq_clause{
							-get_var_id(i,j,d1,n),
							-get_var_id(i,j,d2,n)
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

void get_validity_clauses_rows(int n, std::vector< std::vector<int> > & clauses, 
													std::vector< std::vector<int> > & sudoku_grid, bool & solv){
	
	int N2 = n * n;

	for(int row=0; row < N2; row++){
		for(int d=1; d<=N2; d++){
			// Now we compare two positions in the row. They can't have the same digit d
			for(int c1=0; c1 < N2-1; c1++){
				for(int c2=c1+1; c2 < N2; c2++){
					// if they have the same digit, the puzzle is not solvable
					if(sudoku_grid[row][c1] && sudoku_grid[row][c1] == sudoku_grid[row][c2]){
						solv = false;
						return;
					}

					// If any of the two positions is set to d, the other can't be set to d

					if(sudoku_grid[row][c1]==d){
						// Generate the validity clause
						std::vector<int> val_clause{
							-get_var_id(row,c2,d,n)
						};

						// Add the validity clause
						clauses.push_back(val_clause);
						continue;
					}

					if(sudoku_grid[row][c2]==d){
						// Generate the validity clause
						std::vector<int> val_clause{
							-get_var_id(row,c1,d,n)
						};

						// Add the validity clause
						clauses.push_back(val_clause);
						continue;
					}

					// case when no one is set to d

					// Generate the validity clause
					std::vector<int> val_clause{
						-get_var_id(row,c1,d,n),
						-get_var_id(row,c2,d,n)
					};

					// Add the validity clause
					clauses.push_back(val_clause);

				}
			}
		}
	}

}

void get_validity_clauses_columns(int n, std::vector< std::vector<int> > & clauses, 
													std::vector< std::vector<int> > & sudoku_grid, bool & solv){
	int N2 = n * n;

	for(int col=0; col < N2; col++){
		for(int d=1; d<=N2; d++){
			// Now we compare two positions in the column. They can't have the same digit d
			for(int r1=0; r1 < N2-1; r1++){
				for(int r2=r1+1; r2 < N2; r2++){
					// if they have the same digit, the puzzle is not solvable
					if(sudoku_grid[r1][col] && sudoku_grid[r1][col] == sudoku_grid[r2][col]){
						solv = false;
						return;
					}

					// If any of the two positions is set to d, the other can't be set to d

					if(sudoku_grid[r1][col]==d){
						// Generate the validity clause
						std::vector<int> val_clause{
							-get_var_id(r2,col,d,n)
						};

						// Add the validity clause
						clauses.push_back(val_clause);
						continue;
					}

					if(sudoku_grid[r2][col]==d){
						// Generate the validity clause
						std::vector<int> val_clause{
							-get_var_id(r1,col,d,n)
						};

						// Add the validity clause
						clauses.push_back(val_clause);
						continue;
					}

					// case when no one is set to d

					// Generate the validity clause
					std::vector<int> val_clause{
						-get_var_id(r1,col,d,n),
						-get_var_id(r2,col,d,n)
					};

					// Add the validity clause
					clauses.push_back(val_clause);

				}
			}
		}
	}

}

void get_validity_clauses_blocks(int n, std::vector< std::vector<int> > & clauses, 
													std::vector< std::vector<int> > & sudoku_grid, bool & solv){
	
	int N2 = n * n;

	for(int block_r=0; block_r < N2; block_r+=n){ // choose the row in the top-left-most position in block
		for(int block_c=0; block_c < N2; block_c+=n){ // choose the column in the top-left-most position in block

			for(int d=1; d <= N2; d++){
				// iterate over the positions in the block
				// pairwise compare them. They can't have the same digit d
				for(int position1 = 0; position1 < N2-1; position1++){
					for(int position2 = position1+1; position2 < N2; position2++){
						
						// get rows and columns from positions number
						int r1 = position1/n, c1 = position1%n,
							r2 = position2/n, c2 = position2%n;

						// if they have the same digit, the puzzle is not solvable
						if(sudoku_grid[r1][c1] && sudoku_grid[r1][c1] == sudoku_grid[r2][c2]){
							solv = false;
							return;
						}

						// If any of the two positions is set to d, the other can't be set to d

						if(sudoku_grid[r1][c1]==d){
							// Generate the validity clause
							std::vector<int> val_clause{
								-get_var_id(r2,c2,d,n)
							};

							// Add the validity clause
							clauses.push_back(val_clause);
							continue;
						}

						if(sudoku_grid[r2][c2]==d){
							// Generate the validity clause
							std::vector<int> val_clause{
								-get_var_id(r1,c1,d,n)
							};

							// Add the validity clause
							clauses.push_back(val_clause);
							continue;
						}

						// case when no one is set to d

						// Generate the validity clause
						std::vector<int> val_clause{
							-get_var_id(r1,c1,d,n),
							-get_var_id(r2,c2,d,n)
						};

						// Add the validity clause
						clauses.push_back(val_clause);

					}
				}
			}
			
		} 
	}

}

void get_validity_clauses(int n, std::vector< std::vector<int> > & clauses, 
													std::vector< std::vector<int> > & sudoku_grid, bool & solv){
	
	get_validity_clauses_rows(n, clauses, sudoku_grid, solv);

	if(solv) get_validity_clauses_columns(n, clauses, sudoku_grid, solv);

	if(solv) get_validity_clauses_blocks(n, clauses, sudoku_grid, solv);

	return;
}

void print_sat_conflict(){

	std::cout << "c This cnf is not satisfiable\n";
	std::cout << "p cnf 1 2\n";
	std::cout << "1 0 -1\n"; // p ^ !p == false

}

int get_number_of_vars(std::vector< std::vector<int> > & clauses){

	std::unordered_map<int, int> count_vars;

	for(auto clause : clauses){
		for(int var : clause){
			count_vars[var]++;
		}
	}

	return (int) count_vars.size();
}

void print_sat_instance(std::vector< std::vector<int> > & clauses){

	// avoid repeated clauses
	std::sort(clauses.begin(), clauses.end());
	clauses.resize(std::distance(clauses.begin(),std::unique(clauses.begin(), clauses.end())));

	int number_of_vars = get_number_of_vars(clauses),
		number_of_clauses = clauses.size();

	std::cout << "c This cnf is traducted from an sudoku instance. \n";
	std::cout << "p cnf " << number_of_vars << " " << number_of_clauses << "\n";

	for(auto clause : clauses){
		
		for(auto var : clause){
			std::cout << var << " ";
		}

		std::cout << "0\n";
	}

	std::cout << "p cnf " << number_of_vars << " " << number_of_clauses << "\n";
}

void reduce_sudoku_to_SAT(){

	int n;
	bool solvable = true;
	std::vector< std::vector<int> > sudoku_grid;
	std::vector< std::vector<int> > clauses;

	get_sudoku_input(n, sudoku_grid);

	get_completeness_clauses(n, clauses, sudoku_grid);

	get_uniqueness_clauses(n, clauses, sudoku_grid);

	get_validity_clauses(n, clauses, sudoku_grid, solvable);

	if(!solvable){ 
		print_sat_conflict();
		return;
	}

	print_sat_instance(clauses);

	return;
}


int main(){
	reduce_sudoku_to_SAT();
	return 0;
}