#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "grafo_matriz.h"

using namespace std;

// Construtor sem parâmetros
grafo_matriz::grafo_matriz() : ordem(0), direcionado(false) {
    // Inicializa os membros sem alocar as matrizes
}

// Destruidor
grafo_matriz::~grafo_matriz() {}

// Função para calcular o índice no vetor linear
int grafo_matriz::calcularIndiceLinear(int origem, int destino) {
    if (origem <= destino) {
        // Cálculo para matriz triangular superior
        return (destino * (destino - 1)) / 2 + origem - 1;
    }
    return (origem * (origem - 1)) / 2 + destino - 1;
}

// Carrega o grafo a partir de um arquivo
void grafo_matriz::carrega_grafo() {
    std::ifstream arquivo("Grafo.txt");
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo Grafo.txt" << std::endl;
        return;
    }

    std::string linha;
    int num_vertices, direcionado, vertice_ponderado, aresta_ponderada;

    // Lê a primeira linha com as configurações do grafo
    if (std::getline(arquivo, linha)) {
        std::istringstream iss(linha);
        iss >> num_vertices >> direcionado >> vertice_ponderado >> aresta_ponderada;

        this->ordem = num_vertices;
        this->direcionado = (direcionado == 1);

        // Recria o vetor linear de acordo com o número de vértices
        matrizLinear.resize((ordem * (ordem + 1)) / 2, 0);

        // Se o grafo for direcionado, aloca a matriz 2D
        if (this->direcionado) {
            matriz.resize(ordem, vector<int>(ordem, 0));  // Inicializa a matriz 2D com 0
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

        // Se o grafo for direcionado
        if (direcionado) {
            if(aresta_ponderada) {
                matriz[origem - 1][destino - 1] = peso;
            } else {
                matriz[origem - 1][destino - 1] = 1;
            }
        } else {
            // Se o grafo for não direcionado, usa o vetor linear
            int indice = calcularIndiceLinear(origem, destino);

            if (aresta_ponderada) {
                matrizLinear[indice] = peso;
            } else {
                matrizLinear[indice] = 1;  // Se não for ponderado, coloca 1
            }

            // Adiciona a aresta na direção inversa (não direcionado)
            int indiceInvertido = calcularIndiceLinear(destino, origem);

            if (aresta_ponderada) {
                matrizLinear[indiceInvertido] = peso;
            } else {
                matrizLinear[indiceInvertido] = 1;  // Se não for ponderado, coloca 1
            }
        }
    }

    arquivo.close();
    std::cout << "Grafo carregado com sucesso!" << std::endl;
}

// Função para obter a matriz linear
const std::vector<int>& grafo_matriz::get_matriz_linear() const {
    return matrizLinear;
}

// Função para obter a matriz 2D de adjacência
const std::vector<std::vector<int>>& grafo_matriz::get_matriz() const {
    return matriz;
}

// Implementações das funções restantes
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
    return ordem;
}

bool grafo_matriz::eh_direcionado() {
    return direcionado;
}

bool grafo_matriz::vertice_ponderado() {
    return false;
}

bool grafo_matriz::aresta_ponderada() {
    return false;
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
