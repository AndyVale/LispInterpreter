#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <stdexcept>

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//Token
/*È la struttura che rappresenta l'elemento più piccolo leggibile dal file di testo.
NOTA:
	Idealmente ho visto i token come divisi in tre categorie: le parentesi, le parole chiave del linguaggio,
	e i numeri con gli indetificativi delle variabili questo perchè per la creazione di un token di categoria 0 (parentesi)
	devo leggere solo un carattere, per la creazione di un token di categoria 1 mi basta conoscere il tag associato al token
	e la parola viene stabilita deterministicamente di conseguenza, per la categoria 2 invece ho bisogno di conoscere
	sia il tag che il valore.
*/

struct Token
{
	//categoria0
	static constexpr int SP = 0;//(
	static constexpr int CP = 1;//)

	//categoria 1
	static constexpr int BLOCK = 2;
	static constexpr int SET = 3;
	static constexpr int PRINT = 4;
	static constexpr int INPUT = 5;
	static constexpr int IF = 6;
	static constexpr int WHILE = 7;
	static constexpr int ADD = 8;
	static constexpr int SUB = 9;
	static constexpr int MUL = 10;
	static constexpr int DIV = 11;
	static constexpr int LT = 12;
	static constexpr int GT = 13;
	static constexpr int EQ = 14;
	static constexpr int AND = 15;
	static constexpr int OR = 16;
	static constexpr int NOT = 17;
	static constexpr int TRUE = 18;
	static constexpr int FALSE = 19;

	//categoria 2
	static constexpr int VAR = 20;
	static constexpr int NUM = 21;

	static constexpr const char* tag2word[]{
		"(", ")", "BLOCK", "SET", "PRINT",
		"INPUT", "IF", "WHILE", "ADD", "SUB",
		"MUL", "DIV", "LT", "GT", "EQ",
		"AND", "OR", "NOT", "TRUE", "FALSE", "VAR", "NUM"
	};
	Token(int t, std::string v, std::pair<int, int> c) : tag{ t }, word{ v }, coordinate{c} {};//per creare token di qualsiasi categoria

	Token(int t, std::pair<int, int>c) : tag{ t }, coordinate{ c } {//per creare token di categoria 0 o 1
		if (t < 0 || t > 19)
			throw std::invalid_argument("Errore: tag di un token non di categoria 0 o 1");
		else
			word = tag2word[t];
	};

	int tag;
	std::string word;
	std::pair<int, int> coordinate;//mi servono per tenere traccia della posizione del token nel file per segnalare eventuali errori
};

std::ostream& operator<<(std::ostream& os, const Token& t);

#endif
