#include "grafo_matriz.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

grafo_matriz::grafo_matriz() : matriz(nullptr), matrizLinear(nullptr), ordem(0), direcionado(false), vtp(false), atp(false) {}

grafo_matriz::~grafo_matriz() {
    if (matriz) {
        for (int i = 0; i < ordem; ++i) {
            delete[] matriz[i];
        }
        delete[] matriz;
    }
    delete[] matrizLinear;
}

void grafo_matriz::inicializarMatrizes() {
    matriz = new int*[ordem];
    for (int i = 0; i < ordem; ++i) {
        matriz[i] = new int[ordem]();
    }
    matrizLinear = new int[(ordem * (ordem + 1)) / 2]();
}

int grafo_matriz::calcularIndiceLinear(int origem, int destino) {
    if (origem <= destino) {
        return (destino * (destino - 1)) / 2 + origem - 1;
    }
    return (origem * (origem - 1)) / 2 + destino - 1;
}

int** grafo_matriz::get_matriz() {
    return matriz;
}

int* grafo_matriz::get_matriz_linear() {
    return matrizLinear;
}

void grafo_matriz::carrega_grafo() {
    ifstream arquivo("Grafo.txt");
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo Grafo.txt" << endl;
        return;
    }

    arquivo >> ordem >> direcionado >> vtp >> atp;

    inicializarMatrizes();

    int* pesosVertices = nullptr;
    if (vtp) {
        pesosVertices = new int[ordem];
        for (int i = 0; i < ordem; ++i) {
            arquivo >> pesosVertices[i];
        }
    }

    int origem, destino, peso = 1;
    while (arquivo >> origem >> destino) {
        if (atp) {
            arquivo >> peso;
        }

        if (direcionado) {
            matriz[origem - 1][destino - 1] = peso;
        } else {
            int indice = calcularIndiceLinear(origem, destino);
            matrizLinear[indice] = peso;
        }
    }

    arquivo.close();
    if (vtp) delete[] pesosVertices;

    cout << "Grafo carregado com sucesso!" << endl;
}
void grafo_matriz::novo_grafo() {
    srand(time(0));
    cout << "Novo grafo gerado." << endl;
}

bool grafo_matriz::eh_direcionado() {
    return direcionado;
}

bool grafo_matriz::vertice_ponderado() {
    return vtp;
}

bool grafo_matriz::aresta_ponderada() {
    return atp;
}

int grafo_matriz::get_ordem() {
    return ordem;
}

int grafo_matriz::get_grau() {
    int grau_maximo = 0;

    if (direcionado) {
        // Grafo direcionado: usa matriz 2D
        for (int i = 0; i < ordem; ++i) {
            int grau = 0;
            for (int j = 0; j < ordem; ++j) {
                if (matriz[i][j] != 0) grau++;
            }
            grau_maximo = max(grau_maximo, grau);
        }
    } else {
        // Grafo não direcionado: usa matriz linear
        for (int i = 0; i < ordem; ++i) {
            int grau = 0;
            for (int j = 0; j < ordem; ++j) {
                if (i != j) {
                    int indice = calcularIndiceLinear(i + 1, j + 1);
                    if (matrizLinear[indice] != 0) grau++;
                }
            }
            grau_maximo = max(grau_maximo, grau);
        }
    }
    return grau_maximo;
}


int grafo_matriz::n_conexo() {
    bool* visitado = new bool[ordem]();
    int componentes = 0;

    if (direcionado) {
        // Grafo direcionado: usa matriz 2D
        auto dfs = [&](int v, auto& dfs_ref) -> void {
            visitado[v] = true;
            for (int u = 0; u < ordem; ++u) {
                if (matriz[v][u] != 0 && !visitado[u]) {
                    dfs_ref(u, dfs_ref);
                }
            }
        };

        for (int i = 0; i < ordem; ++i) {
            if (!visitado[i]) {
                componentes++;
                dfs(i, dfs);
            }
        }
    } else {
        // Grafo não direcionado: usa matriz linear
        auto dfs = [&](int v, auto& dfs_ref) -> void {
            visitado[v] = true;
            for (int u = 0; u < ordem; ++u) {
                if (u != v) {
                    int indice = calcularIndiceLinear(v + 1, u + 1);
                    if (matrizLinear[indice] != 0 && !visitado[u]) {
                        dfs_ref(u, dfs_ref);
                    }
                }
            }
        };

        for (int i = 0; i < ordem; ++i) {
            if (!visitado[i]) {
                componentes++;
                dfs(i, dfs);
            }
        }
    }

    delete[] visitado;
    return componentes;
}

