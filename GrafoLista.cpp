#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "GrafoLista.h"
#include <unordered_map>
#include <queue>
#include <functional>  // Necessário para usar std::function


using namespace std;

// Construtor
GrafoLista::GrafoLista() {
    vertices = new ListaEncadeada<VerticeEncadeado>();
    arestas = new ListaEncadeada<ArestaEncadeada>();
    ordem = 0; // Inicializa a ordem
    direcionado = false;
    verticePonderado = false;
    arestaPonderada = false;
}

// Implementação da função para carregar o grafo
void GrafoLista::carrega_grafo() {
    std::ifstream arquivo("Grafo.txt");

    if (!arquivo.is_open()) {
        throw std::runtime_error("Erro ao abrir o arquivo: Grafo.txt");
    }

    std::string linha;
    if (std::getline(arquivo, linha)) {
        std::istringstream iss(linha);
        int direcionadoInput;

        iss >> this->ordem >> direcionadoInput >> this->vTP >> this->aTP;

        set_eh_direcionado(direcionadoInput);
        set_vertice_ponderado(this->vTP);
        set_aresta_ponderada(this->aTP);
    } else {
        throw std::runtime_error("Arquivo vazio ou formato inválido na primeira linha.");
    }

    while (std::getline(arquivo, linha)) {
        std::istringstream streamAresta(linha);
        int origem, destino, peso = 0;

        if (streamAresta >> origem >> destino) {
            if (aresta_ponderada() && !(streamAresta >> peso)) {
                throw std::runtime_error("Erro ao ler peso da aresta em grafo ponderado.");
            }
            adicionarAresta(origem, destino, peso);
        }
    }

    arquivo.close();
}

// Função para encontrar um vértice pelo ID
VerticeEncadeado* GrafoLista::encontraVertice(int id) {
    VerticeEncadeado* vertice = vertices->getInicio();

    while (vertice != nullptr) {
        if (vertice->getId() == id) {
            return vertice;
        }
        vertice = vertice->getProximo();
    }
    return nullptr;
}

// Função para adicionar um vértice
void GrafoLista::adicionarVertice(int id, float peso) {
    if (encontraVertice(id) != nullptr) {
        cout << "VerticeEncadeado com ID " << id << " ja existe!" << endl;
        return;
    }
    VerticeEncadeado* novoVertice = new VerticeEncadeado(id, peso);
    vertices->adicionar(novoVertice);
}

// Função para adicionar uma aresta
void GrafoLista::adicionarAresta(int origem, int destino, int peso) {
    ArestaEncadeada* atual = arestas->getInicio();
    while (atual != nullptr) {
        if (atual->getOrigem()->getId() == origem &&
            atual->getDestino()->getId() == destino) {
            cout << "Aresta entre " << origem << " e " << destino << " ja existe!" << endl;
            return;
        }
        atual = atual->getProximo();
    }

    VerticeEncadeado* verticeOrigem = encontraVertice(origem);
    VerticeEncadeado* verticeDestino = encontraVertice(destino);
    if (verticeOrigem == nullptr || verticeDestino == nullptr) {
        cout << "Erro: Um ou ambos os vertices nao existem!" << endl;
        return;
    }

    ArestaEncadeada* novaAresta = new ArestaEncadeada(verticeOrigem, verticeDestino, peso);

    verticeOrigem->setConexao(verticeDestino, peso);
    if (!eh_direcionado())
        verticeDestino->setConexao(verticeOrigem, peso);
    arestas->adicionar(novaAresta);
}

// Função para calcular o maior grau
int GrafoLista::get_grau() {
    VerticeEncadeado* atual = vertices->getInicio();

    int maiorGrau = 0;
    while (atual != nullptr) {
        if (maiorGrau < atual->getGrau()) {
            maiorGrau = atual->getGrau();
        }
        atual = atual->getProximo();
    }

    return maiorGrau;
}

// Função para verificar se o grafo é completo
bool GrafoLista::eh_completo() {
    if (ordem < 2) {
        return true;
    }

    VerticeEncadeado* v1 = vertices->getInicio();

    while (v1 != nullptr) {
        if (v1->getGrau() < get_ordem() - 1) {
            return false;
        }
        v1 = v1->getProximo();
    }

    return true;
}

// Busca em profundidade
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

