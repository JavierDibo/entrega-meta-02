#ifndef META_PRACTICA_02_LECTOR_DATOS_CUIDADES_H
#define META_PRACTICA_02_LECTOR_DATOS_CUIDADES_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <iomanip>

#include "omp.h"
#include "globals.h"
#include "Reloj.h"

class LectorCiudades {
private:
    std::string ruta_archivo_datos;
    std::vector<std::vector<float>> ciudades;
    std::vector<std::vector<float>> distancias;
    int num_ciudades{};
public:


public:

    LectorCiudades();

    explicit LectorCiudades(const std::string &ruta);

    static bool son_iguales(const std::vector<std::vector<double>> &A, const std::vector<std::vector<double>> &B,
                            double tolerancia = 1e-9);

    [[nodiscard]] std::string get_ruta_archivo_datos() const {
        return ruta_archivo_datos;
    }

    [[nodiscard]] std::vector<std::vector<float>> get_matriz_ciudades() const {
        return ciudades;
    }

    [[nodiscard]] std::vector<std::vector<float>> get_matriz_distancias() const {
        return distancias;
    }

    [[nodiscard]] double calcular_distancia(int ciudad_a, int ciudad_b) const;

    [[nodiscard]] int get_num_ciudades() const;

    [[nodiscard]] const std::vector<std::vector<float>> &get_ciudades() const;

    [[nodiscard]] const std::vector<std::vector<float>> &get_distancias() const;
};

#endif //META_PRACTICA_02_LECTOR_DATOS_CUIDADES_H