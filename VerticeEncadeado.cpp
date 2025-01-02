#include "VerticeEncadeado.h"

VerticeEncadeado::VerticeEncadeado(int id, int peso)
    : id(id), peso(peso), grau(0), proximo(nullptr), conexoes(new ListaEncadeada<ArestaEncadeada>()) {}

int VerticeEncadeado::getId() const {
    return id;
}

int VerticeEncadeado::getPeso() const {
    return peso;
}

int VerticeEncadeado::getGrau() const {
    return grau;
}


VerticeEncadeado* VerticeEncadeado::getProximo() const {
    return proximo;
}
 

void VerticeEncadeado::setConexao(VerticeEncadeado* verticeDestino, int pesoAresta) {

    ArestaEncadeada* novaAresta = new ArestaEncadeada(this, verticeDestino, pesoAresta);
    conexoes->adicionar(novaAresta);
    grau++;
}

void VerticeEncadeado::decrementarGrau() {
    if (grau > 0) {
        grau--;
    }
}

/* template <typename T> 
void ListaEncadeada<T>::setPrimeiro(T* novoPrimeiro) {
    primeiro = novoPrimeiro;
}


void VerticeEncadeado::removerConexao(VerticeEncadeado* verticeDestino) {
    ArestaEncadeada* atual = conexoes->getInicio();
    ArestaEncadeada* anterior = nullptr;

    while (atual != nullptr) {
        if (atual->getDestino() == verticeDestino) {
            if (anterior == nullptr) {
                // Atualiza o início da lista (primeiro) usando setPrimeiro
                conexoes->setPrimeiro(atual->getProximo());
            } else {
                // Remove a conexão intermediária ou final
                anterior->setProximo(atual->getProximo());
            }
            delete atual;        // Libera memória da conexão
            decrementarGrau();   // Atualiza o grau
            return;
        }
        anterior = atual;
        atual = atual->getProximo();
    }

    std::cerr << "Erro: Conexao com o vertice " << verticeDestino->getId() << " nao encontrada.\n";
}
*/

ArestaEncadeada* VerticeEncadeado::getPrimeiraConexao(){

    return conexoes->getInicio();
}

void VerticeEncadeado::setProximo(VerticeEncadeado* novoProximo){
    proximo = novoProximo;
}

std::ostream& operator<<(std::ostream& os, const VerticeEncadeado& vertice) {
    os << "VerticeEncadeado " << vertice.id
       << " (Peso: " << vertice.peso << ", Grau: " << vertice.grau << ")\n";
    os << "Conexoes: ";
    vertice.conexoes->imprimir();
    return os;
}

