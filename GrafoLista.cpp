#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "GrafoLista.h"

using namespace std;

GrafoLista::GrafoLista() {
    vertices = new ListaEncadeada<VerticeEncadeado>();
    arestas = new ListaEncadeada<ArestaEncadeada>();
}

int ordem;
bool vtp;
bool atp;

void GrafoLista::carrega_grafo() {
    std::ifstream arquivo("Grafo.txt");

    if (!arquivo.is_open()) {
        throw std::runtime_error("Erro ao abrir o arquivo: Grafo.txt");
    }

    std::string linha;


    if (std::getline(arquivo, linha)) {
        std::istringstream iss(linha);
        iss >> ordem >> direcionado >> vtp >> atp;

        this->ordem = ordem;
        set_eh_direcionado(direcionado);
        set_vertice_ponderado(vtp);
        set_aresta_ponderada(atp);

    } else {
        throw std::runtime_error("Arquivo vazio ou formato invalido na primeira linha.");
    }
    if (vertice_ponderado()) {
        if (std::getline(arquivo, linha)) {
            std::istringstream streamPesos(linha);
            for (int i = 1; i <= ordem; i++) {
                float pesoVertice;
                if (streamPesos >> pesoVertice) {
                    adicionarVertice(i, pesoVertice);
                } else {
                    throw std::runtime_error("Erro ao ler pesos dos vertices.");
                }
            }
        } else {
            throw std::runtime_error("Pesos dos vertices não encontrados em arquivo ponderado.");
        }
    } else {
        for (int i = 1; i <= ordem; i++) {
            adicionarVertice(i, 0);
        }
    }

    while (std::getline(arquivo, linha)) {
        std::istringstream streamAresta(linha);
        int origem, destino;
        int peso = 0;

        if (streamAresta >> origem >> destino) {
            if (aresta_ponderada() && !(streamAresta >> peso)) {
                throw std::runtime_error("Erro ao ler peso da aresta em grafo ponderado.");
            }
            adicionarAresta(origem, destino, peso);
        } else {
            throw std::runtime_error("Erro ao ler aresta (origem e destino).");
        }
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
        cout << "VerticeEncadeado com ID " << id << " ja existe!" << endl;
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
    if(!eh_direcionado())
        verticeDestino->setConexao(verticeOrigem, peso);
    arestas->adicionar(novaAresta);
}

int GrafoLista::get_grau() {
    VerticeEncadeado* atual = vertices->getInicio();

    int maiorGrau = 0;
    while (atual != nullptr) {
        if(maiorGrau < atual->getGrau()) {
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

    while(v1 != nullptr) {
        if(v1->getGrau() < get_ordem() - 1) {
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
    cout << "grau do grafo: " << get_grau() <<"\n";
    cout << "Eh completo? " << eh_completo() <<"\n";
    cout << "Quantidade de componente conexas: " <<n_conexo();

}

bool GrafoLista::eh_bipartido() {
    int* cores = new int[ordem + 1];
    for (int i = 1; i <= ordem; i++) {
        cores[i] = -1;
    }

    // Pilha simulada para a DFS
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
                        // Atribui a cor oposta ao vizinho
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
    return false;
}

bool GrafoLista::possui_ponte() {
    return false;
}

void GrafoLista::novo_grafo() {

}

GrafoLista::~GrafoLista() {
    delete vertices;
    delete arestas;
}