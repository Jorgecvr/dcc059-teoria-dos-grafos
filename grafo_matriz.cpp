#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include "grafo_matriz.h"
#include <iostream>

using namespace std;

grafo_matriz::grafo_matriz(int ordem, bool direcionado)
    : ordem(ordem), direcionado(direcionado), matriz(ordem, std::vector<int>(ordem, 0)) {}

grafo_matriz::~grafo_matriz() {}

void grafo_matriz::carrega_grafo() {

        std::ifstream arquivo("Grafo.txt");
        if (!arquivo.is_open()) {
            std::cerr << "Erro ao abrir o arquivo Grafo.txt" << std::endl;
            return;
        }

        std::string linha;
        int num_vertices, direcionado, vertice_ponderado, aresta_ponderada;

        // Lê a primeira linha: número de nós, direcionado, ponderado vértices, ponderado arestas
        if (std::getline(arquivo, linha)) {
            std::istringstream iss(linha);
            iss >> num_vertices >> direcionado >> vertice_ponderado >> aresta_ponderada;

            this->ordem = num_vertices;
            this->direcionado = (direcionado == 1);
            matriz = std::vector<std::vector<int>>(ordem, std::vector<int>(ordem, 0));
        }

        // Se os vértices forem ponderados, lê os pesos dos vértices
        std::vector<int> pesos_vertices;
        if (vertice_ponderado && std::getline(arquivo, linha)) {
            std::istringstream iss(linha);
            int peso;
            while (iss >> peso) {
                pesos_vertices.push_back(peso);
            }
        }

        // Lê as arestas (origem, destino, peso)
        while (std::getline(arquivo, linha)) {
            std::istringstream iss(linha);
            int origem, destino, peso = 1; // Peso padrão é 1 caso não seja ponderado
            iss >> origem >> destino;
            if (aresta_ponderada) {
                iss >> peso;
            }

            // Adiciona a aresta à matriz de adjacência
            matriz[origem - 1][destino - 1] = peso;

            // Se o grafo não for direcionado, adiciona a aresta na direção inversa
            if (!this->direcionado) {
                matriz[destino - 1][origem - 1] = peso;
            }
        }

        arquivo.close();

        std::cout << "Grafo carregado com sucesso!" << std::endl;
}

const std::vector<std::vector<int>>& grafo_matriz::get_matriz() const {
    return matriz;
}


/////////////////////////////////////////////////////////////////////////////

bool grafo_matriz::eh_bipartido() {
    return false;
}

int grafo_matriz::n_conexo() {
    return 0;
}

int grafo_matriz::get_grau() {
    return 0;
}

int grafo_matriz::get_ordem() {
    return matriz.size();
}

bool grafo_matriz::eh_direcionado() {
    return direcionado;
}

bool grafo_matriz::vertice_ponderado() {
    return 0;
}

bool grafo_matriz::aresta_ponderada() {
    return 0;
}

bool grafo_matriz::eh_completo() {
    return false;
}

bool grafo_matriz::eh_arvore() {
    return false;
}

bool grafo_matriz::possui_articulacao() {
    return false;
}

bool grafo_matriz::possui_ponte() {
    return false;
}

void grafo_matriz::novo_grafo() {
}
