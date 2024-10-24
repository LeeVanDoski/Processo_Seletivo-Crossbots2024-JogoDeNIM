#include "JogoDeNim.h"

	JogoDeNim::JogoDeNim() {
		executar();
	}
	void JogoDeNim::executar() {
		bool rodando = true;
		char cont = 0;
		int numP;
		int num;
		int aux;
		const char* osClean;
#ifndef __WIN32 //acha sistema pra dar clean no terminal
		osClean = "cls";
#else
		osClean = "clear";
#endif // !__WIN32


		cout << "Este jogo de Nim eh baseado em pilhas, ou seja," << endl;
		cout << "voce nao pode remover palitos de duas pilhas diferentes" << endl;
		cout << "Dado o numero de pilhas e o total de palitos, as pilhas serao montadas aleatoriamente" << endl;
		cout << "Para visualização, não escolha números exorbitantes, use no maximo 42 pilhas " << endl;
		cout << "Bom jogo ;)" << endl;

		while (rodando) {
			switch (cont) {
			case 0:
				cout << "Digite a quantidade de pilhas que deseja: ";
				cin >> numP;
				cont = 1;
				break;
			case 1:
				cout << "Digite a quantidade de pecas: ";
				cin >> num;
				pilhas = geraQtdPalitosPilha(numP, num);
				if (!pilhas.empty()) {
					system(osClean);
					desenhaPalitos();
					cont = 5;
				}
				break;

			case 2:
				cout << "Escolha de qual pilha vai retirar(0...n-1): ";
				cin >> numP;
				if (!pilhaErro(numP))
					cont = 3;
				break;
			case 3:
				cout << "Quantas pecas vai retirar: ";
				cin >> num;
				if (!pecasErro(numP, num)) {
					retiraPalitos(numP, num);
					system(osClean);
					desenhaPalitos();
					cont = 4;
				}
				break;
			case 4:
				cont = 2;
				system(osClean);
				bot();
				desenhaPalitos();
				break;
			case 5:

				cout << "Quem vai comecar?[tecle 0 para vc, 1 para o bot]";
				cin >> aux;
				if (aux == 0)
					cont = 2;
				else
					cont = 4;
				break;
			}

			if (!pilhas.empty() && accumulate(pilhas.begin(), pilhas.end(), 0) == 0)
			{
				if (cont == 2)
					cout << "Bot venceu" << endl;
				else
					cout << "Jogador venceu" << endl;

				rodando = false;
				cout << "Deseja jogar novamente?[y/n]";
				cin >> cont;
				if (cont == 'y' || cont == 'Y') {
					rodando = true;
					cont = 0;
					pilhas.clear();
				}
			}


		}


	}
	void JogoDeNim::desenhaPalitos() {
		if (pilhas.empty())
			return;
		vector<int> vecSup = this->pilhas;
		//int maxPalitos = *max_element(vecSup.begin(), vecSup.end());
		int width = 2; 

		while (accumulate(vecSup.begin(), vecSup.end(), 0) != 0) {
			for (size_t i = 0; i < vecSup.size(); i++) {
				cout << setw(width); 
				if (vecSup[i] > 0) {
					cout << "|"; 
					vecSup[i]--;
				}
				else {
					cout << " "; // se n tiver palitos
				}
				cout << "  "; // espaco entre pilhas
			}
			cout << endl; //proxima linha
		}
		cout << setw(width); // alinha
		for (size_t i = 0; i < pilhas.size(); i++) {
			cout << pilhas[i]; 
			cout << "   "; // espaco soma
		}
		cout << endl;
	}
	vector<int> JogoDeNim::geraQtdPalitosPilha(int pilhas, int palitos) {
		vector<int> qtdPalitos(pilhas);

		
		srand(static_cast<unsigned>(time(0)));

		
		if (pilhas > palitos) {
			cout << "Número de pilhas maior que o número de palitos. Tente novamente." << endl;
			return vector<int>(); 
		}

		// pelo menos 1 palito em cada pilha
		for (int i = 0; i < pilhas; ++i) {
			qtdPalitos[i] = 1;
		}

		// calcula os palitos restantes
		palitos -= pilhas;

		// distribui uma quantidade base maior na primeira pilha
		int base = palitos / (2 * pilhas); 
		qtdPalitos[0] += base; 
		palitos -= base; 

		// distribui restante
		for (int i = 0; i < pilhas; ++i) {
			if (palitos <= 0) break;

			int addPalitos = rand() % (palitos / (pilhas - i) + 1); // proporcional
			qtdPalitos[i] += addPalitos; 
			palitos -= addPalitos; 
		}

		// se ainda houver palitos restantes adiciona aleatoriamente
		while (palitos > 0) {
			for (int i = 0; i < pilhas; ++i) {
				if (palitos > 0) {
					qtdPalitos[i]++; 
					palitos--;
				}
			}
		}

		// ordena
		sort(qtdPalitos.begin(), qtdPalitos.end(), greater<int>());

		return qtdPalitos;
	}

	void JogoDeNim::retiraPalitos(int pilha, int palitos) {
		pilhas[pilha] -= palitos;
	}
	void JogoDeNim::bot()
	{
		int nimSum = calculaSomaNim();
		if (nimSum == 0) {
			int i = 0;
			while (pilhas[i] == 0) {
				i++;
			}
			retiraPalitos(i, 1);
		}
		else {

			for (int i = 0; i < pilhas.size(); i++) {
				int novaPilha = pilhas[i] ^ nimSum; 
				if (novaPilha < pilhas[i]) { 
					cout << "Bot retirou " << pilhas[i] - novaPilha << " peca(s) da pilha " << i + 1 << endl;
					pilhas[i] = novaPilha; 
					break;
				}
			}
		}
	}
	int JogoDeNim::calculaSomaNim() {
		int nim = 0;
		for (int pilha : pilhas) {
			nim ^= pilha;
		}
		return nim;
	}

	bool JogoDeNim::pilhaErro(int pilha) {
		if (pilha >= pilhas.size() || pilhas[pilha] == 0) {
			cout << "Coloque um numero valido de pilha!!" << endl;
			return true;
		}
		return false;
	}
	 bool JogoDeNim::pecasErro(int pilha, int pecas) {
		if (pecas > pilhas[pilha]) {
			cout << "Coloque um numero valido de pecas!!" << endl;
			return true;
		}
		return false;
	 }
