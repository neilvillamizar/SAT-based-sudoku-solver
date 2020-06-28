

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <cstdlib>


class retrieveSudokuFromSAT {

public:
	//retrieveSudokuFromSAT();
	//~retrieveSudokuFromSAT();

	void retrieveSudokuFromSAT_(){

		int sat_result = getSatOutput();

		if(sat_result == -1){
			std::cout << "Time Limit Exceeded.\n";
			return;
		}

		if(sat_result == 0){
			std::cout << "Unsatisfiable.\n";
			return;
		}

		setSudokuStr();

		std::cout << sudoku_order << " " << sudoku_str << std::endl;

	}
	
private:

	std::string sudoku_str;
	int sudoku_order;
	int square_sud_order;
	std::vector<int> sat_variables_v;



	void setSudokuOrder(int number_of_vars){
		
		for (int i = 0; i*i*i*i*i*i <= number_of_vars; ++i) {
			sudoku_order = i;
		}

		square_sud_order = sudoku_order * sudoku_order;

		try{
			assert(
				square_sud_order*square_sud_order*square_sud_order == number_of_vars
			);
		}catch(...){
			throw "The number of SAT variables doesn't match with a sudoku instance";
		}

	}

	int getDigit(int var){
		
		return (var-1) % square_sud_order + 1;

	}

	std::pair<int, int> getCoordinates(int var){
		
		int sud_ord_to_4th = square_sud_order * square_sud_order;
		
		return {
			(var-1) / sud_ord_to_4th,
			((var-1) % sud_ord_to_4th)/square_sud_order
		};

	}

	int getPositionInSudokuStr(int var){
		
		std::pair<int, int> sudoku_coordinates = getCoordinates(var);
		int i = sudoku_coordinates.first, j = sudoku_coordinates.second;

		return i * square_sud_order + j;

	}

	int getSatOutput(){

		int sat_result, 
		number_of_vars,
		sat_variable;
		char start_code;
		std::string cnf;

		while(std::cin >> start_code){
			
			if(start_code == 'c'){
				std::string line;
				getline(std::cin, line);
				continue;
			}

			try{
				assert(start_code == 's');
			}catch(...){
				throw " getSatOutput: Format error.";
			}

			std::cin >> cnf >> sat_result >> number_of_vars;

			break;

		}

		try{
			assert(-1 <= sat_result && sat_result <= 1 && number_of_vars >= 0);
		}catch(...){
			throw " getSatOutput: Format error.";
		}

		if(sat_result != 1){
			return sat_result;
		}

		setSudokuOrder(number_of_vars);

		for (int i = 0; i < number_of_vars; ++i) {
			
			std::cin >> start_code >> sat_variable;

			try{
				assert( std::abs(sat_variable) <= number_of_vars && start_code == 'v');
			}catch(...){
				throw " getSatOutput: Format error.";
			}

			if(sat_variable < 0) continue;

			sat_variables_v.push_back(sat_variable);

		}

		return sat_result;
	}

	void setSudokuStr(){
		
		int sud_ord_to_4th = square_sud_order * square_sud_order;

		sudoku_str.resize(sud_ord_to_4th, '0');

		try{
			assert(sud_ord_to_4th == (int) sat_variables_v.size());
		}catch(...){
			throw " The number of variables set to true doesn't match with a sudoku instance.";
		}

		for(int var : sat_variables_v){
			sudoku_str[getPositionInSudokuStr(var)] = getDigit(var)  + '0';
		}
		
	}

};


int main(int argc, char const *argv[]){

	if(argc > 2){
		std::cerr << "USAGE:\n./retrieve_sudoku_from_SAT < file_name\n./retrieve_sudoku_from_SAT <file_name>\n";
		return 0;
	}

	if(argc == 2){
		//std::cout << "reading from: "<< (std::string) argv[1] << std::endl;
		std::freopen(argv[1],"r",stdin);
	}

	retrieveSudokuFromSAT retriever;
	retriever.retrieveSudokuFromSAT_();

	return 0;
}