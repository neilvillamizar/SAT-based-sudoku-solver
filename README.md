# SAt-based sudoku solver

En este proyecto se implementó un resolvedor de instancias de Sudoku, basado en un resolvedor de SAT (el problema de satisfacibilidad booleana) en el lenguaje C++.

## Descripción del problema del Sudoku:

Dado un número entero positivo N, un tablero de Sudoku de orden N es una matrix de NxN filas y
NxN columnas. La matriz está dividida en NxN secciones disjuntas. Cada una de estas es una matriz de tamaño
N×N. Cada celda puede contener a lo sumo un número entre 1 y NxN. La idea del Sudoku es llenar el tablero completo con números entre 1 y NxN cumpliendo las siguientes reglas:

- En una fila no se pueden repetir números. Es decir, cada número del 1 al NxN aparece exactamente una vez en cada fila del tablero.
- En una columna no se pueden repetir números. Es decir, cada número del 1 al NxN aparece exactamente una vez en cada columna del tablero.
- En un bloque (submatriz NxN) no se pueden repetir números. Es decir, cada número del 1 al NxN aparece exactamente una vez en cada bloque del tablero.

Para una descripcion mas detallada consulte: [Sudoku](https://en.wikipedia.org/wiki/Sudoku)

## Descripcion de SAT:

Dada una fórmula en lógica proposicional φ, el Problema de Satisfacibilidad Booleana o SAT
consta de intentar hallar una asignación para las variables en φ, que hagan que dicha
fórmula sea satisfecha (evalúe a *true*).

Para una descripcion mas detallada consulte: [SAT](https://en.wikipedia.org/wiki/Boolean_satisfiability_problem)

## Reduccion de Sudoku a SAT:

Es posible reducir cualquier instancia de Sudoku a SAT (Problema de Satisfacibilidad Booleana),
representando el estado del tablero con variables y cláusulas de una fórmula en CNF (Forma
Normal Conjuntiva).

Para representar un tablero de Sudoku de orden N , tendremos variables con forma x(i,j,d) , que serán
verdaderas si la casilla (i, j) del tablero tiene el número d. Existirán N^3 variables de este tipo. Cada variable tendrá un entero que la identifica. Este se obtiene a través del método 'getVarId' que usa la biyeccion i * N^4 + j * N^2 + d.

Para modelar las restricciones del Sudoku usaremos 3 tipos de cláusulas:

- Cláusulas de completitud: Una solución para un tablero de Sudoku no debe tener posiciones vacías, por lo que en cada posición del tablero debe haber alguno de los números disponibles. Existiran NxN cláusulas de este tipo (Una por cada posición del tablero): (x(i,j,1) || x(i,j,2) || ...)

- Cláusulas de Unicidad: Una solución para un tablero de Sudoku no debe tener dos números en una misma
posición del tablero. Entonces sean d y d' dos dígitos entre 1 y NxN, para todas las posiciones del tablero se agrega una cláusula de la forma: (!x(i,j,d) || !x(i,j,d'))

- Cláusulas de Validez: Una solución para un tablero de Sudoku no debe tener el mismo valor dos o más veces en una misma fila, columna o sección. Sea S un conjunto de índices representando una fila, columna o sección del tablero, s, s' pertenecientes a S y d un digito, se agregan las cláusulas de tipo: (!x(si,sj,d) || !x(si',sj',d))

Además, se agregan cláusulas unitarias de la forma (x(i,j,d)) para cada posicion con valor inicial d.

## Estructura:

El proyecto cuanta con tres modulos principales:

- Un programa que traduce instancias de Sudoku a instancias de SAT.

- Un resolvedor de SAT.

- Un programa que traduce soluciónes de SAT a soluciónes de Sudoku.

Ademas cuenta con un script que orquesta los distintos modulos.

## Traductor de Sudoku a SAT:

El traductor se implemento en el archivo `reduce_sudoku_to_SAT.cpp` en una clase llamda *reduceSudokuToSAT*. Para probarlo de forma individual, se debe ejecutar:

`./sud-sat archivo_sudoku.txt`

EL programa tambien puede recibir una instancia a través del standard input. 

El programa creará una instancia de *reduceSudokuToSAT* y llamará a la función principal de la clase, *reduceSudokuToSAT_*, la cual realiza la traducción a través de los demás métodos privados de la clase. Primero se llama al método *getSudokuInput*, el cual lee el orden del sudoku (denotado por N a partir de ahora) y el string que representa el estado inicial del sudoku de tamaño N^4. Además llena una matriz de tamaño N^2 x N^2 que representa el tablero del sudoku en su estado inicial con los dígitos indicados por el string. Este método toma tiempo lineal en el input. Análogamente, usa memoria lineal en el input (O(N^4)). 

