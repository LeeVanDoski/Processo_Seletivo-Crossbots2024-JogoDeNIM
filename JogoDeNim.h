#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <iomanip>
using namespace std;

class JogoDeNim {
private:
	vector<int> pilhas;
	inline void executar();
	inline void desenhaPalitos();
	inline vector<int> geraQtdPalitosPilha(int pilhas, int palitos);
	inline void retiraPalitos(int pilha, int palitos);
	inline void bot();
	inline int calculaSomaNim();
	inline bool pilhaErro(int pilha);
	inline bool pecasErro(int pilha, int pecas);

public:
	inline JogoDeNim();
};
