#include <iostream>
#include <fstream>
#include "GrafoLista.h"

using namespace std;

GrafoLista::GrafoLista() {
    vertices = new ListaEncadeada<VerticeEncadeado>();
    arestas = new ListaEncadeada<ArestaEncadeada>();
}

int ordem;
int vtp;
int atp;

void GrafoLista::carrega_grafo() {
    std::ifstream arquivo("Grafo.txt");

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: Grafo.txt" << endl;
        std::exit(EXIT_FAILURE);
    }

    std::string linha;

    if (std::getline(arquivo, linha)) {
        int direcionado;
        if (std::sscanf(linha.c_str(), "%d %d %d %d", &ordem, &direcionado, &vtp, &atp) != 4) {
            cerr << "Formato inválido na primeira linha." << endl;
            std::exit(EXIT_FAILURE);
        }

        this->ordem = ordem;
        set_eh_direcionado(direcionado);
        set_vertice_ponderado(vtp);
        set_aresta_ponderada(atp);
    } else {
        cerr << "Arquivo vazio ou formato inválido na primeira linha." << endl;
        std::exit(EXIT_FAILURE);
    }

    if (vertice_ponderado()) {
        if (std::getline(arquivo, linha)) {
            size_t pos = 0;
            for (int i = 1; i <= ordem; i++) {
                size_t nextPos;
                try {
                    float pesoVertice = std::stof(linha.substr(pos), &nextPos);
                    pos += nextPos;
                    adicionarVertice(i, pesoVertice);
                } catch (...) {
                    cerr << "Erro ao converter peso dos vértices." << endl;
                    std::exit(EXIT_FAILURE);
                }
            }
        } else {
            cerr << "Pesos dos vértices não encontrados em arquivo ponderado." << endl;
            std::exit(EXIT_FAILURE);
        }
    } else {
        for (int i = 1; i <= ordem; i++) {
            adicionarVertice(i, 0);
        }
    }

    while (std::getline(arquivo, linha)) {
        int origem, destino, peso = 0;
        if (aresta_ponderada()) {
            if (std::sscanf(linha.c_str(), "%d %d %d", &origem, &destino, &peso) < 3) {
                cerr << "Erro ao ler peso da aresta em grafo ponderado." << endl;
                std::exit(EXIT_FAILURE);
            }
        } else {
            if (std::sscanf(linha.c_str(), "%d %d", &origem, &destino) < 2) {
                cerr << "Erro ao ler aresta (origem e destino)." << endl;
                std::exit(EXIT_FAILURE);
            }
        }
        adicionarAresta(origem, destino, peso);
    }

    arquivo.close();
}

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

void GrafoLista::adicionarVertice(int id, float peso) {
    if (encontraVertice(id) != nullptr) {
        cout << "VerticeEncadeado com ID " << id << " já existe!" << endl;
        return;
    }
    VerticeEncadeado* novoVertice = new VerticeEncadeado(id, peso);
    vertices->adicionar(novoVertice);
}

void GrafoLista::adicionarAresta(int origem, int destino, int peso) {
    ArestaEncadeada* atual = arestas->getInicio();
    while (atual != nullptr) {
        if (atual->getOrigem()->getId() == origem &&
            atual->getDestino()->getId() == destino) {
            cout << "Aresta entre " << origem << " e " << destino << " já existe!" << endl;
            return;
        }
        atual = atual->getProximo();
    }

    VerticeEncadeado* verticeOrigem = encontraVertice(origem);
    VerticeEncadeado* verticeDestino = encontraVertice(destino);
    if (verticeOrigem == nullptr || verticeDestino == nullptr) {
        cout << "Erro: Um ou ambos os vértices não existem!" << endl;
        return;
    }

    ArestaEncadeada* novaAresta = new ArestaEncadeada(verticeOrigem, verticeDestino, peso);

    verticeOrigem->setConexao(verticeDestino, peso, false);
    if (!eh_direcionado())
        verticeDestino->setConexao(verticeOrigem, peso, true);
    arestas->adicionar(novaAresta);
}

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