Luego se procede a generar las cláusulas de la instancia de SAT. Una cláusula se guarda como un vector de enteros, donde cada entero es el identificador que representa una variable. Todas las cláusulas generadas se almacenan en un vector de vectores de enteros 'clauses'.

Primero se llama al método *getUnitClauses*, el cuál genera las cláusulas unitarias correspondiantes a las posiciones del tablero que inicialmente ya tienen algún valor. Estas cláusulas unitarias son de la forma (x(i,j,d)) para cada posición con valor inicial d.

Seguidamente, el método *getCompletenessClauses* genera las cláusulas que garantizan que todas las celdas del tablero de Sudoku seran llenadas con algún número entre 1 y N^2. De las N^2 cláusulas que existen de este tipo no se agregan las que corresponden a una posición ya llena del tablero inicial, ya que estas se satisfacen desde el inicio al estar ya ocupada dicha posición.

Luego se llama al método *getUniquenessClauses*, el cuál se encarga de añadir las cláusulas que aseguran que para una misma casilla no existirán dos variables que la representen con distintos dígitos con valor True. Entonces sean d y d' dos dígitos entre 1 y NxN, para todas las posiciones del tablero se agrega una cláusula de la forma: (!x(i,j,d) || !x(i,j,d')). Si en la casilla (i,j) está desde el estado incial del tablero cualquiera de los dígitos d o d', entonces en su lugar se agrega una cláusula unitaria con la variable que representa al dígito que no está en la casilla negada.

Después, el método *getValidityClauses*, genera las cláusulas que garantizan que la solución a SAT generada por el solucionador de SAT cumplirá con las reglas de validez definidas para sudoku. Este método llama a otros tres métodos, cada uno de los cuales se encarga de añadir las cláusulas que aseguran el cumplimiento de una de las reglas del Sudoku. *getValidityClausesRows* genera las clásulas para evitar que un número se repita en una fila del tablero. *getValidityClausesColumns* hace lo propio con las columnas del tablero. Por último, *getValidityClausesBlocks* funcióna de forma análoga pero con los bloques de tamaño N^2. Sea S un conjunto de índices representando una fila, columna o sección del tablero, s, s' pertenecientes a S y d un dígito, se agregan las cláusulas de tipo: (!x(si,sj,d) || !x(si',sj',d)). En caso de que una de las dos variables ya se cumpla sólo se agrega la clausua con la otra variable.

Finalmente se llama a el método *printSatInstance*, el cual toma las cláusulas generadas por los métodos anteriores y las imprime por el standard output, siguiendo el formato especificado. Hasta aquí, todo el proceso toma tiempo lineal con respecto a la salida, ya que primero genera todas las cláusulas y luego las imprime. Al generar las cláusulas, en los casos donde solo se añade una variable y no las dos de una cláusula de tamaño 2,  puede suceder que esta se repita. Para evitar esto, se ordenan las cláusulas y se eliminan las repetidas. Sea M la suma de los tamaños de las cláusulas, entonces la complejidad en tiempo en total queda en O(MlogM). En cuanto al espacio usado, el traductor de Sudoku a SAT, toma espacio O(M).

## Resolvedor de SAT:

La implementación del resolvedor de SAT puede encontrarse en los archivos `main_sat.cpp` y `sat_solver.cpp`

El enfoque que tomamos para resolver el problema, consiste en un backtracking iterativo con ciertas optimizaciones para evitar cálculos innecesarios. 

#### Las estructuras de datos

Las descripción general de las principales estructuras de datos es la siguiente:

- *Assignment*: par int, bool que representa la asignación de un booleano a una variable
- *Structure Decision*: estructura que almacenará la información necesaria para recuperar un estado del backtracking.
- *Structure Clause*: estructura que representa una cláusula
- *stack&lt;Decision&gt;* decisions: Pila del backtracking
- *vector&lt;Clause&gt;* clauses: Vector que contiene las distintas cáusulas de la expresión
- *vector&lt;bool&gt;* decided: Vector que aloja un booleano que especifíca si una variable ha sido asignada o no
- *vector&lt;bool&gt;* value: Vector que almacena los valores de las variables
- *queue&lt;Assignment&gt;* assignments: colas de variables asignadas esperando por ser propagadas
- *Matrix&lt;int&gt;* watched: estructura que, para cada variable, almacena los índices de las cláusulas que vigila.

