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

void VerticeEncadeado::setConexao(VerticeEncadeado* verticeDestino, int pesoAresta) {

    ArestaEncadeada* novaAresta = new ArestaEncadeada(this, verticeDestino, pesoAresta);
    conexoes->adicionar(novaAresta);
    grau++;
}

ArestaEncadeada* VerticeEncadeado::getPrimeiraConexao() {

    return conexoes->getInicio();
}

ListaEncadeada<ArestaEncadeada>* VerticeEncadeado::getConexoes() {
    return conexoes;
}

void VerticeEncadeado::setProximo(VerticeEncadeado* novoProximo) {
    proximo = novoProximo;
}

void VerticeEncadeado::setConexoes(ListaEncadeada<ArestaEncadeada>* novasConexoes) {
    conexoes = novasConexoes;
}


std::ostream& operator<<(std::ostream& os, const VerticeEncadeado& vertice) {
    os << "VerticeEncadeado " << vertice.id
       << " (Peso: " << vertice.peso << ", Grau: " << vertice.grau << ")\n";
    os << "Conexoes: ";
    vertice.conexoes->imprimir();
    return os;
}
