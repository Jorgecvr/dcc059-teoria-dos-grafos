#ifndef LISTAENCADEADA_H
#define LISTAENCADEADA_H

#include <iostream>

using namespace std;

template <typename T>

class ListaEncadeada {
private:
    T* primeiro;
    T* ultimo;


public:
    ListaEncadeada() : primeiro(nullptr), ultimo(nullptr) {}

    T* getInicio() const {
        return primeiro;
    }

    void adicionar(T* novoNo) {
        if (primeiro == nullptr) {
            primeiro = novoNo;
            ultimo = novoNo;
        } else {
            ultimo->setProximo(novoNo);
            ultimo = novoNo;
        }
    }

    void imprimir() const {
        T* atual = primeiro;
        while (atual != nullptr) {
            cout << *atual << endl;
            atual = atual->getProximo();
        }
    }

    ~ListaEncadeada() {
        T* atual = primeiro;
        while (atual != nullptr) {
            T* proximo = atual->getProximo();
            delete atual;
            atual = proximo;
        }
    }
};

#endif