#### Métodos

Una descripción general de los métodos más importantes implementados es la siguiente:

- En primer lugar, el método *IsSat()* de la clase SatSolver, se encarga de la ejecución del ciclo principal, que enlaza las distintas piezas del backtracing.
- El método *RemoveSingleVars* se encarga de hacer una reducción de las cláusulas unitarias que se encuentran al inicio de la entrada, ya que los literales que las componen deben evaluar a *true* para que la expresión sea satisfacible
- El método *Decide* se encarga de escoger la siguiente variable no asignada, proporcionarle un valor, y mantener esta decisión en la pila del backtracking, de manera que el estado pueda ser restituido.
- El método *PropagateDecision*, que se encarga de la propagación de todas las asignaciones pendientes. Esto incluye setear su valor, almacenar esta acción en el último estado actual, y llamar al método DeduceImplications para generar más asignaciones
- El método *DeduceImplications* se encarga de revisar todas las implicaciones que pudieron ser generadas a partir de la última asignación de una variable (esto es, reducir las cláusulas unitarias restantes)
- El método *ChangeDecision* se encarga de recuperar el estado en caso de conflicto

#### Enfoque y flujo del programa

La idea de nuestro enfoque es optimizar el algoritmo general del backtracking iterativo que sigue los siguientes pasos:

  1. Decidir una variable
  2. Si no hay más variables por decidir, la expresión se satisface
  3. Propagar la última decisión realizada, y chequear conflictos
  4. Si ocurre algún conflicto, intentar cambiar el valor de la última decisión. Si esto ya fue hecho, recuperar el estado previo.
  5. En caso de no haber conflictos, volver al paso 1

Las optimizaciones planteadas son las siguientes:

##### Unit propagation

La idea general de unit propagation es que, después de cada asignación de alguna variable, se chequea si alguna de las cláusulas se reduce a una cláusula de tamaño 1 (el resto de sus literales fue asignado en falso). De esta manera, la única posibilidad de que la expresión sea satisfacible es que este literal evalue verdadero, por lo que el valor de la variable queda implicado. 

##### Watched Literals

La idea de esta optimización es mantener para cada cláusula, dos literales que aún no se encuentren asignados, o que tengan el valor *true* asignado. De esta manera, cuando una asignación es realizada con el valor falso (estas son las asignaciones que pueden generar implicaciones), sólo debemos chequear las cláusulas donde esa variable sea una de las vigilantes de la cláusula, y seguir el siguiente algoritmo:

  1. Si puedo conseguir otra variable que no esté asignada en la cláusula, o cuyo valor sea verdadero, entonces esta nueva variable será uno de los vigilantes de la cláusula
  2. Si no existe tal variable, entonces podemos asegurarnos de que el otro vigilante debe evaluar verdadero para que la expresión sea satisfacible.
	
Lo interesante de esta técnica es que esta propiedad se mantiene en el backtracking, y no existe la necesidad de deshacer ningún cambio. Luego, esta técnica reduce un chequeo que pudo ser O(|C|x|V|), donde |C| es el número de cláusulas y |V| el número de variables, a una operación que puede tomar tiempo constante para cada cláusula que sea vigilada por una variable. Potencialmente, la complejidad de este chequeo sigue manteniéndose en O(|C|x|V|), ya que una variable puede vigilar a todas las cláusulas, y el reemplazo de esta puede ser caro. Pero en la práctica esta técnica ha sido realmente efectiva reduciendo el tiempo de ejecución de diferentes sat solvers.

Así, el flujo general del programa, es el siguiente:

En primer lugar, se reducen todas las cláusulas de tamaño uno, al recorrer la lista de cláusulas y mantener las asignaciones realizadas. Luego, se llama al método de propagación para generar las posibles implicaciones. La complejidad del método, sin considerar la llamada a la función de propagación, es líneal en el número de cláusulas (ya que solo se hace una serie de operaciones constantes por cada cláusula de tamaño uno).

Luego, si ningún conflicto fue encontrado en esta ejecución, se escoge una variable que no ha sido asignada, y se asigna un valor sobre esta, agregándola en la cola de decisiones por propagar. También, se crea en la pila un estado donde se almacenará las variables que son asignadas a partir de esta desición, y otros valores para recuperar el estado en caso de un conflicto. La complejidad de este método es asintóticamente lineal en el número de variables (porque se busca asignar la siguiente variable), pero en general solo realizará un número constante de operaciones

