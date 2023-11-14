#include "Poblacion.h"

int encontrar_ciudad_mas_cercana(int ciudad_actual, const std::vector<bool> &visitada,
                                 const std::vector<std::vector<double>> &distancias) {
    double distancia_minima = std::numeric_limits<double>::max();
    int ciudad_mas_cercana = -1;

    size_t tam = distancias.size();
    for (int i = 0; i < tam; ++i) {
        if (!visitada[i] && distancias[ciudad_actual][i] < distancia_minima) {
            distancia_minima = distancias[ciudad_actual][i];
            ciudad_mas_cercana = i;
        }
    }

    return ciudad_mas_cercana;
}

std::vector<int> Poblacion::camino_greedy() {
    std::vector<std::vector<double>> distancias = lector_datos.get_distancias();
    std::vector<bool> visitadas(num_ciudades, false);
    std::vector<int> camino;
    camino.reserve(num_ciudades + 1);

    int ciudad_actual = random.get_int(0, num_ciudades - 1);;
    visitadas[ciudad_actual] = true;
    camino.push_back(ciudad_actual);

    for (int i = 1; i < num_ciudades; ++i) {
        int siguiente_ciudad = -1;
        double min_distancia = INFINITO_POSITIVO;
// #pragma omp parallel default(none) shared(distancias, visitadas, ciudad_actual, min_distancia, siguiente_ciudad, num_ciudades) if (PARALELIZACION)
        {
            int local_siguiente_ciudad = -1;
            double local_min_distancia = std::numeric_limits<double>::max();

// #pragma omp for nowait
            for (int j = 0; j < num_ciudades; ++j) {
                if (!visitadas[j] && distancias[ciudad_actual][j] < local_min_distancia) {
                    local_min_distancia = distancias[ciudad_actual][j];
                    local_siguiente_ciudad = j;
                }
            }

// #pragma omp critical
            {
                if (local_min_distancia < min_distancia) {
                    min_distancia = local_min_distancia;
                    siguiente_ciudad = local_siguiente_ciudad;
                }
            }
        }

        if (siguiente_ciudad != -1) {
            visitadas[siguiente_ciudad] = true;
            camino.push_back(siguiente_ciudad);
            ciudad_actual = siguiente_ciudad;
        }
    }

    return camino;
}


void Poblacion::crear_poblacion() {

    for (int i = 0; i < num_invididuos; ++i) {

        std::vector<int> ruta_inicial;
        double valor_aleatorio = random.get_double(0.0, 1.0);

        if (valor_aleatorio > PROBABILIDAD_GREEDY) {
            ruta_inicial = vector_aleatorio(num_ciudades);
        } else {
            ruta_inicial = camino_greedy();
        }

        Individuo individuo(ruta_inicial, lector_datos);
        individuo.evaluar();
        individuos.emplace_back(individuo);
    }
}


bool Poblacion::condicion_parada() {

    reloj.finalizar();
    double tiempo = reloj.obtener_tiempo_transcurrido(SEGUNDOS);

    return (num_generaciones >= MAX_NUMERO_GENERACIONES || tiempo >= 60.0);
}

void Poblacion::evolucionar() {
    while (!condicion_parada()) {

        std::vector<Individuo> nueva_poblacion;
        std::vector<Individuo> elites;

        nueva_poblacion.reserve(num_invididuos);
        elites.reserve(NUMERO_ELITES);

        /// Aplicar elitismo: almacenar los "num_elites" mejores individuos
        if (NUMERO_ELITES > 0) {
            std::vector<Individuo> aux = individuos;
            std::sort(aux.begin(), aux.end());
            elites.clear();
            elites.insert(elites.end(), aux.begin(), aux.begin() + NUMERO_ELITES);
        }

        /// Generar la nueva población
        while (nueva_poblacion.size() < num_invididuos) {
            Individuo padre_a = mejor_entre_random();
            Individuo padre_b = mejor_entre_random();
            std::vector<int> camino_hijo = cruzar(padre_a, padre_b);
            Individuo hijo(camino_hijo, lector_datos);
            hijo.mutar();
            hijo.evaluar();
            nueva_poblacion.push_back(hijo);
        }

        std::sort(nueva_poblacion.begin(), nueva_poblacion.end());

        /// Buscar los elites en la nueva poblacion y reemplazarlos si no estan por uno malo
        for (const Individuo &elite: elites) {
            // Si no está en la poblacion lanzo torneo kworst
            if (std::find(nueva_poblacion.begin(), nueva_poblacion.end(), elite) == nueva_poblacion.end()) {
                Individuo *peor = torneo_kworst();
                *peor = elite;
            }
        }

        /// Reemplazar la población antigua con la nueva
        individuos = nueva_poblacion;
        num_generaciones++;
    }
}

