#ifndef VERTICE_H
#define VERTICE_H

#include "aresta.h"
#include<list>

class vertice
{
private:
    int id;
    int peso;
    int grau;
    list<aresta> vet;
    /* data */
public:
    vertice(int id, int peso);
    ~vertice();
};

#endif

vertice::vertice(int id, int peso)
{
    this->id = id;
    this->peso = peso;
    this->grau = 0;
}

vertice::~vertice()
{

}

