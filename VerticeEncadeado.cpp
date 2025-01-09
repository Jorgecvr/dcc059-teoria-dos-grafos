#include "VerticeEncadeado.h"

VerticeEncadeado::VerticeEncadeado(int id, int peso)
    : id(id), peso(peso), grau(0), proximo(nullptr), conexoes(new ListaEncadeada<ArestaEncadeada>()) {}

int VerticeEncadeado::getId() const {
    return id;
}

int VerticeEncadeado::getPeso() const {
    return peso;
}

int VerticeEncadeado::getGrau() const {
    return grau;
}

VerticeEncadeado* VerticeEncadeado::getProximo() const {
    return proximo;
}

void VerticeEncadeado::setConexao(VerticeEncadeado* verticeDestino, int pesoAresta, bool arestaDuplicada) {

    ArestaEncadeada* novaAresta = new ArestaEncadeada(this, verticeDestino, pesoAresta);
    conexoes->adicionar(novaAresta);

    if(!arestaDuplicada)
    grau++;
}

ArestaEncadeada* VerticeEncadeado::getPrimeiraConexao() {
    return conexoes->getInicio();
}

ListaEncadeada<ArestaEncadeada>* VerticeEncadeado::getConexoes() {
    return conexoes;
}

ArestaEncadeada* VerticeEncadeado::getConexao(int origem, int destino) {
    ArestaEncadeada* arestaAtual = conexoes->getInicio();
    
    while(arestaAtual != nullptr) {
        if(arestaAtual->getOrigem()->getId() == origem && arestaAtual->getDestino()->getId() == destino) 
            break;
 
        arestaAtual = arestaAtual->getProximo();
    }
    return arestaAtual;

}

void VerticeEncadeado::setProximo(VerticeEncadeado* novoProximo) {
    proximo = novoProximo;
}

void VerticeEncadeado::setConexoes(ListaEncadeada<ArestaEncadeada>* novasConexoes) {
    conexoes = novasConexoes;
}

int VerticeEncadeado::removeConexao(VerticeEncadeado* destino) {

    ArestaEncadeada* aresta = getConexao(id, destino->getId());
    if(aresta == nullptr)
        return 0;
        
    int pesoAresta = aresta->getPeso();
    conexoes->remover(aresta);
    return pesoAresta;

}


std::ostream& operator<<(std::ostream& os, const VerticeEncadeado& vertice) {
    os << "VerticeEncadeado " << vertice.id
       << " (Peso: " << vertice.peso << ", Grau: " << vertice.grau << ")\n";
    os << "Conexoes: ";
    vertice.conexoes->imprimir();
    return os;
}