Después de esto, entra en acción el método de propagación. Este toma la cola de decisiones por asignar, y va realizando el chequeo de conflictos con cada una de ellas. Si la introducción de una asignación no induce un conflicto, el valor de la variable es asignado, y se almacena en la pila que un cambio fue realizado. 

Luego, se da paso al método que deriva las implicaciones. Este método es el que realiza la propagación unitaria.Potencialmente, cualquier asignación puede generar implicaciones, dado a las negaciones. Por lo tanto, para el estado de la variable que fue asignado en falso (esto es, p si la asignación fue p = *false*, o !p si la asignación fue p = *true*), se llamará a la función de deducción de implicaciones. Este función recorrerá las cláusulas de las que el literal es un vigilante, chequeando si cualquier implicacion es generada (esto es, buscando una cláusula cuyo tamaño sea ahora 1). Cómo se explicó anteriormente, la complejidad de esta operación está acotada asintóticamente por el número de cláusulas multiplicado por el número de variables, pero en la práctica el funciónamiento de este método es mucho más veloz. Después de que cada implicación fue descubierta y se realizó lo necesario para mantener la propiedad de watched literals, las implicaciones generadas son insertadas en la cola donde esperarán a ser asignadas. 

Si en alguna de estas asignaciones ocurre un error, el programa es dirigido a la función que recupera el estado. Esta función verifica si la última asignación evaluó tanto el valor verdadero como falso. Si no, entonces el valor es modificado, se recupera el estado de las variables, y se realiza la nueva asignación para esta variable. En cambio, si ya se intentó usar ambos valores y en ambos el conflicto fue encontrado, entonces se elimina esta decisión de la pila de estados y se itera al siguiente estado en la pila. Este método realiza un número de operaciones lineal al tamaño de variables por cada iteración. 

Finalmente, si un conjunto de asignaciones e implicaciones no genera un conflicto, entonces se responderá que la expresión se satisface. Por otro lado, si cada camino lleva a un conflicto, entonces la expresión se considera insatisfacible.

La complejidad general del algoritmo es díficil de calcular. Sabemos que está acotada porr una función exponencial, pero dependerá mucho del tipo de expresión la cantidad de operaciones utilizadas.

## Traductor de SAT a Sudoku:

El traductor se implemento en el archivo `retrieve_sudoku_from_SAT.cpp` en una clase llamda *retrieveSudokuFromSAT*. Para probarlo de forma individual usar:

`./sat-sud archivo_sudoku.txt`

El programa también puede recibir la instancia de SAT a través del standard input. 

El programa crea una instancia de retrieveSudokuFromSAT y llama a la función principal de la clase, *retrieveSudokuFromSAT_*, la cual realiza la traducción a través de los demás métodos privados de la clase. Primero se llama al método *getSatOutput*, el cual lee del input la solución a una instancia SAT que luego sera traducida a una solución de Sudoku. Este método ignora todas las lineas del input que sean comentarios hasta que llega al encabezado. Del encabezado consigue el resultado concreto (satisfacible, no satisfacible o tiempo límite excedido) y el número de variables en la instancia de SAT. 

Usando el número de variables del SAT se calcula el orden del Sudoku (N), ya que el número de variables debe ser igual a N^6. Posteriormente se leen las variables de la solución a SAT y se ignoran las que fueron asignadas *false*. Aquellas con valor asignado *true* corresponden a las N^4 posiciones del tablero, y señalan que número quedó asignado a cada casilla. Estas son guardadas en un vector de enteros *sat_variables_v*. 

Posteriormente se llama a la función *setSudokuStr* la cuál se encarga de crear el string que representa la solución al sudoku. Para esto se recorren las variables en *sat_variables_v*, y para cada una se se calcula la posición en el string que le toca mediante la función *getPositionInSudokuStr* y el número que representa usando *getDigit*. Se le asigna el caracter correspondiente al entero retornado por *getDigit* a la posición dada por *getPositionInSudokuStr*. 

Finalmente se imprime por salida estándar el orden del sudoku y el string que representa la solución a la instancia de sudoku. En *getSatOutput* solo se lee el input, entonces su complejidad es lineal en el tamaño del input, por lo que toma O(N^6) en tiempo, y O(N^4) en espacio. Luego el método *setSudokuStr* toma O(N^4) en tiempo ya que solo recorre el vector de las variables guardadas y tanto *getPositionInSudokuStr* como *getDigit* toman tiempo constante. En espacio, análogamente, usa O(N^4) en el string y el vector de variables. En total este traductor toma tiempo acotado por el input: O(N^6); y memoria, acotado por el tamaño de la solución al sudoku.

