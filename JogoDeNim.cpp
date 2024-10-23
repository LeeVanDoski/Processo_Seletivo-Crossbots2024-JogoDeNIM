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

		cout << "Este jogo de Nim é baseado em pilhas, ou seja," << endl;
		cout << "voce nao pode remover palitos de duas pilhas diferentes" << endl;
		cout << "Dado o numero de pilhas e o total de palitos, as pilhas serao montadas aleatoriamente" << endl;
		cout << "Bom jogo ;)" << endl;

		while (rodando) {
			switch (cont) {
			case 0:
				cout << "Digite a quantidade de pilhas que deseja(1-42): ";
				cin >> numP;
				cont = 1;
				break;
			case 1:
				cout << "Digite a quantidade de pecas(1-150): ";
				cin >> num;
				pilhas = geraQtdPalitosPilha(numP, num);
				system("cls");
				desenhaPalitos();
				cont = 5;
				break;

			case 2:
				cout << "Escolha de qual pilha vai retirar: ";
				cin >> numP;
				if (!pilhaErro(numP))
					cont = 3;
				break;
			case 3:
				cout << "Quantas pecas vai retirar: ";
				cin >> num;
				if (!pecasErro(numP, num)) {
					retiraPalitos(numP, num);
					system("cls");
					desenhaPalitos();
					cont = 4;
				}
				break;
			case 4:
				cont = 2;
				system("cls");
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
		int maxSticks = *max_element(vecSup.begin(), vecSup.end());
		int width = 2; 

		while (accumulate(vecSup.begin(), vecSup.end(), 0) != 0) {
			for (size_t i = 0; i < vecSup.size(); i++) {
				cout << setw(width); 
				if (vecSup[i] > 0) {
					cout << "|"; 
					vecSup[i]--;
				}
				else {
					cout << " "; // Print space if there are no sticks
				}
				cout << "  "; // Space between piles
			}
			cout << endl; // Move to the next line after drawing the sticks
		}
		cout << setw(width); // Align with column width for the sums
		for (size_t i = 0; i < pilhas.size(); i++) {
			cout << pilhas[i]; // Print the sum for each column
			cout << "   "; // Space between sums
		}
		cout << endl;
	}
	vector<int> JogoDeNim::geraQtdPalitosPilha(int pilhas, int palitos) {
		vector<int> qtdPalitos(pilhas);

		// Inicializa o gerador de números aleatórios
		srand(static_cast<unsigned>(time(0)));

		// Garantir que o número de pilhas não exceda o número de palitos
		if (pilhas > palitos) {
			cout << "Número de pilhas maior que o número de palitos. Tente novamente." << endl;
			return vector<int>(); // Retorna um vetor vazio em caso de erro
		}

		// Garantir pelo menos 1 palito em cada pilha
		for (int i = 0; i < pilhas; ++i) {
			qtdPalitos[i] = 1;
		}

		// Calcula os palitos restantes após a distribuição inicial
		palitos -= pilhas;

		// Distribui uma quantidade base maior na primeira pilha
		int base = palitos / (2 * pilhas); // Definindo uma base proporcional para distribuição
		qtdPalitos[0] += base; // Aumenta a primeira pilha com a base
		palitos -= base; // Ajusta o número de palitos restantes

		// Distribuição das peças restantes para todas as pilhas de forma mais equilibrada
		for (int i = 0; i < pilhas; ++i) {
			if (palitos <= 0) break; // Para se não houver mais palitos para distribuir

			int addSticks = rand() % (palitos / (pilhas - i) + 1); // Distribui de forma proporcional
			qtdPalitos[i] += addSticks; // Adiciona os palitos à pilha atual
			palitos -= addSticks; // Atualiza o número de palitos restantes
		}

		// Se ainda houver palitos restantes, adiciona-os aleatoriamente
		while (palitos > 0) {
			for (int i = 0; i < pilhas; ++i) {
				if (palitos > 0) {
					qtdPalitos[i]++; // Adiciona um palito a uma pilha aleatória
					palitos--; // Atualiza o número de palitos restantes
				}
			}
		}

		// Ordena a distribuição em ordem decrescente
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

			// Procura por uma pilha que possa ser modificada para zerar o nim-sum
			for (int i = 0; i < pilhas.size(); i++) {
				int novaPilha = pilhas[i] ^ nimSum; // Tenta alterar o valor da pilha
				if (novaPilha < pilhas[i]) { // Verifica se é uma jogada válida
					cout << "Bot retirou " << pilhas[i] - novaPilha << " peca(s) da pilha " << i + 1 << endl;
					pilhas[i] = novaPilha; // Faz a jogada para zerar o nim-sum
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