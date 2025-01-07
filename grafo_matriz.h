#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

#include <string>

#include "grafo.h"

class grafo_matriz : public grafo {
private:
    int** matriz;           
    int* matrizLinear;      
    int ordem;              
    bool direcionado;
    bool vtp;
    bool atp;

    bool temCiclo(int v, int pai, bool* visitado); 
    bool bfsBipartido(int inicio, int* cor);       
    void inicializarMatrizes();                   

public:
    grafo_matriz();
    ~grafo_matriz();

    int calcularIndiceLinear(int origem, int destino);

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
    int** get_matriz();
    int* get_matriz_linear();
};

#endif