#include <iostream>
#include <cstdio>
#include "sat_solver.hpp"

int main(int argc, char **argv) {

    if (argc != 1 && argc != 3) {
        std::cout << "Wrong Usage\nCorrect usage: (1) ./sat <inputFile> <outputFile>\n(2) ./sat";
        return EXIT_FAILURE;
    }

    if (argc == 3) {
        std::freopen(argv[1],"r",stdout);
        std::freopen(argv[2],"a",stdout);
    }

    SATSolver *SAT = new SATSolver();
    SAT->ReadExpression();
    SAT->SolveExpression();
    SAT->PrintResult();

}