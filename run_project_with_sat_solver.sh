#!/bin/bash
input="InstanciasSudoku.txt"
i=0
echo "Building project with make command..."
make
echo "Executing with sat_solver solver..."
if test -f "SolucionesSudoku_sat_solver.txt"; then
  rm SolucionesSudoku_sat_solver.txt;
fi
while IFS= read -r sudokuInstance
do
  echo "";
  echo "Instacia #$i:";
  ((i=i+1))
  echo "$sudokuInstance" | ./reduce_sudoku_to_SAT > cnf_file_sat_solver.txt;
  time (timeout 2s ./sat_solver cnf_file_sat_solver.txt > sat_sol_sat_solver.txt; status=$?;)
  if [[ $status -ne 0 ]]; then
  	echo "s cnf -1 0" > sat_sol_sat_solver.txt;
  fi
  ./retrieve_sudoku_from_SAT sat_sol_sat_solver.txt >> SolucionesSudoku_sat_solver.txt;
done < "$input" &> "times_sat_solver.txt"
echo "Solutions are in SolucionesSudoku_sat_solver.txt file"
echo "Time measurements are in times_sat_solver.txt file"
rm cnf_file_sat_solver.txt sat_sol_sat_solver.txt