#ifndef VERTICEENCADEADO_H
#define VERTICEENCADEADO_H

#include <iostream>
#include "ListaEncadeada.h"

class ArestaEncadeada;

class VerticeEncadeado {
private:
    int id;
    float peso;
    int grau;

public:
    VerticeEncadeado(int id, float peso)
        : id(id), peso(peso), grau(0) {}


    int getId() const {
        return id;
    }

    float getPeso() const {
        return peso;
    }

    int getGrau() const {
        return grau;
    }



    friend std::ostream& operator<<(std::ostream& os, const VerticeEncadeado& vertice) {
        os << "VerticeEncadeado " << vertice.id << " (Peso: " << vertice.peso << ", Grau: " << vertice.grau << ")\n";
        os << "Conexões: ";

        return os;
    }
};

#endif
