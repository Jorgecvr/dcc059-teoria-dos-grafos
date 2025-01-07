#include "GrafoLista.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

GrafoLista::GrafoLista() {
    vertices = new ListaEncadeada<VerticeEncadeado>();
    arestas = new ListaEncadeada<ArestaEncadeada>();
    ordem = 0;
    direcionado = false;
    verticePonderado = false;
    arestaPonderada = false;
}

GrafoLista::~GrafoLista() {
    delete vertices;
    delete arestas;
}

void GrafoLista::imprimir() {
    std::cout << "Vertices:" << std::endl;
    if (vertices != nullptr) {
        vertices->imprimir();
    } else {
        std::cout << "Nenhum vertice." << std::endl;
    }

    std::cout << "Arestas:" << std::endl;
    if (arestas != nullptr) {
        arestas->imprimir();
    } else {
        std::cout << "Nenhuma aresta." << std::endl;
    }

    std::cout << "Grau do grafo: " << get_grau() << std::endl;
    std::cout << "Eh completo? " << (eh_completo() ? "Sim" : "Nao") << std::endl;
    std::cout << "Quantidade de componentes conexas: " << n_conexo() << std::endl;
}

void GrafoLista::carrega_grafo() {
    ifstream arquivo("Grafo.txt");
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo Grafo.txt" << endl;
        return;
    }

    string linha;
    if (getline(arquivo, linha)) {
        istringstream iss(linha);
        int direcionadoInput;
        iss >> ordem >> direcionadoInput >> verticePonderado >> arestaPonderada;
        direcionado = direcionadoInput;
        for (int i = 1; i <= ordem; i++) {
            adicionarVertice(i, 0);
        }
    }

    while (getline(arquivo, linha)) {
        istringstream iss(linha);
        int origem, destino, peso = 1;
        iss >> origem >> destino;
        if (arestaPonderada) {
            iss >> peso;
        }
        adicionarAresta(origem, destino, peso);
    }
    arquivo.close();
}

void GrafoLista::adicionarVertice(int id, float peso) {
    VerticeEncadeado* novo = new VerticeEncadeado(id, peso);
    vertices->adicionar(novo);
}

void GrafoLista::adicionarAresta(int origem, int destino, int peso) {
    VerticeEncadeado* verticeOrigem = encontraVertice(origem);
    VerticeEncadeado* verticeDestino = encontraVertice(destino);
    if (verticeOrigem && verticeDestino) {
        ArestaEncadeada* novaAresta = new ArestaEncadeada(verticeOrigem, verticeDestino, peso);
        arestas->adicionar(novaAresta);
    }
}

bool GrafoLista::eh_bipartido() {
    return false;
}

int GrafoLista::n_conexo() {
    return 0;
}

int GrafoLista::get_grau() {
    return 0;
}

bool GrafoLista::eh_completo() {
    return false;
}

bool GrafoLista::eh_arvore() {
    int ordem = get_ordem();
    if (ordem == 0) return false;

    bool* visitados = new bool[ordem];
    for (int i = 0; i < ordem; ++i) {
        visitados[i] = false;
    }

    bool aciclico = true;

    auto dfs = [&](VerticeEncadeado* vertice, VerticeEncadeado* pai, auto& dfsRef) -> void {
        visitados[vertice->getId()] = true;

        ArestaEncadeada* aresta = vertice->getPrimeiraConexao();
        while (aresta != nullptr) {
            VerticeEncadeado* vizinho = aresta->getDestino();

            if (!visitados[vizinho->getId()]) {
                dfsRef(vizinho, vertice, dfsRef);
            } else if (vizinho != pai) {
                aciclico = false;
            }

            aresta = aresta->getProximo();
        }
    };

    VerticeEncadeado* verticeInicial = encontraVertice(0);
    dfs(verticeInicial, nullptr, dfs);

    for (int i = 0; i < ordem; ++i) {
        if (!visitados[i]) {
            delete[] visitados;
            return false;
        }
    }

    delete[] visitados;
    return aciclico;
}

bool GrafoLista::possui_articulacao() {
    int ordem = get_ordem();
    if (ordem == 0) return false;

    bool* visitados = new bool[ordem];
    int* discovery = new int[ordem];
    int* low = new int[ordem];
    int* parent = new int[ordem];
    bool* articulacao = new bool[ordem];

    for (int i = 0; i < ordem; i++) {
        visitados[i] = false;
        parent[i] = -1;
        articulacao[i] = false;
    }

    int tempo = 0;

    auto dfsArticulacao = [&](VerticeEncadeado* vertice, auto& dfsArticulacaoRef) -> void {
        int u = vertice->getId();
        visitados[u] = true;
        discovery[u] = low[u] = ++tempo;
        int filhos = 0;

        ArestaEncadeada* aresta = vertice->getPrimeiraConexao();
        while (aresta != nullptr) {
            VerticeEncadeado* vizinho = aresta->getDestino();
            int v = vizinho->getId();

            if (!visitados[v]) {
                filhos++;
                parent[v] = u;
                dfsArticulacaoRef(vizinho, dfsArticulacaoRef);

                low[u] = std::min(low[u], low[v]);

                if (parent[u] == -1 && filhos > 1)
                    articulacao[u] = true;
                if (parent[u] != -1 && low[v] >= discovery[u])
                    articulacao[u] = true;
            } else if (v != parent[u]) {
                low[u] = std::min(low[u], discovery[v]);
            }

            aresta = aresta->getProximo();
        }
    };

    for (int i = 0; i < ordem; i++) {
        if (!visitados[i]) {
            dfsArticulacao(encontraVertice(i), dfsArticulacao);
        }
    }

    bool possuiArticulacao = false;
    for (int i = 0; i < ordem; i++) {
        if (articulacao[i]) {
            possuiArticulacao = true;
            break;
        }
    }

    delete[] visitados;
    delete[] discovery;
    delete[] low;
    delete[] parent;
    delete[] articulacao;

    return possuiArticulacao;
}

bool GrafoLista::possui_ponte() {
    return false;
}

void GrafoLista::novo_grafo() {
    cout << "Novo grafo gerado." << endl;
}

VerticeEncadeado* GrafoLista::encontraVertice(int id) {
    VerticeEncadeado* atual = vertices->getInicio();
    while (atual != nullptr) {
        if (atual->getId() == id) {
            return atual;
        }
        atual = atual->getProximo();
    }
    return nullptr;
}

void GrafoLista::buscaEmProfundidade(VerticeEncadeado* vertice, bool* visitados) {
    visitados[vertice->getId()] = true;
    ArestaEncadeada* aresta = vertice->getPrimeiraConexao();
    while (aresta != nullptr) {
        VerticeEncadeado* vizinho = aresta->getDestino();
        if (!visitados[vizinho->getId()]) {
            buscaEmProfundidade(vizinho, visitados);
        }
        aresta = aresta->getProximo();
    }
}
