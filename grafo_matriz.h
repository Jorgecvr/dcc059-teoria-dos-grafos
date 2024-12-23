#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

#include <vector>
#include "grafo.h"  

class grafo_matriz : public grafo {
private:
    std::vector<std::vector<int>> matriz;        // Matriz de adjacência 2D (para grafos direcionados)
    std::vector<int> matrizLinear;               // Representação linear da matriz de adjacência (para grafos não direcionados)

    int ordem;                                  // Número de vértices no grafo
    bool direcionado;                           // Indica se o grafo é direcionado ou não

public:
    // Construtores e destrutores
    grafo_matriz();                             // Construtor padrão
    ~grafo_matriz();                            // Destruidor

    // Métodos para acessar as representações do grafo
    const std::vector<std::vector<int>>& get_matriz() const;  // Retorna a matriz 2D de adjacência
    const std::vector<int>& get_matriz_linear() const;        // Retorna a matriz linear

    // Função para calcular o índice no vetor linear
    int calcularIndiceLinear(int origem, int destino);

    // Sobrecarga das funções da classe base 'grafo'
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
    
    // Função para carregar o grafo a partir de um arquivo
    void carrega_grafo() override;

    // Função para criar um novo grafo
    void novo_grafo() override;
};

#endif  // GRAFO_MATRIZ_H
