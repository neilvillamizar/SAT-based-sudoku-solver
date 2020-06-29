# SAT-based-sudoku-solver
A SAT based Sudoku solver implemented in c++ for the subject "Algorithms Design I" at Simon Bolivar University


# IMPORTANTE
Si estas leyendo esto, significa que no nos dio chance de terminar el proyecto y lo que teníamos fue pulleado :(
Existe una versión de nuestro sat solver pero que no funciona correctamente
Además, la documentación y las pruebas no han sido terminadas
Estamos trabajando en completarlo y esperamos que esa nueva version pueda ser corregida :(

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
verdaderas si la casilla (i, j) del tablero tiene el número d. Existirán N 3 variables de este tipo. Cada variable tendra un entero que la identifica. Este se obtiene a traves del metodo 'getVarId' que usa la biyeccion i * N^4 + j * N^2 + d.

Para modelar las restricciones del Sudoku usaremos 3 tipos de clausulas:

- Clausulas de completitud: Una solución para un tablero de Sudoku no debe tener posiciones vacías, por lo que en cada posición del tablero debe haber alguno de los números disponibles. Existiran N x N clausulas de este tipo (Una por cada posicion del tablero): (x(i,j,1) || x(i,j,2) || ...)

- Clausulas de Unicidad: Una solución para un tablero de Sudoku no debe tener dos números en una misma
posición del tablero. Entonces sean d y d' dos digitos entre 1 y N x N, para todas las posiciones del tablero se agrega una clausula de la forma: (!x(i,j,d) || !x(i,j,d'))

- Clausulas de Validez: Una solución para un tablero de Sudoku no debe tener el mismo valor dos o más veces en una misma fila, columna o sección. Sea S un conjunto de índices representando una fila, columna o sección del tablero, s, s' pertenecientes a S y d un digito, se agregan las clausulas de tipo: (!x(si,sj,d) || !x(si',sj',d))

Ademas se agregan clausulas unitarias de la forma (x(i,j,d)) para cada posicion con valor inicial d.

## Estructura:

El proyecto cuanta con tres modulos principales:

- Un programa que traduce instancias de Sudoku a instancias de SAT.

- Un resolvedor de SAT.

- Un programa que traduce soluciones de SAT a soluciones de Sudoku.

Ademas cuenta con un script que orquesta los distintos modulos.

## Traductor de Sudoku a SAT:

El traductor se implemento en el archivo reduce_sudoku_to_SAT.cpp en una clase llamda 'reduceSudokuToSAT'. Para probarlo de forma individual usar:
	./reduce_sudoku_to_SAT archivo_sudoku.txt

Tambien se le puede pasar la instancia de sudoku por standard input. El programa crea una instancia de reduceSudokuToSAT y llama a la funcion principal de la clase, reduceSudokuToSAT_, la cual realiza la traduccion a traves de los demas metodos privados de la clase. Primero se llama al metodo getSudokuInput, el cual lee el orden del sudoku 'sudoku_order' (el cual denotaremos por N a partir de ahora) y el string que representa el estado inicial del sudoku de tamaño N^4. Ademas llena una matriz de tamaño N^2 x N^2 'sudoku_grid' que representa el tablero del sudoku en su estado inicial con los digitos indicados por el string. Este metodo toma tiempo lineal en el input, analogamente usa memoria lineal en el input (O(N^4)). 

Luego se procede a generar las clausulas de la instancia de SAT. Una clausula se guarda como un vector de enteros, donde cada entero es el identificador que representa una variable. Todas las clausulas generadas se almacenan en un vector de vectores de enteros 'clauses'.

Primero se llama al metodo getUnitClauses, el cual genera las clausulas de unitarias correspondiantes a las posiciones del tablero que inicialmente ya tienen algun valor. Estas clausulas unitarias son de la forma (x(i,j,d)) para cada posicion con valor inicial d.

A continuacion el metodo getCompletenessClauses genera las clausulas que garantizan que todas las celdas del tablero de Sudoku seran llenadas con algun numero entre 1 y N^2. De las N^2 clausulas que existen de este tipo no se agregan las que corresponden a una posicion ya llena del tablero inicial ya que estas se satisfacen desde el inicio al estar ya ocupada dicha posicion.

Luego se llama al metodo getUniquenessClauses, el cual se encarga de añadir las clausulas que aseguran que para una misma casilla no existiran dos variables que la representen con distintos digitos con valor True ambas. Entonces sean d y d' dos digitos entre 1 y N x N, para todas las posiciones del tablero se agrega una clausula de la forma: (!x(i,j,d) || !x(i,j,d')). Si en la casilla (i,j) esta desde el estado incial del tablero cualquiera de los digitos d o d' entonces en su lugar se agrega una clausula  unitaria con la variable que representa al digito que no esta en la casilla negada.

