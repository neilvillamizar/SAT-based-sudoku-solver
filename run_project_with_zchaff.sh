#!/bin/bash
input="InstanciasSudoku.txt"
i=0
echo "Building project with make command..."
make
echo "Executing with zchaff solver..."
if test -f "SolucionesSudoku_zchaff.txt"; then
  rm SolucionesSudoku_zchaff.txt;
fi
while IFS= read -r sudokuInstance
do
  echo "";
  echo "Instacia #$i:";
  ((i=i+1))
  echo "$sudokuInstance" | ./reduce_sudoku_to_SAT > cnf_file.txt;
  time (timeout 2s ./zchaff cnf_file.txt > sat_sol.txt; status=$?;)
  if [[ $status -ne 0 ]]; then
  	echo "s cnf -1 0" > sat_sol.txt;
  fi
  ./retrieve_sudoku_from_SAT sat_sol.txt >> SolucionesSudoku_zchaff.txt;
done < "$input" &> "times_zchaff.txt"
echo "Solutions are in SolucionesSudoku_zchaff.txt file"
echo "Time measurements are in times_zchaff.txt file"
rm cnf_file.txt sat_sol.txt