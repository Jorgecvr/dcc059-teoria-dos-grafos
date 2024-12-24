#ifndef ARESTAENCADEADA_H
#define ARESTAENCADEADA_H

#include <iostream>
#include "No.h"
#include "VerticeEncadeado.h"

using namespace std;

class VerticeEncadeado;

class ArestaEncadeada {
private:
    No<VerticeEncadeado>* origem;
    No<VerticeEncadeado>* destino;
    int peso;
    No<ArestaEncadeada>* proximo;

public:
    ArestaEncadeada(No<VerticeEncadeado>* origem, No<VerticeEncadeado>* destino, int peso)
        : origem(origem), destino(destino), peso(peso), proximo(nullptr) {}

    No<VerticeEncadeado>* getOrigem() const {
        return origem;
    }

    No<VerticeEncadeado>* getDestino() const {
        return destino;
    }

    int getPeso() const {
        return peso;
    }

    friend ostream& operator<<(ostream& os, const ArestaEncadeada& aresta) {
        os << "Origem: " << aresta.origem->getValor().getId()
            << ", Destino: " << aresta.destino->getValor().getId()
            << ", Peso: " << aresta.peso;
        return os;
    }
};

#endif