Despues el metodo getValidityClauses, genera las clausulas que garantizan que la solucion a SAT generada por el solucionador de SAT cumplirá con las reglas de validez definidas para sudoku. Este metodo llama a otros tres metodos, cada uno de los cuales se encarga de añadir las clausulas que aseguran el cumplimiento de una de las reglas del Sudoku. getValidityClausesRows genera las clasulas para evitar que un numero se repita en una fila del tablero. getValidityClausesColumns hace lo propio con las columnas del tablero. Y getValidityClausesBlocks por su parte, funciona de forma analoga pero con los bloques de tamaño N^2. Sea S un conjunto de índices representando una fila, columna o sección del tablero, s, s' pertenecientes a S y d un digito, se agregan las clausulas de tipo: (!x(si,sj,d) || !x(si',sj',d)). En caso de que una de las dos variables ya se cumpla solo se agrega la clausua con la otra variable.

Finalmente se llama a el metodo printSatInstance, el cual toma las clausulas generadas por los metodos anteriores y las imprime por el standard output, siguiendo el formato especificado. Hasta aquí, todo el proceso toma tiempo lineal con respecto a la salida, ya que primero genera todas las clausulas y luego las imprime. Pero al generar las clausulas, en los casos donde solo añade una variable y no las dos de una clausula de tamaño 2, ya que una vale True y ocurre negada, puede darse que se añada la misma clausula dos veces. Para evitar esto, se ordenan las clausulas y se eliminan las repetidas. Sea M la suma de los tamaños de las clausulas, entonces la complejidad en tiempo en total queda en O(MlogM). Se podrian dejar estas clausulas repetidas pero se prefirio ahorrarle trabajo al resolvedor de SAT, ya que este toma mucho mas tiempo ya que SAT es un problema NP-complete. En cuanto al espacio usado, el traductor de Sudoku a SAT, toma espacio O(M).

## Resolvedor de SAT:



## Traductor de SAT a Sudoku:

El traductor se implemento en el archivo retrieve_sudoku_from_SAT.cpp en una clase llamda 'retrieveSudokuFromSAT'. Para probarlo de forma individual usar:
	./retrieve_sudoku_from_SAT archivo_sudoku.txt

Tambien se le puede pasar la instancia de SAT por standard input. El programa crea una instancia de retrieveSudokuFromSAT y llama a la funcion principal de la clase, retrieveSudokuFromSAT_, la cual realiza la traduccion a traves de los demas metodos privados de la clase. Primero se llama al metodo 'getSatOutput', el cual lee del input la solucion a una instancia SAT que luego sera traducida a una solucion de Sudoku. Este metodo ignora todas las lineas del input que sean comentarios hasta que llega al encabezado. Del encabezado consigue el resultado concreto (satisfacible, no satisfacible o tiempo limite excedido) y el numero de variables en la instancia de SAT. Usando el numero de variables del SAT se calcula el orden del Sudoku (N), ya que el numero de variables debe ser igual a N^6. Posteriormente se leen las variables de la solucion a SAT y se ignoran las que fueron asignadas False. Las que les fue asignado True corresponden a las N^4 posiciones del tablero, y señalan que numero quedo en cada casilla. Estas son guardadas en un vector de enteros 'sat_variables_v'. 

Posteriormente se llama a la funcion 'setSudokuStr' la cual se encarga de crear el string que representa la solucion al sudoku. Para esto se recorren las variables en 'sat_variables_v', y para cada una se se calcula la posicion en el string que le toca mediante la funcion 'getPositionInSudokuStr' y el numero que representa usando 'getDigit'. Se le asigna el caracter correspondiente al entero retornado por 'getDigit' a la posicion dada por 'getPositionInSudokuStr'. 

Finalmente se imprime por salida estándar el orden del sudoku y el string que representa la solucion a la instancia de sudoku. En getSatOutput solo se lee el input, entonces es lineal en el input, por lo que toma O(N^6) (N^6 es la cantidad e variables en el input) en tiempo, y O(N^4) en espacio ya  que solo se guardan las variables con valor True. Luego el metodo setSudokuStr toma O(N^4) ya que solo recorre el vector de las variables guardadas y tanto getPositionInSudokuStr como getDigit toman tiempo constante. En espacio, analogamente usa O(N^4) por el string y el vector de variables. En total este traductor toma tiempo acotado por el input: O(N^6), y memoria por el tamaño de la solucion al sudoku.

## Scripts para correr el proyecto:

El proyecto cuenta con dos scripts en bash para correrlo:

- run_project_with_sat_solver.sh : corre el proyecto usando el solucionador propio del proyecto. Para correrlo:
	
	./run_project_with_sat_solver.sh

- run_project_with_zchaff.sh : corre el proyecto usando el solucionador zchaff. Para correrlo:
	
	./run_project_with_zchaff.sh

Ambos scripts funcionan de forma analoga. Primero compilan los programas involucrados en el proyecto llamando al makefile del proyecto. Luego, para cada linea en el archivo de instancias de sudokus, traducen la instancia a SAT con el primer traductor, luego resuelven la instancia de SAT con el resolverdor de SAT del script, y finalmente traducen la solucion a Sudoku. Las soluciones van al archivo SolucionesSudoku_sat_solver.txt (o SolucionesSudoku_zchaff.txt segun sea el caso) mientras las mediciones de tiempo van al archivo times_sat_sover.txt (o times_zchaff.txt). El Time Limit usado para ambos casos es de 2 segundos.


## Instancias Resueltas:



## Tiempos de Ejecucion: 

