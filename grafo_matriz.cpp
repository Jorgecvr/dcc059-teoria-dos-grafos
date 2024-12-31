#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "grafo_matriz.h"
#include <ctime>

using namespace std;

// Construtor sem parâmetros
grafo_matriz::grafo_matriz() : ordem(0), direcionado(false) {
    // Inicializa os membros sem alocar as matrizes
}

// Destrutor
grafo_matriz::~grafo_matriz() {}

// Variaveis pra copiar alguns valores para funções
int ordem;
bool vtP;
bool aTP;

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
        ordem = num_vertices;
        vtP = vertice_ponderado;
        aTP = aresta_ponderada;
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

// Função n_conexo modificada
int grafo_matriz::n_conexo() {
    vector<bool> visitado(ordem, false);  // Marca os vértices visitados
    int componentes_conexas = 0;

    // Função lambda para realizar a DFS
    auto dfs = [&](int v, auto& dfs_ref) -> void {
        visitado[v] = true;

        if (direcionado) {
            // Para grafos direcionados: percorre a linha v da matriz de adjacência
            for (int u = 0; u < ordem; ++u) {
                if (matriz[v][u] != 0 && !visitado[u]) {
                    dfs_ref(u, dfs_ref);  // Chamando a recursão para o próximo vértice
                }
            }
        } else {
            // Para grafos não direcionados: percorre todas as arestas
            // Deve percorrer os dois sentidos da aresta (v -> u e u -> v)
            for (int u = 0; u < ordem; ++u) {
                if (u != v) {  // Não deve visitar o próprio vértice
                    int indice = calcularIndiceLinear(v + 1, u + 1);  // Calculando o índice da aresta
                    if (matrizLinear[indice] != 0 && !visitado[u]) {
                        dfs_ref(u, dfs_ref);  // Chamando a recursão para o próximo vértice
                    }
                }
            }
        }
    };

    // Percorre todos os vértices e realiza DFS para encontrar componentes conexas
    for (int i = 0; i < ordem; ++i) {
        if (!visitado[i]) {
            componentes_conexas++;  // Inicia uma nova componente conexa
            dfs(i, dfs);  // Realiza DFS a partir do vértice i
        }
    }

    return componentes_conexas;
}


// Função para calcular o maior grau
int grafo_matriz::get_grau() {
    int grau_maximo = 0;

    if (direcionado) {
        // Grafo direcionado (matriz 2D)
        for (int i = 0; i < ordem; ++i) {
            int grau_saida = 0;
            int grau_entrada = 0;
            // Soma as conexões de saída (linha)
            for (int j = 0; j < ordem; ++j) {
                if (matriz[i][j] != 0) grau_saida++;
            }
            // Soma as conexões de entrada (coluna)
            for (int j = 0; j < ordem; ++j) {
                if (matriz[j][i] != 0) grau_entrada++;
            }
            int grau_vertice = grau_saida + grau_entrada;
            grau_maximo = std::max(grau_maximo, grau_vertice);
        }
    } else {
        // Grafo não direcionado (matriz linear)
        for (int i = 0; i < ordem; ++i) {
            int grau_vertice = 0;
            // Conta as arestas de cada vértice
            for (int j = i + 1; j < ordem; ++j) {
                int indice = calcularIndiceLinear(i + 1, j + 1);  // Calcula o índice da aresta (1-indexed)
                if (matrizLinear[indice] != 0) {
                    grau_vertice++;
                }
            }
            grau_maximo = std::max(grau_maximo, grau_vertice);
        }
    }
    
    return grau_maximo;
}

int grafo_matriz::get_ordem() {
    return ordem;
}

bool grafo_matriz::eh_direcionado() {
    return direcionado;
}

bool grafo_matriz::vertice_ponderado() {
    std::cout << "Vértices ponderados: ";
    return vtP;
}

bool grafo_matriz::aresta_ponderada() {
    std::cout << "Arestas ponderados: ";
    return aTP;
}

bool grafo_matriz::eh_completo() {
    if (direcionado) {
        // Verifica matriz 2D para grafos direcionados
        for (int i = 0; i < ordem; ++i) {
            for (int j = 0; j < ordem; ++j) {
                if (i != j && matriz[i][j] == 0) {
                    // Se há um par de vértices (i, j) sem aresta, não é completo
                    return false;
                }
            }
        }
    } else {
        // Verifica vetor linear para grafos não direcionados
        for (int i = 1; i <= ordem; ++i) {
            for (int j = i + 1; j <= ordem; ++j) {
                int indice = calcularIndiceLinear(i, j);
                if (matrizLinear[indice] == 0) {
                    // Se há um par de vértices sem conexão, não é completo
                    return false;
                }
            }
        }
    }

    // Se todas as condições foram satisfeitas, o grafo é completo
    return true;
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
