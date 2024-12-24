#include "GrafoLista.h"

int main() {
    GrafoLista grafo;

    grafo.adicionarVertice(1, 1.5);
    grafo.adicionarVertice(2, 2.5);

    grafo.adicionarAresta(1, 2, 10);

    grafo.adicionarVertice(1, 3);

    grafo.imprimir();
 
    return 0;
}