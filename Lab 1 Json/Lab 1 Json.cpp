#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <C:\Users\aldan\Downloads\json-develop\json-develop\nlohmann_json.natvis>

using namespace std;
using Json = nlohmann::json;

vector<vector<int>> findNearestOnes(vector<vector<int>>& arrays, Json& input1, Json& input2) {
    int num_arrays = input2.size();
    vector<vector<int>> left_ones(num_arrays); // Almacena los índices del último 1 encontrado hacia la izquierda
    vector<vector<int>> right_ones(num_arrays); // Almacena los índices del último 1 encontrado hacia la derecha
    vector<vector<int>> nearest_ones(num_arrays);

    // Primera pasada: Encuentra el último 1 hacia la izquierda para cada posición en cada arreglo
    for (int i = 0; i < num_arrays; ++i) {
        int n = input1.size();
        left_ones[i].resize(n);
        int last_one = INT_MIN;
        for (int j = 0; j < n; ++j) {
            if (arrays[j][i] == 1) {
                last_one = j;
            }
            left_ones[i][j] = last_one;
        }
    }

    // Segunda pasada: Encuentra el último 1 hacia la derecha para cada posición en cada arreglo
    for (int i = 0; i < num_arrays; ++i) {
        int n = input1.size();
        right_ones[i].resize(n);
        int last_one = INT_MAX;
        for (int j = n - 1; j >= 0; --j) {
            if (arrays[j][i] == 1) {
                last_one = j;
            }
            right_ones[i][j] = last_one;
        }
    }

    // Tercera pasada: Calcula la distancia al 1 más cercano para cada posición en cada arreglo
    for (int i = 0; i < num_arrays; ++i) {
        int n = input1.size();
        nearest_ones[i].resize(n);
        for (int j = 0; j < n; ++j) {
            int left_dist = (left_ones[i][j] == INT_MIN) ? INT_MAX : (j - left_ones[i][j]);
            int right_dist = (right_ones[i][j] == INT_MAX) ? INT_MAX : (right_ones[i][j] - j);
            nearest_ones[i][j] += min(left_dist, right_dist);
        }
    }

    return nearest_ones;
}

int main() {
    string Archivo;
    cout << "Ingrese la ruta del archivo Json: ";
    cin >> Archivo;

    ifstream ifs(Archivo);
    Json j;
    ifs >> j;

    auto input1 = j["input1"];
    auto input2 = j["input2"];

    vector<vector<int>> arrays;
    for (auto& element : input1) {
        vector<int> apartment;
        for (auto& business : element) {
            apartment.push_back(business.get<bool>() ? 1 : 0);
        }
        arrays.push_back(apartment);
    }

    vector<vector<int>> nearest_ones = findNearestOnes(arrays, input1, input2);

    vector<int> recommended_apartments;
    for (size_t i = 0; i < nearest_ones.size(); ++i) {
        bool satisfies_requirements = true;
        for (size_t j = 0; j < nearest_ones[i].size(); ++j) {
            if (input2[j].get<bool>() && nearest_ones[i][j] == INT_MAX) {
                satisfies_requirements = false;
                break;
            }
        }
        if (satisfies_requirements) {
            recommended_apartments.push_back(i);
        }
    }

    // Imprimir recomendaciones
    cout << "Recomendaciones de apartamentos: ";
    if (recommended_apartments.empty()) {
        cout << "No se encontraron apartamentos que cumplan con los requisitos." << endl;
    }
    else {
        cout << "[";
        for (size_t i = 0; i < recommended_apartments.size() - 1; ++i) {
            cout << recommended_apartments[i] << ", ";
        }
        cout << recommended_apartments.back() << "]" << endl;
    }
    return 0;
}