## Scripts para ejecutar el proyecto:

El proyecto cuenta con dos scripts en bash para su ejecución:

- *run_sat.sh* : ejecuta el proyecto usando nuestro propio solucionador
	
	`./run_sat.sh`

- *run_zchaff.sh* : ejecuta el proyecto usando el solucionador zchaff
	
	`./run_zchaff.sh`

Ambos scripts funciónan de forma análoga. Primero, de ser necesario, compilan los programas involucrados en el proyecto llamando al makefile del proyecto. Luego, para cada línea en el archivo de instancias de sudokus, traducen la instancia a SAT con el primer traductor, luego resuelven la instancia de SAT con el resolverdor de SAT del script, y finalmente traducen la solución a Sudoku. Las soluciónes van al archivo `log/solutions_log_sat.txt' (o 'log/solutions_log_zchaff.txt` según sea el caso) mientras las mediciones de tiempo van al archivo `log/time_log_sat.csv` (o `log/time_log_zchaff.csv`). El Time Limit usado para ambos casos es de 3 segundos.

De la misma manera, se tiene un pequeño script escrito en el lenguaje M, cuya función es tomar los tiempos de ejecución de cada uno de los programas para cada una de las instancias y crear una gráfica de comparación entre estos. La gráfica se almacena bajo el directorio `media`. La única dependencia para la ejecución de este script es la instalación de [Octave](https://www.gnu.org/software/octave/)

## Cambios para la seguna entrega

Para esta segunda entrega se introdujeron los siguientes cambios:

  - Arreglos en la documentación.
  - Mejoras en detalles de legibilidad del código y los distintos scripts utilizados.
  - Ahora el código fuente es compilado con el flag -O2, lo cuál debería producir una mejora en el tiempo de ejecución de nuestro resolvedor.
  - Cambios en el almacenamiento de los tiempos de ejecución permiten una mejor comparación ahora entre los resolvedores de SAT. Se actualizó la gráfica de comparación de resultados.
  - Dado a que desde la primera entrega se cumplieron los requerimientos de la segunda (la adición de propagación unitaria), estamos realizando un esfuerzo en entender/aprender/implementar alguna otra optimización que pueda mejorar el rendimiento de nuestro resolvedor. Si estás viendo este mensaje, probablemente no nos alcanzó el tiempo para entender e implementar alguna optimización que realmente haga una diferencia :disappointed:

## Instancias Resueltas:

Nuestro resolvedor de sudokus a base de SAT Solver puede resolver todas las instancias del archivo que fueron suministradas. Seguidamente, mostraremos ejemplos de instancias de sudoku y la respuesta otorgada por nuestro solver:

![alt text](https://github.com/neilvillamizar/SAT-based-sudoku-solver/blob/master/media/example1.png?raw=*true*)
![alt text](https://github.com/neilvillamizar/SAT-based-sudoku-solver/blob/master/media/example1_solved.png?raw=*true*)
![alt text](https://github.com/neilvillamizar/SAT-based-sudoku-solver/blob/master/media/example2.png?raw=*true*)
![alt text](https://github.com/neilvillamizar/SAT-based-sudoku-solver/blob/master/media/example2_solved.png?raw=*true*)
![alt text](https://github.com/neilvillamizar/SAT-based-sudoku-solver/blob/master/media/example3.png?raw=*true*)
![alt text](https://github.com/neilvillamizar/SAT-based-sudoku-solver/blob/master/media/example3_solved.png?raw=*true*)


El resto de los resultados pueden consultarse al ejecutar el script, el cual generará el log con los resultados.

## Tiempos de Ejecucion: 

Se compararon los tiempos de ejecución de diversas instancias de resolución de sudoku, utiliando el sat solver propuesto contra el solver zChaff. Los resultados fueron graficados en el siguiente cuadro:

![alt text](https://github.com/neilvillamizar/SAT-based-sudoku-solver/blob/master/media/time_bars.jpg?raw=*true*)

Para esta entrega se pudo actualizar la comparación de resultados. Ahora podemos evidenciar con mejor precisión los resultados para el resolvedor de zchaff. Se puede notar que, aún después de introducidas las optimizaciones de compilación a nuestro resolvedor, el tiempo de ejecución de zchaff es mejor en casi todas las instancias (exepto una), debido al gran número de optimizaciones que son implementadas en este resolvedor a diferencia del nuestro. Aún así, para estas instancias, el resolvedor propuesto por el grupo es suficientemente rápido como para ejecutar todas las instancias en menos de media décima de segundo.
