#include <iostream>

#include "GrafoLista.h"
#include "grafo_matriz.h"

int main() {
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


     GrafoLista grafo1;



    //grafo1.adicionarVertice(1, 1.56);

    //grafo1.adicionarVertice(2, 2.5);
    //grafo1.adicionarVertice(2, 2.5);
    //grafo1.adicionarVertice(3, 3);
    //grafo1.adicionarVertice(4, 2);


    //grafo1.adicionarAresta(1, 2, 100);
    //grafo1.adicionarAresta(1, 4, 104);
    //grafo1.adicionarAresta(3, 4, 106);
    //grafo1.adicionarAresta(2, 4, 102);
    //grafo1.adicionarAresta(4, 3, 102);

    grafo1.carrega_grafo();

    grafo1.imprimir();


    return 0;
}