// Função para calcular o número de componentes conexas
int GrafoLista::n_conexo() {
    bool* visitados = new bool[ordem + 1];
    for (int i = 1; i <= ordem; i++) {
        visitados[i] = false;
    }

    int componentes = 0;

    VerticeEncadeado* vertice = vertices->getInicio();
    while (vertice != nullptr) {
        if (!visitados[vertice->getId()]) {
            buscaEmProfundidade(vertice, visitados);
            componentes++;
        }
        vertice = vertice->getProximo();
    }

    delete[] visitados;

    return componentes;
}

// Função para imprimir o grafo
void GrafoLista::imprimir() {
    cout << "Vertices:\n";
    vertices->imprimir();
    cout << "Arestas:\n";
    arestas->imprimir();
    cout << "\n";
    cout << "grau do grafo: " << get_grau() << "\n";
    cout << "Eh completo? " << eh_completo() << "\n";
    cout << "Quantidade de componente conexas: " << n_conexo() << "\n";
    cout << "Possui Ponte: " << possui_ponte();
}

bool GrafoLista::possui_ponte(){
    // Para cada aresta no grafo, verificamos se sua remoção aumenta o número de componentes conexas
    ArestaEncadeada* atual = arestas->getInicio();
    while (atual != nullptr) {
        // Removemos temporariamente a aresta
        VerticeEncadeado* origem = atual->getOrigem();
        VerticeEncadeado* destino = atual->getDestino();
        origem->removerConexao(destino);
        if (!eh_direcionado()){
            destino->removerConexao(origem);
        }
        int componentesAntes = n_conexo(); // Número de componentes conexas antes
        adicionarAresta(origem->getId(), destino->getId(), atual->getPeso());
        int componentesDepois = n_conexo(); // Número de componentes conexas depois de restaurar
        if (componentesDepois > componentesAntes) {
            return true;
        }
        atual = atual->getProximo();
    }
    return false;
}




// Implementações das funções virtuais puras
bool GrafoLista::eh_bipartido() {
    return false;
}

// Função para verificar se o grafo é uma árvore
bool GrafoLista::eh_arvore(VerticeEncadeado* verticeInicial) {
    bool* visitados = new bool[get_ordem()];
    for (int i = 0; i < get_ordem(); i++) {
        visitados[i] = false;
    }

    bool aciclico = true;

    std::function<void(VerticeEncadeado*, VerticeEncadeado*)> buscaComVerificacaoDeCiclo;

    buscaComVerificacaoDeCiclo = [&](VerticeEncadeado* vertice, VerticeEncadeado* pai) {
        visitados[vertice->getId()] = true;

        ArestaEncadeada* aresta = vertice->getPrimeiraConexao();
        
        while (aresta != nullptr) {
            VerticeEncadeado* vizinho = aresta->getDestino();

            if (!visitados[vizinho->getId()]) {
                buscaComVerificacaoDeCiclo(vizinho, vertice);
            }
            else if (vizinho != pai) {
                aciclico = false;
            }

            aresta = aresta->getProximo();
        }
    };

    buscaComVerificacaoDeCiclo(verticeInicial, nullptr);

    for (int i = 0; i < get_ordem(); i++) {
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

    std::function<void(int)> dfsArticulacao = [&](int u) {
        visitados[u] = true;
        discovery[u] = low[u] = ++tempo;
        int filhos = 0;

        VerticeEncadeado* vertice = encontraVertice(u);  // Substituído por encontraVertice
        ArestaEncadeada* aresta = vertice->getPrimeiraConexao();

        while (aresta != nullptr) {
            int v = aresta->getDestino()->getId();

            if (!visitados[v]) {
                filhos++;
                parent[v] = u;
                dfsArticulacao(v);

                low[u] = std::min(low[u], low[v]);

                if (parent[u] == -1 && filhos > 1)
                    articulacao[u] = true;

                if (parent[u] != -1 && low[v] >= discovery[u])
                    articulacao[u] = true;
            }
            else if (v != parent[u]) {
                low[u] = std::min(low[u], discovery[v]);
            }

            aresta = aresta->getProximo();
        }
    };

    for (int i = 0; i < ordem; i++) {
        if (!visitados[i]) {
            dfsArticulacao(i);
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

void GrafoLista::novo_grafo() {
    cout << "Novo grafo gerado." << endl;
}

// Destrutor
GrafoLista::~GrafoLista() {
    delete vertices;
    delete arestas;
}
