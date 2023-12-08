# Manual de usuario

## 1. Ejecución del programa

Para ejecutar el algoritmo se ruega que se haga en el entorno CLion. Debido a la naturaleza de la ejecución y nombre de directorios en los distintos sistemas, la compilación en un .exe del programa no produce los resultados esperables.

Para ejecutar el programa en el entorno CLion simplemente basta con abrir el archivo adjunto a través del menú correspondiente, y establecer el nombre del archivo de parámetros (./parametros.txt) en la ejecución, de esta forma:

![imagen1](https://github.com/JavierDibo/entrega-meta-02/blob/main/imagenes/imagen1.png)

![imagen2](https://github.com/JavierDibo/entrega-meta-02/blob/main/imagenes/imagen2.png)

Los archivos logs se pueden encontrar en este [link de Google Drive](https://drive.google.com/file/d/1PJ68QoQrf4463XZ-_LRmtkPR6VusRCK-/view?usp=sharing).

Una vez proporcionados los datos correspondientes en el archivo de parámetros (localizado en ./cmake-build-debug/) es recomendable recargar el archivo cmake haciendo click derecho en el proyecto:

![imagen3](https://github.com/JavierDibo/entrega-meta-02/blob/main/imagenes/imagen3.png)

## 1.2 Archivo de parámetros

Se presentan todos los parámetros que se pueden modificar en el archivo de parámetros.

- **semilla**
  - *Descripción:* Representa las semillas utilizadas para generar números aleatorios en el programa.
  - *Uso:* Se puede establecer una única semilla o varias separadas por comas. Si no se especifica una semilla, el programa utiliza un conjunto de semillas por defecto.
  - *Ejemplo:* `semilla=3475,3718,3264`
  - *Nota:* Las semillas utilizadas para las ejecuciones recogidas son: 77646884, 47764688, 84776468, 88477646, 68847764.

- **algoritmo**
  - *Descripción:* Determina el algoritmo que se utilizará para resolver el problema.
  - *Uso:* Puede ser uno entre generacional o diferencial, representados por números: 0 o 1 respectivamente.
  - *Ejemplos:* 
    - `algoritmo=0` (generacional)
    - `algoritmo=1` (diferencial)

- **cruce**
  - *Descripción:* Define el operador de cruce a utilizar en el algoritmo generacional.
  - *Uso:* 0 representa el operador OX2, 1 representa el operador MOC.

- **diferencial**
  - *Descripción:* Define la selección de individuos en el algoritmo diferencial.
  - *Uso:* 0 representa EDA, 1 representa EDB.

- **echo**
  - *Descripción:* Si está activado, el programa imprimirá datos por consola.
  - *Valores:* true o false

- **paralelizacion**
  - *Descripción:* Activa la paralelización para mejorar el rendimiento del programa usando OpenMP.
  - *Valores:* true o false

- **log**
  - *Descripción:* Si está activado, el programa creará archivos log con información detallada de las operaciones y las guardará en la carpeta correspondiente.
  - *Valores:* true o false

- **datos**
  - *Descripción:* Especifica los archivos de datos que el programa usará.
  - *Uso:* Se pueden especificar múltiples archivos separados por comas.
  - *Ejemplo:* `datos=ch130.tsp,pr144.tsp,d15112.tsp`

- **max_num_evaluaciones**
  - *Descripción:* Define el número máximo de evaluaciones que el algoritmo realizará.
  - *Valores:* Un número entero positivo. Si se introduce 0 se asume que el valor es infinito.

- **max_num_generaciones**
  - *Descripción:* Define el número máximo de generaciones que el algoritmo realizará.
  - *Valores:* Un número entero positivo. Si se introduce 0 se asume que el valor es infinito.
  - *Nota:* se tomará el menor valor entre el número de evaluaciones y el número de generaciones.

- **numero_individuos**
  - *Descripción:* Especifica el número de individuos de la población.
  - *Uso:* Se pueden establecer uno o más valores enteros.
  - *Ejemplo:* `numero_individuos=50,100`
  - *Nota:* El número de individuos debe ser mayor que el número de KBEST.

- **max_segundos_ejecucion**
  - *Descripción:* Define el tiempo máximo medido en segundos que el algoritmo toma en evolucionar a la población.
  - *Valores:* Un número real positivo.

- **numero_elites**
  - *Descripción:* Especifica el número de elites que se tendrán en cuenta en la población.
  - *Uso:* Se pueden establecer uno o más valores enteros.
  - *Ejemplo:* `numero_elites=1,2`

- **numero_kbest**
  - *Descripción:* Especifica el número de KBEST que se tendrán en cuenta en la selección de la población.
  - *Uso:* Se pueden establecer uno o más valores enteros.
  - *Ejemplo:* `numero_kbest=2,3`

- **numero_kworst**
  - *Descripción:* Especifica el número de KWORST que se tendrán en cuenta en la selección de la población.
  - *Valores:* Un número entero positivo.

- **probabilidad_cruce**
  - *Descripción:* Especifica la probabilidad de que dos individuos se crucen.
  - *Valores:* Un número real entre 0 y 1.

- **probabilidad_mutacion**
  - *Descripción:* Especifica la probabilidad de que un individuo mute.
  - *Valores:* Un número real entre 0 y 1.

- **probabilidad_greedy**
  - *Descripción:* Especifica la cantidad de individuos que serán generados mediante el algoritmo greedy. El resto de la población será generada de forma aleatoria.
  - *Valores:* Un número real entre 0 y 1.

Si en la ejecución CMake diese algún error, el archivo CMakeLists.txt utilizado es el siguiente:

```cmake
cmake_minimum_required(VERSION 3.26)
project(meta_practica_02)

set(CMAKE_CXX_STANDARD 17)

add_executable(meta_practica_02 source/main.cpp
        source/LectorCiudades.cpp
        source/LectorCiudades.h
        source/LectorCiudades.h
        source/globals.h
        source/globals.cpp
        source/Reloj.cpp
        source/Reloj.h
        source/LectorParametros.cpp
        source/LectorParametros.h
        source/Logger.cpp
        source/Logger.h
        source/Individuo.cpp
        source/Individuo.h
        source/Poblacion.cpp
        source/Poblacion.h
        source/Poblacion.cpp
)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(meta_practica_02 PUBLIC OpenMP::OpenMP_CXX)
endif()
```