std::vector<int> Poblacion::cruzar(const Individuo &padre1, const Individuo &padre2) {
    double probabilidad_cruzar = random.get_double(0.0, 1.0);

    if (probabilidad_cruzar < PROBABILIDAD_CRUCE) { // 70% de probabilidad_cruzar de cruce
        if (random.get_double(0.0, 1.0) < 0.5) { // Elige uno de los dos operadores de cruce
            return cruceOX2(padre1, padre2);
        } else {
            return cruceMOC(padre1, padre2);
        }
    } else {
        /// Si no hay cruce, devuelve el mejor de los padres
        if (padre1 < padre2) {
            return padre1.get_camino();
        } else {
            return padre2.get_camino();
        }
    }
}

[[maybe_unused]] std::vector<int> Poblacion::cruceSimple(const Individuo &padre_a, const Individuo &padre_b) const {
    std::vector<int> camino_hijo;

    int punto_cruce = random.get_int(1, num_ciudades - 1);


    for (int i = 0; i < punto_cruce; i++) {
        camino_hijo.push_back(padre_a.get_ciudad(i));
    }

    // Copy remaining cities from padre_b to the camino_hijo
    for (int i = 0; i < num_ciudades; i++) {
        int ciudad = padre_b.get_ciudad(i);
        if (std::find(camino_hijo.begin(), camino_hijo.end(), ciudad) == camino_hijo.end()) {
            camino_hijo.push_back(ciudad);
        }
    }

    return camino_hijo;
}

std::vector<int> Poblacion::cruceOX2(const Individuo &padre_a, const Individuo &padre_b) const {
    std::vector<int> camino_hijo = padre_a.get_camino(); // Copia del camino del padre_a
    std::unordered_set<int> ciudadesSeleccionadas;
    std::vector<int> posicionesSeleccionadas;

    // Seleccionar aleatoriamente las posiciones a reordenar
    while (posicionesSeleccionadas.size() < num_ciudades / 2) {
        int pos = random.get_int(0, num_ciudades - 1);
        if (ciudadesSeleccionadas.find(padre_a.get_ciudad(pos)) == ciudadesSeleccionadas.end()) {
            ciudadesSeleccionadas.insert(padre_a.get_ciudad(pos));
            posicionesSeleccionadas.push_back(pos);
        }
    }

    // Eliminar las ciudades seleccionadas del camino_hijo basado en las posiciones seleccionadas
    for (int pos: posicionesSeleccionadas) {
        camino_hijo[pos] = -1; // Marcamos la posición como vacía
    }

    // Rellenar las posiciones vacías en el camino_hijo con las ciudades en el mismo orden que aparecen en padre_b
    int indexParaInsertar = 0;
    for (int ciudad: padre_b.get_camino()) {
        if (ciudadesSeleccionadas.find(ciudad) != ciudadesSeleccionadas.end()) {
            // Encontrar la próxima posición vacía en el camino_hijo
            while (indexParaInsertar < camino_hijo.size() && camino_hijo[indexParaInsertar] != -1) {
                indexParaInsertar++;
            }

            // Comprobar si hemos salido del rango del vector
            if (indexParaInsertar >= camino_hijo.size()) {
                // Manejar el error o romper el bucle
                break;
            }

            camino_hijo[indexParaInsertar] = ciudad;
        }
    }

    return camino_hijo;
}

