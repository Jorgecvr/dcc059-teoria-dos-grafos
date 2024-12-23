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
    int grau, ordem, direcionado, componentesConexas, verticesPonderados, arestasPonderadas;
    bool completo, bipartido, arvore, arestaPonte, verticeArticulacao;

    std::ifstream arquivo("Descricao.txt");
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo Descricao.txt" << std::endl;
        return;
    }

    
    arquivo >> grau;                 
    arquivo >> ordem;                
    arquivo >> direcionado;          
    arquivo >> componentesConexas;   
    arquivo >> verticesPonderados;   
    arquivo >> arestasPonderadas;    
    arquivo >> completo;             
    arquivo >> bipartido;            
    arquivo >> arvore;               
    arquivo >> arestaPonte;          
    arquivo >> verticeArticulacao;   

    arquivo.close();

    // Semente para números aleatórios
    srand(time(0));

    // Criar o arquivo de saída
    std::ofstream arquivo_saida("grafo_aleatorio.txt");
    if (!arquivo_saida.is_open()) {
        std::cerr << "Erro ao criar o arquivo de saída" << std::endl;
        return;
    }

    arquivo_saida << ordem << " " << direcionado << " " << verticesPonderados << " " << arestasPonderadas << std::endl;

    // Gerar pesos para os vértices, se ponderados
    if (verticesPonderados) {
        for (int i = 0; i < ordem; ++i) {
            arquivo_saida << rand() % 10 + 1 << " "; // Peso aleatório para o vértice
        }
        arquivo_saida << std::endl;
    }

    // Gerar arestas e seus pesos (se necessário), se houver arestas ponderadas
    int arestas_max = (direcionado == 1) ? ordem * (ordem - 1) : ordem * (ordem - 1) / 2;
    int arestas_geradas = 0;

    // Gerar arestas aleatórias até o número máximo de arestas possíveis
    while (arestas_geradas < grau && arestas_geradas < arestas_max) {
        int origem = rand() % ordem + 1;
        int destino = rand() % ordem + 1;

        // Se o grafo for direcionado e a aresta já foi gerada, repetir
        if (direcionado == 1 && origem == destino) continue; // evitar laços (se necessário)

        // Se o grafo não for direcionado, evitamos duplicar as arestas (origem -> destino == destino -> origem)
        if (direcionado == 0 && origem > destino) continue;

        // Gerar peso para a aresta, se ponderada
        int peso = arestasPonderadas ? rand() % 21 - 10 : 0; // Pesos entre -10 e 10, se ponderado

        // Escrever aresta (origem, destino, peso)
        arquivo_saida << origem << " " << destino << " " << peso << std::endl;
        arestas_geradas++;
    }

    arquivo_saida.close();
}
