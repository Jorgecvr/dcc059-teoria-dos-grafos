#ifndef GRAFOLISTA_H
#define GRAFOLISTA_H

#include "Grafo.h"
#include "ListaEncadeada.h"
#include "VerticeEncadeado.h"
#include "ArestaEncadeada.h"
#include <iostream>
#include <cstdlib>

class GrafoLista : public grafo {
private:
    ListaEncadeada<VerticeEncadeado>* vertices;
    ListaEncadeada<ArestaEncadeada>* arestas;
    int ordem;
    bool direcionado;
    bool verticePonderado;
    bool arestaPonderada;

    VerticeEncadeado* encontra_vertice(int id);
    void adiciona_vertice(int id, int peso);
    void adiciona_aresta(int origem, int destino, int peso);
    void busca_profundidade(VerticeEncadeado* vertice, bool* visitados);
    void imprimir();

public:
    GrafoLista();
    ~GrafoLista();


    bool eh_bipartido() override;
    int n_conexo() override;
    int get_grau() override;
    int get_ordem() override { return ordem; }
    bool eh_direcionado() override { return direcionado; }
    bool vertice_ponderado() override { return verticePonderado; }
    bool aresta_ponderada() override { return arestaPonderada; }
    bool eh_completo() override;
    bool eh_arvore() override;
    bool possui_articulacao() override;
    bool possui_ponte() override;
    void carrega_grafo() override;
    void novo_grafo() override;
};

#endif