#ifndef NO_H
#define NO_H


template <typename T>

class No {
private:
    T valor;      
    No<T>* proximo; 
      
public:
    No(const T valor, No<T>* proximo = nullptr) : valor(valor), proximo(proximo) {}

    T getValor() {
        return valor;
    }

    No<T>* getProximo() const {
        return proximo;
    }

    void setProximo(No<T>* novoProximo) {
        proximo = novoProximo;
    }

    T& acessarObjeto() {
        return valor;
    }
};

#endif