bool grafo_matriz::eh_completo() {
    if (direcionado) {
        // Grafo direcionado: usa matriz 2D
        for (int i = 0; i < ordem; ++i) {
            for (int j = 0; j < ordem; ++j) {
                if (i != j && matriz[i][j] == 0) {
                    return false;
                }
            }
        }
    } else {
        // Grafo não direcionado: usa matriz linear
        for (int i = 0; i < ordem; ++i) {
            for (int j = i + 1; j < ordem; ++j) { 
                int indice = calcularIndiceLinear(i + 1, j + 1);
                if (matrizLinear[indice] == 0) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool grafo_matriz::eh_arvore() {
    // Verifica se o grafo é conexo e se não possui ciclos
    bool* visitado = new bool[ordem]();
    bool resultado = n_conexo() == 1 && !temCiclo(0, -1, visitado);
    delete[] visitado;
    return resultado;
}


bool grafo_matriz::eh_bipartido() {
    int* cor = new int[ordem];
    for (int i = 0; i < ordem; ++i) cor[i] = -1;

    for (int i = 0; i < ordem; ++i) {
        if (cor[i] == -1) {
            if (!bfsBipartido(i, cor)) {
                delete[] cor;
                return false;
            }
        }
    }

    delete[] cor;
    return true;
}

bool grafo_matriz::bfsBipartido(int inicio, int* cor) {
    cor[inicio] = 0;
    int* fila = new int[ordem];
    int frente = 0, tras = 0;
    fila[tras++] = inicio;

    while (frente != tras) {
        int v = fila[frente++];
        if (direcionado) {
            // Grafo direcionado: usa matriz 2D
            for (int u = 0; u < ordem; ++u) {
                if (matriz[v][u] != 0) { // Existe uma aresta de v para u
                    if (cor[u] == -1) {
                        cor[u] = 1 - cor[v];
                        fila[tras++] = u;
                    } else if (cor[u] == cor[v]) {
                        delete[] fila;
                        return false; // Grafo não é bipartido
                    }
                }
            }
        } else {
            // Grafo não direcionado: usa matriz linear
            for (int u = 0; u < ordem; ++u) {
                if (u != v) { // Evita autoarestas
                    int indice = calcularIndiceLinear(v + 1, u + 1);
                    if (matrizLinear[indice] != 0) { // Existe uma aresta entre v e u
                        if (cor[u] == -1) {
                            cor[u] = 1 - cor[v];
                            fila[tras++] = u;
                        } else if (cor[u] == cor[v]) {
                            delete[] fila;
                            return false; // Grafo não é bipartido
                        }
                    }
                }
            }
        }
    }

    delete[] fila;
    return true;
}


bool grafo_matriz::temCiclo(int v, int pai, bool* visitado) {
    visitado[v] = true;

    if (direcionado) {
        // Grafo direcionado: usa matriz 2D
        for (int u = 0; u < ordem; ++u) {
            if (matriz[v][u]) { // Existe uma aresta de v para u
                if (!visitado[u]) {
                    if (temCiclo(u, v, visitado)) {
                        return true;
                    }
                } else if (u != pai) { // Ciclo detectado
                    return true;
                }
            }
        }
    } else {
        // Grafo não direcionado: usa matriz linear
        for (int u = 0; u < ordem; ++u) {
            if (u != v) { // Evita autoarestas
                int indice = calcularIndiceLinear(v + 1, u + 1);
                if (matrizLinear[indice]) { // Existe uma aresta entre v e u
                    if (!visitado[u]) {
                        if (temCiclo(u, v, visitado)) {
                            return true;
                        }
                    } else if (u != pai) { // Ciclo detectado
                        return true;
                    }
                }
            }
        }
    }

    return false;
}


bool grafo_matriz::possui_articulacao() {
    // Função auxiliar para realizar uma DFS e contar componentes alcançáveis
    auto dfs = [&](int v, bool* visitado, auto& dfs_ref) -> void {
        visitado[v] = true;

        if (direcionado) {
            for (int j = 0; j < ordem; j++) {
                if (matriz[v][j] != 0 && !visitado[j]) {
                    dfs_ref(j, visitado, dfs_ref);
                }
            }
        } else {
            for (int u = 0; u < ordem; u++) {
                if (u != v) {
                    int indice = calcularIndiceLinear(v + 1, u + 1);
                    if (matrizLinear[indice] != 0 && !visitado[u]) {
                        dfs_ref(u, visitado, dfs_ref);
                    }
                }
            }
        }
    };

    for (int vertice = 0; vertice < ordem; vertice++) {
        // Cria um array dinâmico para marcar os vértices visitados
        bool* visitado = new bool[ordem];
        for (int i = 0; i < ordem; i++) {
            visitado[i] = false;
        }
        visitado[vertice] = true; 

        // Inicia a DFS a partir do primeiro vértice não removido
        int inicio = (vertice == 0) ? 1 : 0;
        dfs(inicio, visitado, dfs);

        // Verifica se todos os vértices foram visitados
        for (int i = 0; i < ordem; i++) {
            if (!visitado[i]) {
                delete[] visitado;
                return true; 
            }
        }

        delete[] visitado; 
    }
    return false; 
}

bool grafo_matriz::possui_ponte() {
    int* discovery = new int[ordem];
    int* low = new int[ordem];
    bool* visitados = new bool[ordem];
    int* parent = new int[ordem];
    bool possuiPonte = false;
    int tempo = 0;

    for (int i = 0; i < ordem; ++i) {
        visitados[i] = false;
        parent[i] = -1;
    }

    // Função auxiliar para realizar a DFS e encontrar pontes
    auto dfsPonte = [&](int u, auto& dfsPonteRef) -> void {
        visitados[u] = true;
        discovery[u] = low[u] = ++tempo;

        for (int v = 0; v < ordem; ++v) {
            bool existeAresta = false;

            if (direcionado) {
                // Verifica aresta na matriz 2D
                existeAresta = (matriz[u][v] != 0);
            } else {
                // Verifica aresta na matriz linear
                if (u != v) {
                    int indice = calcularIndiceLinear(u + 1, v + 1);
                    existeAresta = (matrizLinear[indice] != 0);
                }
            }

            if (existeAresta) {
                if (!visitados[v]) {
                    parent[v] = u;
                    dfsPonteRef(v, dfsPonteRef);

                    low[u] = std::min(low[u], low[v]);

                    if (low[v] > discovery[u]) {
                        possuiPonte = true;
                    }
                } else if (v != parent[u]) {
                    low[u] = std::min(low[u], discovery[v]);
                }
            }
        }
    };

    for (int i = 0; i < ordem; ++i) {
        if (!visitados[i]) {
            dfsPonte(i, dfsPonte);
        }
    }

    delete[] discovery;
    delete[] low;
    delete[] visitados;
    delete[] parent;

    return possuiPonte;
}
