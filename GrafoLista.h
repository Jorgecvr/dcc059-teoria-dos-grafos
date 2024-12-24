#ifndef GRAFOLISTA_H
#define GRAFOLISTA_H

#include "ListaEncadeada.h"
#include "VerticeEncadeado.h"
#include "ArestaEncadeada.h"


using namespace std;

class GrafoLista {
private:
    ListaEncadeada<VerticeEncadeado>* vertices;
    ListaEncadeada<ArestaEncadeada>* arestas;

    No<VerticeEncadeado>* encontraVertice(int id) {
        No<VerticeEncadeado>* primeiro = vertices->getInicio();
        while (primeiro != nullptr) {
            if (primeiro->getValor().getId() == id) {
                return primeiro;
            }
            primeiro = primeiro->getProximo();
        }
        return nullptr;
    }

public:
    GrafoLista() {
        vertices = new ListaEncadeada<VerticeEncadeado>();
        arestas = new ListaEncadeada<ArestaEncadeada>();
    }

    void adicionarVertice(int id, float peso) {

        if (encontraVertice(id) != nullptr) {
            cout << "VerticeEncadeado com ID " << id << " ja existe!" << endl;
            return;
        }
        VerticeEncadeado novoVertice(id, peso);
        vertices->adicionar(novoVertice);
    }

    void adicionarAresta(int origem, int destino, int peso) {
        No<VerticeEncadeado>* verticeOrigem = encontraVertice(origem);
        No<VerticeEncadeado>* verticeDestino = encontraVertice(destino);

        if (verticeOrigem == nullptr || verticeDestino == nullptr) {
            cout << "Erro: Um ou ambos os vértices não existem!" << endl;
            return;
        }

        ArestaEncadeada novaAresta(verticeOrigem, verticeDestino, peso);
        arestas->adicionar(novaAresta);
    }

    void imprimir() const {
        cout << "Vertices:\n";
        vertices->imprimir();
        cout << "Arestas:\n";
        arestas->imprimir();
    }

    ~GrafoLista() {
        delete vertices;
        delete arestas;
    }
};

#endif