# SAT-based-sudoku-solver
A SAT based Sudoku solver implemented in c++ for the subject "Algorithms Design I" at Simon Bolivar University

En este proyecto se implemento un resolvedor de Sudoku, basado en un resolvedor de SAT (el problema de satisfacibilidad booleana). En otras palabras se hizo una reduccion del problema del Sudoku al problema SAT. 

## Descripcion del problema del Sudoku:

Dado un número entero positivo N, un tablero de Sudoku de orden N es una matrix de N x N filas y
N x N columnas. La matriz está dividida en N x N secciones disjuntas, cada una una matriz de tamaño
N × N. Cada celda puede contener a lo sumo un número entre 1 y N x N. La idea del Sudoku es llenar el tablero completo con numeros entre 1 y N x N cumpliendo las siguientes reglas:

- En una fila no se pueden repetir numeros. Es decir, cada numero del 1 al N x N aparece exactamente una vez en cada fila del tablero.
- En una columna no se pueden repetir numeros. Es decir, cada numero del 1 al N x N aparece exactamente una vez en cada columna del tablero.
- En un bloque (submatriz N x N) no se pueden repetir numeros. Es decir, cada numero del 1 al N x N aparece exactamente una vez en cada bloque del tablero.

Para una descripcion mas detallada consulte: [Sudoku](https://en.wikipedia.org/wiki/Sudoku)

## Descripcion de SAT:

Dada una fórmula en lógica proposicional φ, el Problema de Satisfacibilidad Booleana o SAT
consta de intentar hallar una asignación de verdad para las variables en φ, que hagan que dicha
fórmula sea satisfecha (evalúe a true).

Para una descripcion mas detallada consulte: [SAT](https://en.wikipedia.org/wiki/Boolean_satisfiability_problem)

## Reduccion de Sudoku a SAT:

Es posible reducir cualquier instancia de Sudoku a SAT (Problema de Satisfacibilidad Booleana),
representando el estado del tablero con variables y cláusulas de una fórmula en CNF (Forma
Normal Conjuntiva).

Para representar un tablero de Sudoku de orden N , tendremos variables con forma x(i,j,d) , que serán
verdaderas si la casilla (i, j) del tablero tiene el número d. Existirán N 3 variables de este tipo.

Para modelar las restricciones del Sudoku usaremos 3 tipos de clausulas:

- Clausulas de completitud: Una solución para un tablero de Sudoku no debe tener posiciones vacías, por lo que en cada posición del tablero debe haber alguno de los números disponibles. Existiran N x N clausulas de este tipo (Una por cada posicion del tablero): (x(i,j,1) || x(i,j,2) || ...)

- Clausulas de Unicidad: Una solución para un tablero de Sudoku no debe tener dos números en una misma
posición del tablero. Entonces sean d y d' dos digitos entre 1 y N x N, para todas las posiciones del tablero se agrega una clausula de la forma: (!x(i,j,d) || !x(i,j,d'))

- Clausulas de Validez: Una solución para un tablero de Sudoku no debe tener el mismo valor dos o más veces en una misma fila, columna o sección. Sea S un conjunto de índices representando una fila, columna o sección del tablero, s, s' pertenecientes a S y d un digito, se agregan las clausulas de tipo: (!x(si,sj,d) || !x(si',sj',d))

Ademas se agregan clausulas unitarias de la forma x(i,j,d) para cada posicion con valor inicial d.

## Estructura:

El proyecto cuanta con tres modulos principales:

- Un programa que traduce instancias de Sudoku a instancias de SAT.

- Un resolvedor de SAT.

- Un programa que traduce soluciones de SAT a soluciones de Sudoku.

Ademas cuenta con un script que orquesta los distintos modulos.

## Traductor de Sudoku a SAT: