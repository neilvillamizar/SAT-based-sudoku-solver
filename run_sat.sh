#!/bin/bash

  mkdir -p log
  input="test/InstanciasSudoku.txt"
  TIMEFORMAT='%E'

  echo "Building project with make command..."
  make
  echo "Executing with sat_solver solver..."

  if test -f "log/solutions_log_sat.txt"; then
    rm solutions_log_sat.txt
  fi

  while IFS= read -r sudokuInstance

    do
      echo "$sudokuInstance" | ./sud-sat > sat_input;
      time timeout 2s ./sat_solver < sat_input > sat_output; 
      status=$?;

      if [[ $status -ne 0 ]]; then
        echo "s cnf -1 0" > sat_output 
      fi

      ./sat-sud  < sat_output >> log/solutions_log_sat.txt;
  
    done < $input &> "log/time_log_sat.csv"

  echo "Solutions are in log/solutions_log_sat.txt file"
  echo "Time measurements are in log/time_log_sat.csv file"
  rm sat_input sat_output 

