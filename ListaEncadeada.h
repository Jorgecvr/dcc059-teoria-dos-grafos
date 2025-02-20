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

    // void setInicio(T* novoInicio) {
    // primeiro = novoInicio;
    // }

    void remover(T* noParaRemover) {
        if (primeiro == nullptr || noParaRemover == nullptr) {
            return;
        }

        if (primeiro == noParaRemover) {
            primeiro = primeiro->getProximo();
            if (primeiro == nullptr) {
                ultimo = nullptr;
            }
            delete noParaRemover;
            return;
        }

        T* atual = primeiro;
        while (atual->getProximo() != nullptr && atual->getProximo() != noParaRemover) {
            atual = atual->getProximo();
        }

        if (atual->getProximo() == noParaRemover) {
            T* proximo = noParaRemover->getProximo();
            atual->setProximo(proximo); 

            if (noParaRemover == ultimo) {
                ultimo = atual;
            }
        }
    }
};

#endif