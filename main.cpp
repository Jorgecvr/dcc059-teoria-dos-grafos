#include <iostream>
#include "grafo_matriz.h"
#include "GrafoLista.h"

using namespace std;

int main() {
    grafo_matriz grafo;

    cout << "=== Testando Grafo com Matriz ===" << endl;

    grafo.carrega_grafo();

    cout << "Grau maximo do grafo: " << grafo.get_grau() << endl;
    cout << "Numero de vertices: " << grafo.get_ordem() << endl;

    cout << "O grafo e " << (grafo.eh_direcionado() ? "direcionado" : "nao direcionado") << "." << endl;

    if (grafo.eh_direcionado()) {
        cout << "Matriz de Adjacencia:" << endl;
        int** matriz = grafo.get_matriz();
        for (int i = 0; i < grafo.get_ordem(); ++i) {
            for (int j = 0; j < grafo.get_ordem(); ++j) {
                cout << matriz[i][j] << " ";
            }
            cout << endl;
        }
    } else {
        cout << "Matriz Linear:" << endl;
        int* matrizLinear = grafo.get_matriz_linear();
        int tamanho = (grafo.get_ordem() * (grafo.get_ordem() + 1)) / 2;
        for (int i = 0; i < tamanho; ++i) {
            cout << matrizLinear[i] << " ";
        }
        cout << endl;
    }

    cout << "O grafo e completo? " << (grafo.eh_completo() ? "Sim" : "Nao") << endl;
    cout << "Numero de componentes conexas: " << grafo.n_conexo() << endl;

    GrafoLista grafoLista;
    grafoLista.carrega_grafo();
    cout << "\n=== Testando Grafo com Lista ===" << endl;
    grafoLista.imprimir();

    return 0;
}