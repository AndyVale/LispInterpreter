#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <vector>
#include <fstream>
#include "Token.h"
#include "MyExceptions.h"

class Tokenizer{
public:
	Tokenizer() : rigaAttuale{ 1 }, nTokenAttuale{ 0 }{}//inizializzo la riga iniziale a 1 e il numero del token a 0

	std::vector<Token> operator()(std::ifstream& input) {//implemento l'operatore ()
		std::vector<Token> tokens;
		tokenizeFile(input, tokens);
		if (tokens.size() > 0) {//se c'è almeno un Token la fase di tokenizzazione è andata a buon fine la fase di parsing
			return tokens;
		}
		else {//Se non c'è alcun token allora lancio un eccezione di file vuoto
			throw LexicalError("File vuoto");
		}
		
	}
private:
	int rigaAttuale;
	int nTokenAttuale;
	void tokenizeFile(std::ifstream& f, std::vector<Token>& tokens);
	char leggiCarattere(std::ifstream&);
	bool getNextToken(std::ifstream& inputFile, std::stringstream& ssToken);
};
#endif
