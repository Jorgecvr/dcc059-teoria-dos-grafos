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
            matriz[origem - 1][destino - 1] = peso;
            matriz[destino - 1][origem - 1] = peso;
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

    for (int i = 0; i < ordem; ++i) {
        int grau = 0;
        for (int j = 0; j < ordem; ++j) {
            if (matriz[i][j] != 0) grau++;
        }
        grau_maximo = max(grau_maximo, grau);
    }

    return grau_maximo;
}

int grafo_matriz::n_conexo() {
    bool* visitado = new bool[ordem]();
    int componentes = 0;

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

    delete[] visitado;
    return componentes;
}

bool grafo_matriz::eh_completo() {
    for (int i = 0; i < ordem; ++i) {
        for (int j = 0; j < ordem; ++j) {
            if (i != j && matriz[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

bool grafo_matriz::eh_arvore() {
    return n_conexo() == 1 && !temCiclo(0, -1, new bool[ordem]());
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
        for (int u = 0; u < ordem; ++u) {
            if (matriz[v][u] != 0) {
                if (cor[u] == -1) {
                    cor[u] = 1 - cor[v];
                    fila[tras++] = u;
                } else if (cor[u] == cor[v]) {
                    delete[] fila;
                    return false;
                }
            }
        }
    }

    delete[] fila;
    return true;
}

bool grafo_matriz::temCiclo(int v, int pai, bool* visitado) {
    visitado[v] = true;

    for (int u = 0; u < ordem; ++u) {
        if (matriz[v][u]) {
            if (!visitado[u]) {
                if (temCiclo(u, v, visitado)) {
                    return true;
                }
            } else if (u != pai) {
                return true;
            }
        }
    }
    return false;
}

bool grafo_matriz::possui_articulacao() {
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
            if (matriz[u][v] != 0) {  // Se houver uma aresta entre u e v
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