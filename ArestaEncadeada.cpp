#include "ArestaEncadeada.h"
#include "VerticeEncadeado.h"
#include <iostream>

ArestaEncadeada::ArestaEncadeada(VerticeEncadeado* origem, VerticeEncadeado* destino, int peso)
    : origem(origem), destino(destino), peso(peso), proximo(nullptr) {}

VerticeEncadeado* ArestaEncadeada::getOrigem() const {
    return origem;
}

VerticeEncadeado* ArestaEncadeada::getDestino() const {
    return destino;
}

int ArestaEncadeada::getPeso() const {
    return peso;
}

ArestaEncadeada* ArestaEncadeada::getProximo() const {
    return proximo;
}

void ArestaEncadeada::setProximo(ArestaEncadeada* novoProximo) {
    proximo = novoProximo;
}


std::ostream& operator<<(std::ostream& os, const ArestaEncadeada& aresta) {
    os << "Origem: " << aresta.getOrigem()->getId()
       << ", Destino: " << aresta.getDestino()->getId()
       << ", Peso: " << aresta.getPeso();
    return os;
}
