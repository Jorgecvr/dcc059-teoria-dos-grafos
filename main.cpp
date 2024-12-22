#include <iostream>
#include "grafo_matriz.h"

int main() {
    // Inicializa o grafo sem definir a ordem diretamente, pois ela será carregada do arquivo
    grafo_matriz g(0, true); // Direcionado inicialmente como true (ajustado pelo arquivo)

    // Carrega o grafo a partir do arquivo "Grafo.txt"
    g.carrega_grafo();

    // Exemplo: Mostrar a matriz de adjacência carregada
    std::cout << "Matriz de Adjacência do Grafo:" << std::endl;
    for (const auto& linha : g.get_matriz()) {
        for (const auto& valor : linha) {
            std::cout << valor << " ";
        }
        std::cout << std::endl;
    }


    return 0;
}
