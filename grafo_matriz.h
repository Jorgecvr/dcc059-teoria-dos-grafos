#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include "grafo.h"

#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

class grafo_matriz : public grafo {
private:
    std::vector<std::vector<int>> matriz; // Matriz de adjacência
    int ordem; // Número de vértices
    bool direcionado; // Se o grafo é direcionado

public:
    const std::vector<std::vector<int>>& get_matriz() const;
    grafo_matriz(int ordem, bool direcionado);
    ~grafo_matriz();

    bool eh_bipartido() override;
    int n_conexo() override;
    int get_grau() override;
    int get_ordem() override;
    bool eh_direcionado() override;
    bool vertice_ponderado() override;
    bool aresta_ponderada() override;
    bool eh_completo() override;
    bool eh_arvore() override;
    bool possui_articulacao() override;
    bool possui_ponte() override;
    void carrega_grafo() override;
    void novo_grafo() override;
};

#endif