std::vector<int> Poblacion::cruceMOC(const Individuo &padre_a, const Individuo &padre_b) const {
    int punto_cruce = random.get_int(0, num_ciudades - 1);
    std::vector<int> camino_hijo(num_ciudades, -1);
    std::unordered_set<int> ciudades_incluidas;

    for (int i = 0; i < punto_cruce; ++i) {
        int ciudad = padre_a.get_ciudad(i);
        camino_hijo[i] = ciudad;
        ciudades_incluidas.insert(ciudad);
    }

    for (int i = punto_cruce; i < num_ciudades; ++i) {
        int ciudad = padre_b.get_ciudad(i);
        if (ciudades_incluidas.insert(ciudad).second) {
            camino_hijo[i] = ciudad;
        }
    }

    std::vector<int> ciudades_restantes;
    for (int ciudad: padre_b.get_camino()) {
        if (ciudades_incluidas.insert(ciudad).second) {
            ciudades_restantes.push_back(ciudad);
        }
    }

    for (int i = punto_cruce, j = 0; i < num_ciudades && j < ciudades_restantes.size(); ++i) {
        if (camino_hijo[i] == -1) {
            camino_hijo[i] = ciudades_restantes[j++];
        }
    }

    return camino_hijo;
}

[[maybe_unused]] std::vector<int> Poblacion::crucePMX(const Individuo &padre_a, const Individuo &padre_b) const {

    int punto_cruce_a = random.get_int(0, num_invididuos - 1);
    int punto_cruce_b = random.get_int(0, num_invididuos - 1);

    // Asegurarse de que punto_cruce_a < punto_cruce_b
    if (punto_cruce_a > punto_cruce_b) {
        std::swap(punto_cruce_a, punto_cruce_b);
    }

    std::vector<int> camino_hijo(num_invididuos);
    std::unordered_map<int, int> mapeo;

    // Intercambiar los segmentos entre los puntos de cruce
    for (int i = punto_cruce_a; i <= punto_cruce_b; ++i) {
        camino_hijo[i] = padre_a.get_ciudad(i);
        mapeo[padre_a.get_ciudad(i)] = padre_b.get_ciudad(i);
    }

    // Completar la descendencia resolviendo los conflictos de duplicados
    for (int i = 0; i < num_invididuos; ++i) {
        if (i >= punto_cruce_a && i <= punto_cruce_b) continue;
        int ciudad = padre_b.get_ciudad(i);
        while (mapeo.count(ciudad)) {
            ciudad = mapeo[ciudad];
        }
        camino_hijo[i] = ciudad;
    }

    return camino_hijo;
}

Individuo Poblacion::mejor_entre_random() {
    if (KBEST > num_invididuos) {
        throw std::runtime_error("KBEST no puede ser mayor que num_invididuos");
    }

    std::unordered_set<int> indices_unicos;
    while (indices_unicos.size() < KBEST) {
        int indice;
        do {
            indice = random.get_int(0, num_invididuos - 1);
        } while (indices_unicos.find(indice) != indices_unicos.end());

        indices_unicos.insert(indice);
    }

    auto mejor_indice = *indices_unicos.begin();
    for (auto indice: indices_unicos) {
        if (individuos[mejor_indice] > individuos[indice]) {
            mejor_indice = indice;
        }
    }

    return individuos[mejor_indice];
}


Individuo *Poblacion::torneo_kworst() {
    if (KWORST > num_invididuos) {
        // Manejar este error adecuadamente
        throw std::runtime_error("KWORST no puede ser mayor que num_invididuos");
    }

    std::unordered_set<int> indices_unicos;
    while (indices_unicos.size() < KWORST) {
        int indice = random.get_int(0, num_invididuos - 1);
        indices_unicos.insert(indice);
    }

    auto peor_indice = *indices_unicos.begin();
    for (auto indice: indices_unicos) {
        if (individuos[peor_indice] < individuos[indice]) {
            peor_indice = indice;
        }
    }

    return &individuos[peor_indice];
}

Poblacion::~Poblacion() = default;

Poblacion::Poblacion(LectorCiudades &lector_datos) : lector_datos(lector_datos) {
    num_ciudades = lector_datos.get_num_ciudades();
    reloj.iniciar();
    crear_poblacion();
}

const std::vector<Individuo> &Poblacion::get_individuos() const {
    return individuos;
}