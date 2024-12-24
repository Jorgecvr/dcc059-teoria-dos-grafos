#ifndef LISTAENCADEADA_H
#define LISTAENCADEADA_H

#include <iostream>

#include "No.h"

using namespace std;

template <typename T>

class ListaEncadeada {
private:
    No<T>* inicio; 

public:
    ListaEncadeada() : inicio(nullptr) {}


    No<T>* getInicio() const {
        return inicio;
    }

void adicionar(const T valor) {
    No<T>* novoNo = new No<T>(valor, inicio);
    inicio = novoNo;
}

    void imprimir() const {
        No<T>* atual = inicio;
        while (atual != nullptr) {
            cout << atual->getValor() << endl;
            atual = atual->getProximo();
        }
    }

    ~ListaEncadeada() {
        No<T>* atual = inicio;
        while (atual != nullptr) {
            No<T>* proximo = atual->getProximo();
            delete atual;
            atual = proximo;
        }
    }
};

#endif