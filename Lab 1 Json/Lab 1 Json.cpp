
// Lab 1 Json.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Función para parsear el archivo JSONL e inicializar los arreglos de apartamentos y requerimientos
vector<vector<bool>> parseInputJsonl(string filename) {
    ifstream jsonFile(filename);
    string json;
    vector<vector<bool>> map;

    if (jsonFile.is_open()) {
        while (getline(jsonFile, json)) {
            map.push_back(vector<bool>());
            int comma = 0;
            stringstream ss(json);
            string token;
            while (getline(ss, token, ',')) {
                if (token[0] == '\"' && token[token.size() - 1] == '\"') {
                    token.erase(remove(token.begin(), token.end(), '\"'), token.end());
                    map.back().push_back(token == "true");
                }
                else if (token == "false") {
                    map.back().push_back(false);
                }
                else if (token == "true") {
                    map.back().push_back(true);
                }
                comma++;
            }
        }
        jsonFile.close();
    }
    else {
        cerr << "Error: no se puede abrir el archivo de salida" << endl;
        exit(1);
    }

    return map;
}

// Función para encontrar los apartamentos que coincidan con los requerimientos
vector<int> findMatchingApartments(vector<vector<bool>> map, vector<string> requerimientos) {
    vector<int> matches;

    for (int i = 0; i < map.size(); i++) {
        if (map[i].size() < requerimientos.size()) {
            // Devolvemos un vector vacío o lanzamos una excepción si el tamaño de map[i] es menor que el tamaño de requerimientos
            return matches;
        }

        bool match = true;
        for (int j = 0; j < requerimientos.size(); j++) {
            if (map[i][j] != (requerimientos[j] == "true")) {
                match = false;
                break;
            }
        }
        if (match) {
            matches.push_back(i);
        }
    }

    return matches;
}

// Función para escribir la salida en un archivo
void writeOutput(vector<int> matches, string input_example) {
    ofstream outfile(input_example);
    if (outfile.is_open()) {
        outfile << "[";
        for (int i = 0; i < matches.size(); i++) {
            outfile << matches[i];
            if (i < matches.size() - 1) {
                outfile << ",";
            }
        }
        outfile << "]";
        outfile.close();
    }
    else {
        cerr << "Error: no se puede abrir el archivo" << endl;
        exit(1);
    }
}

int main() { // Parseamos el archivo JSONL de entrada 
    vector<vector<bool>> map = parseInputJsonl("input_challenge.jsonl");

    // Imprimimos el tamaño de map y el número de requerimientos 
    cout << "Tamaño de map: " << map.size() << endl; vector<string> requerimientos = { "Academy", "Gym", "Store" };

    cout << "Departamento sugerido:  " << requerimientos.size() << endl;

    // Encontramos los apartamentos que coincidan con los requerimientos
    vector<int> matches = findMatchingApartments(map, requerimientos);

    // Imprimimos el tamaño de matches antes de escribir la salida en un archivo 
    //cout << "Tamaño de matches: " << matches.size() << endl;

    // Escribimos la salida en un archivo 
    writeOutput(matches, "output.json");

    return 0;
}