int GrafoLista::n_conexo() {
    int ordem = this->ordem;
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

void GrafoLista::imprimir() {
    cout << "Vertices:\n";
    vertices->imprimir();
    cout << "Arestas:\n";
    arestas->imprimir();
    cout << "\n";
    cout << "grau do grafo: " << get_grau() << "\n";
    cout << "Eh completo? " << eh_completo() << "\n";
    cout << "Quantidade de componente conexas: " << n_conexo();
}

bool GrafoLista::eh_bipartido() {
    int* cores = new int[ordem + 1];
    for (int i = 1; i <= ordem; i++) {
        cores[i] = -1;
    }

    int* pilha = new int[ordem];
    int topo = -1;

    for (int i = 1; i <= ordem; i++) {
        if (cores[i] == -1) {
            pilha[++topo] = i;
            cores[i] = 0;

            while (topo >= 0) {
                int idAtual = pilha[topo--];
                VerticeEncadeado* verticeAtual = encontraVertice(idAtual);

                if (!verticeAtual) continue;

                ArestaEncadeada* conexao = verticeAtual->getPrimeiraConexao();
                while (conexao != nullptr) {
                    VerticeEncadeado* vizinho = conexao->getDestino();
                    int idVizinho = vizinho->getId();

                    if (cores[idVizinho] == -1) {
                        cores[idVizinho] = 1 - cores[idAtual];
                        pilha[++topo] = idVizinho;
                    } else if (cores[idVizinho] == cores[idAtual]) {
                        delete[] cores;
                        delete[] pilha;
                        return false;
                    }

                    conexao = conexao->getProximo();
                }
            }
        }
    }

    delete[] cores;
    delete[] pilha;
    return true;
}

bool GrafoLista::eh_arvore() {
    if (n_conexo() != 1) {
        return false;
    }

    int quantidadeArestas = 0;
    ArestaEncadeada* arestaAtual = arestas->getInicio();
    while (arestaAtual != nullptr) {
        quantidadeArestas++;
        arestaAtual = arestaAtual->getProximo();
    }

    if (quantidadeArestas != (ordem - 1)) {
        return false;
    }
    return true;
}


bool GrafoLista::possui_articulacao() {
    VerticeEncadeado* verticeAtual = vertices->getInicio();

    while (verticeAtual != nullptr) {
        int idAtual = verticeAtual->getId();

        bool* visitados = new bool[ordem + 1];
        for (int i = 1; i <= ordem; i++) {
            visitados[i] = false;
        }

        int componentesConexos = 0;
        visitados[idAtual] = true;

        VerticeEncadeado* vertice = vertices->getInicio();
        while (vertice != nullptr) {
            if (!visitados[vertice->getId()]) {
                buscaEmProfundidade(vertice, visitados);
                componentesConexos++;
            }
            vertice = vertice->getProximo();
        }

        delete[] visitados;

        if (componentesConexos > 1)
            return true;

        verticeAtual = verticeAtual->getProximo();
    }
    return false;
}



bool GrafoLista::possui_ponte(){

    ArestaEncadeada* atual = arestas->getInicio();
    while (atual != nullptr) {
        VerticeEncadeado* origem = atual->getOrigem();
        VerticeEncadeado* destino = atual->getDestino();

        int componentesAntes = n_conexo();
        
        int pesoAresta = origem->removeConexao(destino);
        if (!eh_direcionado()){
            destino->removeConexao(origem);
        }
        int componentesDepois = n_conexo(); 

        origem->setConexao(destino, pesoAresta, false);
        if (!eh_direcionado()){
            destino->setConexao(origem, pesoAresta, true);
        }
        if (componentesDepois > componentesAntes) {
            return true;
        }

        atual = atual->getProximo();
    }
    return false;
}

void GrafoLista::novo_grafo() {}

GrafoLista::~GrafoLista() {
    delete vertices;
    delete arestas;
}
