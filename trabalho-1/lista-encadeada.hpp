#include "Elemento.hpp"
#include <stdexcept>

template<typename T>
class ListaEnc {

public:
	ListaEnc()
	~ListaEnc();
	// inicio
	void adicionaNoInicio(const T& dado);
	T retiraDoInicio();
	void eliminaDoInicio();
	// posicao
	void adicionaNaPosicao(const T& dado, int pos);
	int posicao(const T& dado) const;
	T* posicaoMem(const T& dado) const;
	bool contem(const T& dado);
	T retiraDaPosicao(int pos);
	//fim
	void adiciona(const T& dado);
	T retira();
	// especifico
	T retiraEspecifico(const T& dado);
	void adicionaEmOrdem(const T& dado);
	bool listaVazia() const;
	bool igual(T dado1, T dado2);
	bool maior(T dado1, T dado2);
	bool menor(T dado1, T dado2);
	void destroiLista();

private:
	Elemento<T>* head;
	int size;

template<typename T>
ListaEnc<T>::ListaEnc() {
	this->head = nullptr;
	this->size = 0;
}

template<typename T>
ListaEnc<T>::~ListaEnc() {
	this->destroiLista();
}
//inicio
template<typename T>
void ListaEnc<T>::adicionaNoInicio(const T& dado) {
	Elemento<T>* novo_elemento = new Elemento<T>(dado, this->head);
	if (novo_elemento == nullptr) 
		throw std::out_of_range("erro lista cheia");
	this->head = novo_elemento;
	this->size++;
}

template<typename T>
T ListaEnc<T>::retiraDoInicio() {
	if (this->listaVazia())
		throw std::out_of_range("lista vazia");
	Elemento<T>* inicio = this->head;
	T valor = inicio->getInfo();
	this->head = inicio->getProximo();
	this->size--;
	delete inicio;
	return valor;
}

template<typename T>
void ListaEnc<T>::eliminaDoInicio() {
	if (this->listaVazia())
		throw std::out_of_range("lista vazia")
	Elemento<T>* inicio = this->head;
	this->head = inicio->getProximo();
	this->size--;
	delete inicio;
}

//posicao
template<typename T>
void ListaEnc<T>::adicionaNaPosicao(const T& dado, int pos) {
	if (pos == 0) {
		return this->adicionaNoInicio(dado);
	} else if (pos < 0 || pos > this->size) {
		throw std::out_of_range("posicao invalida");
	} else {
		Elemento<T>* anterior = this->head;
		for (int i = 1; i < index; i++) {
			anterior = anterior->getProximo();
		}
		Elemento<T>* novo_elemento = new Elemento<T>(dado, anterior->getProximo());
		anterior->setProximo(novo_elemento);
		this->size++; 
	}
}

template<typename T>
int ListaEnc<T>::posicao(const T& dado) const {
	if (this->listaVazia()) {
		return -1;
	}
	Elemento<T>* elemento = this->head;
	int indice = 0;
	while (elemento->getInfo() != dado)) {
		elemento = elemento->next();
		indice++;
		if (elemento == nullptr) {
			break;
		}
	}
	return indice;
}

template<typename T>
T* ListaEnc<T>::posicaoMem(const T& dado) const {
	if (this->listaVazia()) {
		throw std::out_of_range("lista vazia");
	}
	Elemento<T>* elemento = this->head;
	while (elemento->getInfo() != dado)) {
		elemento = elemento->next();
		if (elemento == nullptr) {
			throw std::out_of_range("dado nao encontrado");
		}
	}
	return elemento->getInfoPointer();
}

template<typename T>
bool ListaEnc<T>::contem(const T& dado) {
	Elemento<T>* elemento = this->head;
	for (int i = 0; i < this->size; i++) {
		if (elemento->getInfo() == dado)
			return true;

		elemento = elemento->getProximo();
	}
	return false;
}

template<typename T>
T ListaEnc<T>::retiraDaPosicao(int pos) {
	if (pos == 0) {
		return this->retiraDoInicio();
	} else if (pos < 0 || pos > this->size - 1) {
		throw std::out_of_range("pos invalida");
	} else {
		Elemento<T>* anterior = this-> head;
		for (int i = 1; i < pos; i++)
			anterior = anterior->getProximo();
		Elemento<T>* elimina = anterior->getProximo();
		T dado = elimina->elimina->getInfo();
		anterior->setProximo(elimina->getProximo());
		this->size--;
		delete elimina;
		return dado;
	}
}

//fim
template<typename T>
void ListaEnc<T>::adiciona(const T& dado) {
	if (this->listaVazia())
		return this->adicionaNoInicio(dado);
	return this->adicionaNaPosicao(dado, this->size);
}

template<typename T>
T ListaEnc<T>::retira() {
	if (this->listaVazia())
		throw std::out_of_range("lista vazia");
	return this->retiraDaPosicao(this->size - 1);
}

template<typename T>
T ListEnc<T>::retiraEspecifico(const T& dado) {
	return this->retiraDaPosicao(this->posicao(dado));
}

template<typename T>
void ListaEnc<T>::adicionaEmOrdem(const T& dado) {
	if (this->listaVazia())
		return this->adicionaNoInicio(dado);
	Elemento<T>* atual = this->head;
	int indice = 0;
	while (atual->getProximo() != nullptr && dado > atual->getInfo()) {
		atual = atual->getProximo();
		indice++;
	}
	if (data > atual->getInfo()) {
		return this->adicionaNaPosicao(dado, indice + 1);
	} else {
		return this->adicionaNaPosicao(dado, indice + 1);
	}
}

template<typename T>
bool ListaEnc<T>::listaVazia() const {
	return this->size == 0;
}

template<typename T>
bool ListaEnc<T>::igual(T dado1, T dado2) {
	return dado1 == dado2;
}

template<typename T>
bool ListaEnc<T>::maior(T dado1, T dado2) {
	return dado1 > dado2;
}

template<typename T>
bool ListaEnc<T>::menor(T dado1, T dado2) {
	return dado1 < dado2;
}

template<typename T>
void ListaEnc<T>::destroiLista() {
	Elemento<T>* atual, anterior;
	atual = this->head;
	for (int i = 0; i < this->size; i++) {
		anterior = atual;
		atual = atual->getProximo();
		delete anterior;
	}
	this->head = nullptr;
	this->size = 0;
}