#include <iostream>
#include "grafo_matriz.h"

int main() {
    // Inicializa o grafo sem definir a ordem diretamente, pois ela será carregada do arquivo
    grafo_matriz g(0, true); // Direcionado inicialmente como true (ajustado pelo arquivo)

    // Carrega o grafo a partir do arquivo "Grafo.txt"
    g.carrega_grafo();

    // Exemplo: Mostrar a matriz de adjacência
    if (g.eh_direcionado()) {
        // Exibe a matriz 2D se o grafo for direcionado
        std::cout << "Matriz 2D do Grafo Direcionado:" << std::endl;
        const std::vector<std::vector<int>>& matriz = g.get_matriz();
        for (const auto& linha : matriz) {
            for (const auto& valor : linha) {
                std::cout << valor << " ";
            }
            std::cout << std::endl;
        }
    } else {
        // Exibe a matriz linear se o grafo for não direcionado
        std::cout << "Matriz Linear do Grafo Não Direcionado:" << std::endl;
        const std::vector<int>& matrizLinear = g.get_matriz_linear();
        for (int i = 0; i < matrizLinear.size(); ++i) {
            std::cout << matrizLinear[i] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
