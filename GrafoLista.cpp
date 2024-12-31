#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include "GrafoLista.h"

#include <unordered_map>
#include <queue>  

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
bool GrafoLista::possui_ponte(){
    ArestaEncadeada *arestaAtual = aresta ->getinicio();

    while(arestaAtual != nullptr){
        VerticeEncadeado *origem = aresta->getOrigem();
        VerticeEncadeado *destino = aresta->getDestino();

        origem->removeConexao(destino);
        if (!eh_direcionado()){
            destino->removerConexao(origem);
        }
        

    }



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

bool eh_bipartido(VerticeEncadeado* inicio) {
    if (!inicio) return true; // Um grafo vazio é considerado bipartido.

    std::unordered_map<int, int> cores; // Mapeia ID do vértice para sua cor (0 ou 1).
    std::queue<VerticeEncadeado*> fila;

    fila.push(inicio);
    cores[inicio->getId()] = 0; // Atribui a cor inicial ao primeiro vértice.

    while (!fila.empty()) {
        VerticeEncadeado* atual = fila.front();
        fila.pop();

        int corAtual = cores[atual->getId()];
        ArestaEncadeada* aresta = atual->getPrimeiraConexao();

        while (aresta) {
            VerticeEncadeado* vizinho = aresta->getDestino();
            int vizinhoId = vizinho->getId();

            // Se o vizinho ainda não foi colorido, atribui a cor oposta.
            if (cores.find(vizinhoId) == cores.end()) {
                cores[vizinhoId] = 1 - corAtual;
                fila.push(vizinho);
            } 
            // Se o vizinho já está colorido e tem a mesma cor, o grafo não é bipartido.
            else if (cores[vizinhoId] == corAtual) {
                return false;
            }

            aresta = aresta->getProximo();
        }
    }

    return true; // Se percorremos todo o grafo sem conflitos, ele é bipartido.
}

bool GrafoLista::eh_arvore() {
    bool *visitados = new bool[get_ordem()];
    return false;
}

bool eh_ciclico(VerticeEncadeado vertice_atual, bool visitados[], VerticeEncadeado pai)
{
    
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
