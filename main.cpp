#include <iostream>

#include "GrafoLista.h"
#include "grafo_matriz"

int main() {
    GrafoLista grafo;

    grafo.adicionarVertice(1, 1.5);
    grafo.adicionarVertice(2, 2.5);

    grafo.adicionarAresta(1, 2, 10);

    grafo.adicionarVertice(1, 3);

    grafo.imprimir();

    grafo_matriz grafo;  

    grafo.carrega_grafo();

    std::cout << "GRAU:" << grafo.get_grau();

    // Verifica se o grafo foi carregado corretamente
    if (grafo.get_ordem() == 0) {
        std::cerr << "Erro: O grafo não foi carregado corretamente." << std::endl;
        return 1;  // Retorna um código de erro
    }

    // Exibe a ordem do grafo
    std::cout << "Número de vértices: " << grafo.get_ordem() << std::endl;

    // Verifica se o grafo é direcionado ou não
    std::cout << "O grafo é " << (grafo.eh_direcionado() ? "direcionado" : "não direcionado") << "." << std::endl;

    // Exibe a matriz de adjacência (se o grafo for direcionado)
    std::cout << "Matriz de Adjacência:" << std::endl;
    const auto& matriz = grafo.get_matriz();
    for (const auto& linha : matriz) {
        for (int valor : linha) {
            std::cout << valor << " ";
        }
        std::cout << std::endl;
    }

    // Caso o grafo seja não direcionado, exibe a matriz linear
    if (!grafo.eh_direcionado()) {
        std::cout << "Matriz Linear:" << std::endl;
        const auto& matrizLinear = grafo.get_matriz_linear();
        for (size_t i = 0; i < matrizLinear.size(); ++i) {
            std::cout << matrizLinear[i] << " ";
        }
        std::cout << std::endl;
    }

    std::cout <<  "EH COMPLETO: " << grafo.eh_completo();
    std::cout <<  "N conexo: " << grafo.n_conexo();


    return 0;
}
