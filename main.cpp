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


    // Verifica articulação
    std::cout <<std::endl;
    std::cout << "O grafo possui articulação: " 
              << (grafo.possui_articulacao() ? "Sim" : "Não") << std::endl;

    GrafoLista grafoLista;


    // grafoLista.adicionarVertice(1, 4);

    // grafoLista.adicionarVertice(2, 2);
    // grafoLista.adicionarVertice(2, 2);
    // grafoLista.adicionarVertice(3, 3);
    // grafoLista.adicionarVertice(4, 2);


    // grafoLista.adicionarAresta(1, 4, 104);
    // grafoLista.adicionarAresta(3, 4, 106);
    // grafoLista.adicionarAresta(2, 4, 102);

    grafoLista.carrega_grafo();

    grafoLista.imprimir();


    return 0;
}
