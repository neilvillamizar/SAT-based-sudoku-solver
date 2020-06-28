

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <cstdlib>

// A class to retrieve a sudoku solution from a SAT solution
class retrieveSudokuFromSAT {

public:

	// Main function of the class
	// retrieve a sudoku solution from a SAT solution
	void retrieveSudokuFromSAT_(){

		// get the SAT solution 
		int sat_result = getSatOutput();

		// If the sat output is TLE then print a message a exit
		if(sat_result == -1){
			std::cout << "Time Limit Exceeded.\n";
			return;
		}

		// If the sat output is Unsatisfiable then print a message a exit
		if(sat_result == 0){
			std::cout << "Unsatisfiable.\n";
			return;
		}

		// Translate the SAT solution to a sudoku string format
		setSudokuStr();

		// Print the sudoku solution
		std::cout << sudoku_order << " " << sudoku_str << std::endl;

	}
	
private:

	std::string sudoku_str;				// The sudoku string representation of the solution
	int sudoku_order;					// The order of the sudoku instance
	int square_sud_order;				// The square of the sudoku order
	std::vector<int> sat_variables_v;	// The SAT variables set to True in the solution

	// Set the sudoku order given the number of variables in the SAT instance
	void setSudokuOrder(int number_of_vars){
		
		for (int i = 0; i*i*i*i*i*i <= number_of_vars; ++i) {
			sudoku_order = i;
		}

		square_sud_order = sudoku_order * sudoku_order;

		// Check that the number of SAT variables is the sudoku order raised to 6.
		try{
			assert(
				square_sud_order*square_sud_order*square_sud_order == number_of_vars
			);
		}catch(...){
			throw "The number of SAT variables doesn't match with a sudoku instance";
		}

	}

	// Given a variable id, returns the digit associated to it
	int getDigit(int var){
		
		return (var-1) % square_sud_order + 1;

	}

	// Given a variable id, returns the position in the sudoku board associated to it
	std::pair<int, int> getCoordinates(int var){
		
		int sud_ord_to_4th = square_sud_order * square_sud_order;
		
		return {
			(var-1) / sud_ord_to_4th,
			((var-1) % sud_ord_to_4th)/square_sud_order
		};

	}

	// Given a variable id, returns the position in sudoku string representation associated to it
	int getPositionInSudokuStr(int var){
		
		std::pair<int, int> sudoku_coordinates = getCoordinates(var);
		int i = sudoku_coordinates.first, j = sudoku_coordinates.second;

		return i * square_sud_order + j;

	}

	// Get the SAT solution
	int getSatOutput(){

		int sat_result, 				// The result of the SAT {SAT:1, UNSAT: 0, TLE:-1}
		number_of_vars,					// The total number od SAT variables
		sat_variable;					// Auxliar integer to read a SAT variable
		char start_code;				// Auxliar char to read a starting code at the beginning of input lines
		std::string cnf;				// Auxliar string to read the "cnf" word

		// Ignore all the comment lines until gets the header line
		while(std::cin >> start_code){
			
			// If is a comment, continue
			if(start_code == 'c'){
				std::string line;
				getline(std::cin, line);
				continue;
			}

			// check that the first line that is not a comment is the header
			try{
				assert(start_code == 's');
			}catch(...){
				throw " getSatOutput: Format error.";
			}

			// Get the header information and break the loop
			std::cin >> cnf >> sat_result >> number_of_vars;

			break;

		}

		// Check the header information
		try{
			assert(-1 <= sat_result && sat_result <= 1 && number_of_vars >= 0);
		}catch(...){
			throw " getSatOutput: Format error.";
		}

		// If the SAT instance is not satisfiable, then return
		// otherwise read the variables values in the SAT solution

		if(sat_result != 1){
			return sat_result;
		}

		setSudokuOrder(number_of_vars);

		// Read the state of the SAT variables in the solution
		for (int i = 0; i < number_of_vars; ++i) {
			
			std::cin >> start_code >> sat_variable;

			try{
				assert( std::abs(sat_variable) <= number_of_vars && start_code == 'v');
			}catch(...){
				throw " getSatOutput: Format error.";
			}

			// Ignore variables that are set to False
			if(sat_variable < 0) continue;

			// Store the variables set to True
			sat_variables_v.push_back(sat_variable);

		}

		return sat_result;
	}

	// Check if an integer belongs to [low, high]
	bool isInRange(int x, int low, int high){
		return low <= x && x <= high;
	}

	// Set the sudoku in the string format
	void setSudokuStr(){
		
		int sud_ord_to_4th = square_sud_order * square_sud_order;

		sudoku_str.resize(sud_ord_to_4th, '0');

		try{
			assert(sud_ord_to_4th == (int) sat_variables_v.size());
		}catch(...){
			throw " The number of variables set to true doesn't match with a sudoku instance.";
		}

		// For each variable set to True in the SAT solution
		// get it position in the sudoku string and the digit associated with it
		// And set that position with the character corresponding to its digit
		for(int var : sat_variables_v){

			int digit = getDigit(var);

			if(isInRange(digit,0,9))
				sudoku_str[getPositionInSudokuStr(var)] = digit  + '0';
			else if(isInRange(digit,10,35))
				sudoku_str[getPositionInSudokuStr(var)] = digit  + 'A' - 10;
			else if(digit == 36)
				sudoku_str[getPositionInSudokuStr(var)] = '.';
			else if(isInRange(digit,37,62))
				sudoku_str[getPositionInSudokuStr(var)] = digit + 'a' - 37;
			else if(digit == 63)
				sudoku_str[getPositionInSudokuStr(var)] = '#';
			else if(digit == 64)
				sudoku_str[getPositionInSudokuStr(var)] = '@';

		}
		
	}

};


int main(int argc, char const *argv[]){

	// Check the number of command line arguments is ok
	if(argc > 2){
		std::cerr << "USAGE:\n./retrieve_sudoku_from_SAT < file_name\n./retrieve_sudoku_from_SAT <file_name>\n";
		return 0;
	}

	// If a file is given then redirect the std input buffer to that file
	if(argc == 2){
		std::freopen(argv[1],"r",stdin);
	}

	// Create an intance of the retrieveSudokuFromSAT class
	// and retrieve a sudoku solution from a SAT solution
	retrieveSudokuFromSAT retriever;
	retriever.retrieveSudokuFromSAT_();

	return 0;
}