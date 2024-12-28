#ifndef VERTICEENCADEADO_H_INCLUDED
#define VERTICEENCADEADO_H_INCLUDED

#include <iostream>
#include "ListaEncadeada.h"
#include "ArestaEncadeada.h"

class VerticeEncadeado {
private:
    int id;
    int peso;
    int grau;
    VerticeEncadeado* proximo;
    ListaEncadeada<ArestaEncadeada>* conexoes;

public:
    VerticeEncadeado(int id, int peso);

    int getId() const;
    int getPeso() const;
    int getGrau() const;
    VerticeEncadeado* getProximo() const;
    void setProximo(VerticeEncadeado* novoProximo);
    void setConexao(VerticeEncadeado* verticeDestino, int pesoAresta);
    ArestaEncadeada* getPrimeiraConexao();

    friend std::ostream& operator<<(std::ostream& os, const VerticeEncadeado& vertice);
};

#endif // VERTICEENCADEADO_H_INCLUDED
