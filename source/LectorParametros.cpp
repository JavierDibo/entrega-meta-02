#include "LectorParametros.h"


void LectorParametros::leer_parametros() {

    std::ifstream archivo_parametros(nombre_archivo_parametros);
    if (!archivo_parametros.is_open()) {
        throw std::runtime_error("leer_parametros::No se pudo abrir el archivo " + nombre_archivo_parametros);
    }

    string line;
    while (std::getline(archivo_parametros, line)) {
        if (!line.empty() && (line[0] == '#' || (line.size() >= 2 && line[0] == '/' && line[1] == '/'))) {
            continue;
        }

        size_t pos = line.find('=');
        if (pos != string::npos) {
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);
            key.erase(0, key.find_first_not_of(' '));
            key.erase(key.find_last_not_of(' ') + 1);
            value.erase(0, value.find_first_not_of(' '));
            value.erase(value.find_last_not_of(' ') + 1);
            mapa_parametros[key] = value;
        }
    }

    archivo_parametros.close();

    if (mapa_parametros.find("semilla") != mapa_parametros.end()) {
        string seeds_string = mapa_parametros["semilla"];
        std::stringstream ss(seeds_string);
        string seed;
        while (std::getline(ss, seed, ',')) {
            VEC_SEMILLAS.push_back(std::stoi(seed));
        }
    }

    if (VEC_SEMILLAS.empty()) {
        VEC_SEMILLAS.push_back(12345678);
        VEC_SEMILLAS.push_back(23456781);
        VEC_SEMILLAS.push_back(34567812);
    }

    if (mapa_parametros.find("echo") != mapa_parametros.end()) {
        ECHO = (mapa_parametros["echo"] == "true");
    }

    if (mapa_parametros.find("log") != mapa_parametros.end()) {
        LOG = (mapa_parametros["log"] == "true");
    }

    if (mapa_parametros.find("max_num_evaluacion") != mapa_parametros.end()) {
        MAX_NUM_EVALUACIONES = std::stoi(mapa_parametros["max_num_evaluacion"]);
    }

    if (mapa_parametros.find("cruce") != mapa_parametros.end()) {
        OPERADOR_CRUCE_GEN = std::stoi(mapa_parametros["cruce"]);
    }

    if (mapa_parametros.find("diferencial") != mapa_parametros.end()) {
        OPERADOR_DIFERENCIAL = std::stoi(mapa_parametros["diferencial"]);
    }

    if (mapa_parametros.find("algoritmo") != mapa_parametros.end()) {
        ALGORITMO = std::stoi(mapa_parametros["algoritmo"]);
    }

    if (mapa_parametros.find("probabilidad_cruce") != mapa_parametros.end()) {
        PROBABILIDAD_CRUCE = std::stod(mapa_parametros["probabilidad_cruce"]);
    }

    if (mapa_parametros.find("probabilidad_cruce") != mapa_parametros.end()) {
        PROBABILIDAD_CRUCE = std::stod(mapa_parametros["probabilidad_cruce"]);
    }

    if (mapa_parametros.find("probabilidad_mutacion") != mapa_parametros.end()) {
        PROBABILIDAD_MUTACION = std::stod(mapa_parametros["probabilidad_mutacion"]);
    }

    if (mapa_parametros.find("probabilidad_greedy") != mapa_parametros.end()) {
        PROBABILIDAD_GREEDY = std::stod(mapa_parametros["probabilidad_greedy"]);
    }

    if (mapa_parametros.find("numero_elites") != mapa_parametros.end()) {
        string numeros_string = mapa_parametros["numero_elites"];
        std::stringstream ss(numeros_string);
        string numero;
        while (getline(ss, numero, ',')) {
            VEC_NUMERO_ELITES.push_back(std::stoi(numero));
        }
    }

    if (mapa_parametros.find("max_num_generaciones") != mapa_parametros.end()) {
        MAX_NUMERO_GENERACIONES = std::stoi(mapa_parametros["max_num_generaciones"]);
        if (MAX_NUMERO_GENERACIONES <= 0) {
            MAX_NUMERO_GENERACIONES = INFINITO_POSITIVO;
        }
    }

    if (mapa_parametros.find("numero_individuos") != mapa_parametros.end()) {
        string numeros_string = mapa_parametros["numero_individuos"];
        std::stringstream ss(numeros_string);
        string numero;
        while (getline(ss, numero, ',')) {
            VEC_NUM_INDIVIDUOS.push_back(std::stoi(numero));
        }
    }

    if (mapa_parametros.find("max_segundos_ejecucion") != mapa_parametros.end()) {
        MAX_TIEMPO_EJECUCION = std::stod(mapa_parametros["max_segundos_ejecucion"]);
    }

    if (mapa_parametros.find("max_num_evaluaciones") != mapa_parametros.end()) {
        MAX_NUM_EVALUACIONES = std::stoi(mapa_parametros["max_num_evaluaciones"]);
        if (MAX_NUM_EVALUACIONES <= 0) {
            MAX_NUM_EVALUACIONES = INFINITO_POSITIVO;
        }
    }

    if (mapa_parametros.find("numero_kbest") != mapa_parametros.end()) {
        string numeros_string = mapa_parametros["numero_kbest"];
        std::stringstream ss(numeros_string);
        string numero;
        while (getline(ss, numero, ',')) { // Use comma as the delimiter
            VEC_KBEST.push_back(std::stoi(numero));
        }
    }


    if (mapa_parametros.find("numero_kworst") != mapa_parametros.end()) {
        KWORST = std::stoi(mapa_parametros["numero_kworst"]);
    }

    if (mapa_parametros.find("datos") != mapa_parametros.end()) {
        string archivos_string = mapa_parametros["datos"];
        std::stringstream ss(archivos_string);
        string archivo;
        while (getline(ss, archivo, ',')) {
            VEC_ARCHIVOS_DATOS.push_back("datos/" + archivo);
            ARCHIVO_DATOS = "datos/" + archivo;
        }
    }

    if (mapa_parametros.find("paralelizacion") != mapa_parametros.end()) {
        PARALELIZACION = (mapa_parametros["paralelizacion"] == "true");
    }

    if (mapa_parametros.find("statistics") != mapa_parametros.end()) {
        STATS = (mapa_parametros["statistics"] == "true");
    }
}

[[maybe_unused]] LectorParametros::LectorParametros(string nombre_archivo) {
    this->nombre_archivo_parametros = std::move(nombre_archivo);

    leer_parametros();
}

LectorParametros::~LectorParametros() = default;

[[maybe_unused]] const std::map<string, string> &LectorParametros::getMapaParametros() const {
    return mapa_parametros;
}

[[maybe_unused]] const string &LectorParametros::getNombreArchivoParametros() const {
    return nombre_archivo_parametros;
}
