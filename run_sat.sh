#!/bin/bash
input="Test/InstanciasSudoku.txt"
i=0
echo "Building project with make command..."
make
echo "Executing with sat_solver solver..."
if test -f "solutions_log_sat.txt"; then
  rm solutions_log_sat.txt
fi
while IFS= read -r sudokuInstance
do
  echo "$sudokuInstance" | ./reduce_sudoku_to_SAT > sat_input.txt;
  time (timeout 2s ./sat_solver < sat_input.txt > sat_output.txt; status=$?;)
  if [[ $status -ne 0 ]]; then
  	echo "s cnf -1 0" > sat_output.txt;
  fi
  ./retrieve_sudoku_from_SAT sat_output.txt >> solutions_log_sat.txt;
done < "$input" &> "time_log_sat.txt"
cat time_log_zchaff.txt | grep real | cut -f 2 > time_log_sat.txt
echo "Solutions are in solutions_log_sat.txt file"
echo "Time measurements are in time_log_sat.txt file"
rm sat_input.txt sat_output.txt

