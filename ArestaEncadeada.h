#ifndef ARESTAENCADEADA_H_INCLUDED
#define ARESTAENCADEADA_H_INCLUDED

#include <iostream>

class VerticeEncadeado;

class ArestaEncadeada {
private:
    VerticeEncadeado* origem;
    VerticeEncadeado* destino;
    int peso;
    ArestaEncadeada* proximo;

public:
    ArestaEncadeada(VerticeEncadeado* origem, VerticeEncadeado* destino, int peso);

    VerticeEncadeado* getOrigem() const;
    VerticeEncadeado* getDestino() const;
    int getPeso() const;
    ArestaEncadeada* getProximo() const;
    void setProximo(ArestaEncadeada* novoProximo);

    friend std::ostream& operator<<(std::ostream& os, const ArestaEncadeada& aresta);
};

#endif