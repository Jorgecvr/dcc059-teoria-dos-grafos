#ifndef GRAFOLISTA_H
#define GRAFOLISTA_H

#include "Grafo.h"
#include "ListaEncadeada.h"
#include "VerticeEncadeado.h"
#include "ArestaEncadeada.h"

#include <iostream>
#include <unordered_map>
#include <queue>         

class GrafoLista : public grafo {
private:
    ListaEncadeada<VerticeEncadeado>* vertices;
    ListaEncadeada<ArestaEncadeada>* arestas;

    VerticeEncadeado* encontraVertice(int id);
    bool vTP;
    bool aTP;
    int ordem;
    bool direcionado;
    bool verticePonderado;
    bool arestaPonderada;


public:
    GrafoLista();

    void adicionarVertice(int id, float peso);
    void adicionarAresta(int origem, int destino, int peso);
    void imprimir();

    bool eh_bipartido() override;

    int n_conexo() override;

    int get_grau() override;

    int get_ordem() override {
        return ordem;
    }
    void aumenta_ordem() {
        this->ordem++;
    };

    bool eh_direcionado() override {
        return direcionado;
    }
    void set_eh_direcionado(bool direcionado) {
        this->direcionado = direcionado;
    };


    bool vertice_ponderado() override {
        return verticePonderado;
    }

    void set_vertice_ponderado(bool verticePonderado) {
        this->verticePonderado = verticePonderado;
    };

    bool aresta_ponderada() override {
        return arestaPonderada;
    }
    void set_aresta_ponderada(bool arestaPonderada) {
        this->arestaPonderada = arestaPonderada;
    };

    bool eh_completo() override;

    bool eh_arvore(VerticeEncadeado* verticeInicial) override;
    bool possui_articulacao() override;
    bool possui_ponte() override;
    bool bfs_bipartido(VerticeEncadeado* inicio, std::unordered_map<int, int>& cor);
    void carrega_grafo() override;
    void novo_grafo() override;

    void buscaEmProfundidade(VerticeEncadeado* vertice, bool* visitados);

    ~GrafoLista();
};

#endif
