#ifndef GRAFO_H
#define GRAFO_H


class grafo
{
private:
    /* data */


public:
    grafo(/* args */);
    ~grafo();
    bool eh_bipartido(); //- função força bruta que indica se o grafo é bipartido ou não
    int n_conexo();//- função que indica a quantidade de componentes conexas
    int get_grau(); //- função que retorna o grau do grafo
    int get_ordem(); //- função que retorna a ordem do grafo
    bool eh_direcionado(); //- função que retorna se o grafo é direcionado ou não
    bool vertice_ponderado(); //- função que informa se os vertices do grafo tem peso
    bool aresta_ponderada(); //- função que informa se as arestas do grafo tem peso
    bool eh_completo(); //- função que diz se um grafo é completo ou não
    bool eh_arvore(); //- função que diz se o grafo é uma árvore
    bool possui_articulacao(); //- função que diz se existe ao menos um vertice de articulação
    bool possui_ponte(); //- função que diz se existe ao menos uma aresta ponte
    void carrega_grafo(); //- função que lê um arquivo txt com um grafo e carrega ele
    void novo_grafo(); //- função que lê um arquivo txt de configuracao e gera um grafo aleatorio
};

#endif

grafo::grafo(/* args */)
{

}

grafo::~grafo()
{
}
