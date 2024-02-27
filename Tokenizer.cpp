#include <map>
#include <sstream>
#include <fstream>
#include "Tokenizer.h"
#include <iostream>

bool spazioBianco(char c) {//isBlank generava problemi
	switch (c) {
	case '\t': case '\n': case '\v': case '\f': case '\r': case ' ':
		return 1;
	default:
		return 0;
	}
}

bool unaCifra(char c) {//isDigit generava problemi
	switch (c) {
	case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7':case '8': case '9':
		return 1;
	default:
		return 0;
	}
}

bool isAlphaString(const std::string s) {//Funzione che mi permetta di stabilire se una stringa è composta da sole lettere
	int i;
	for (i = 0; i < s.size(); i++) {
		if (!isalpha(static_cast <unsigned char>(s.at(i)))) return 0;
	}
	return 1;
}

char Tokenizer::leggiCarattere(std::ifstream& inputFile) {//funzione che legge il carattere successivo nel file e aumenta la riga nel caso in cui sia un "a capo"
	char c = inputFile.get();
	if (c == '\n') {
		rigaAttuale++;
		nTokenAttuale = 0;
	}
	return c;
}

bool Tokenizer::getNextToken(std::ifstream& inputFile, std::stringstream& ssToken) {
	/*Ha il compito di mettere in ssToken il prossimo token nel file (sovrascrivendo quello che c'era). Restituisce True se è stato letto un token, false se non è stato letto ed è stato letto EOF*/
	char c, next;
	ssToken.str("");//cancello quello che c'era dentro
	while (((c = leggiCarattere(inputFile)) != EOF) && (spazioBianco((static_cast <unsigned char>(c))))) {
	}//scorro fintanto che sono nel file e leggo spazi bianchi (ignoro appunto gli spazi bianchi)
	if (c != EOF) {//se sono uscito perchè ho trovato un carattere non bianco
		ssToken << c;//il primo carattere sicuramente lo devo considerare nel token
		if (c != ')' && c != '(') {//se è un token di cat0 allora devo fermarmi, quindi controllo non lo sia
			//altrimenti continuo a leggere per comporre il token
			next= inputFile.peek();//prendo il prossimo carattere
			bool isAPossNumber = unaCifra(c) || c == '-';//questo valore booleano mi indica se il token creato ha i presupposti per essere un numero
			while (next != EOF && !spazioBianco(next) && next != ')' && next != '(' && next != '-' && (isAPossNumber == unaCifra(next))) {//Se il prosismo carattere fa parte del token continuo a leggere, altrimenti mi fermo
				c =	leggiCarattere(inputFile);//lo leggo "davvero"
				ssToken << c;//lo immetto nello stringstream
				next=inputFile.peek();//leggo il prossimo con peek
			}
		}
		nTokenAttuale++;//ho finito di leggere il token allora aumento la variabile per tenerne conto
		return true;//restituisco true perchè ho formato un token
	}
	else {//se ho letto la fine del file e ho letto solo caratteri bianchi allora esco subito
		return false;
	}
}

void Tokenizer::tokenizeFile(std::ifstream& f, std::vector<Token>& tokens) {
	//dal momento che il tokenizer viene richiamato solo una volta preferisco 												
	//occupare più memoria ma scrivere in modo più elegante, a mio avviso, il codice
	//adoperando una mappa rispetto che una serie di if-else:
	std::map<std::string, int> wordToTag												
	{
		{ "(", Token::SP }, { ")", Token::CP }, { "BLOCK", Token::BLOCK }, { "SET", Token::SET }, { "PRINT", Token::PRINT },
		{ "INPUT", Token::INPUT }, { "IF", Token::IF }, { "WHILE", Token::WHILE }, { "ADD", Token::ADD }, { "SUB", Token::SUB },
		{ "MUL", Token::MUL }, { "DIV", Token::DIV }, { "LT", Token::LT }, { "GT", Token::GT }, { "EQ", Token::EQ },
		{ "AND", Token::AND }, { "OR", Token::OR }, { "NOT", Token::NOT }, { "TRUE", Token::TRUE }, { "FALSE", Token::FALSE }
	};
	std::stringstream actualTokenTag;
	while (getNextToken(f, actualTokenTag))//leggo il token
	{
		std::pair<int, int> coord(rigaAttuale, nTokenAttuale);//creo una variabile pair che tenga conto della rigaAttuale e del tokenAttuale
		const std::string tokenStr = actualTokenTag.str();//utilizzo una stringa di supporto per non richiamare più volte il metodo str

		if (wordToTag.find(tokenStr) != wordToTag.end()) {//se lo trovo nella mappa delle parole chiave allora sarà di categoria 0 o 1
			Token t = Token(wordToTag[tokenStr], coord);//creo il token
			try {
				tokens.push_back(t);//questo costruttore lancia un eccezione se il token non è di categoria 0 o 1 ma non dovrebbe mai presentarsi questa casistica
			}
			catch (std::invalid_argument e) {//per essere sicuri la gestisco qui dentro inviando un errore dei tipi personalizzati
				throw GenericError("Costruzione di un token non riuscita.");
			}
		}
		else {//se non lo trovo potrebbe esse una variabile o un numero o essere un errore lessicale
			if (tokenStr.length() > 0) {//condizione necessaria per essere una variabile o un numero, così sono sicuro che la variabile firstChar possa essere inizializzata
				char firstChar = static_cast <unsigned char>(tokenStr.at(0));//prendo il primo carattere della stringa (la casto per evitare problemi con isAlpha e isDigit)
				if (isalpha(firstChar)) {//se il primo carattere è una lettera
					if (isAlphaString(tokenStr)) {//controllo se sono solo lettere allora è una variabile
						tokens.push_back(Token(Token::VAR, tokenStr, coord));//aggiungo al vector un Token "VAR"
					}
					else {//altrimenti segnalo l'errore
						std::stringstream error;
						error << "\"" << tokenStr << "\"" << " non risulta essere un nome di variabile valido (riga:  " << coord.first << " il token n'" << coord.second << ") dopo \"" << tokens.back().word << "\"" << std::endl;
						throw LexicalError(error.str());//lancio un eccezione in caso di errore lessicale
					}
				}
				else {//altrimenti potrebbe ancora essere un numero
					if (tokenStr.size()>1 && firstChar == '-' && unaCifra(static_cast <unsigned char>(tokenStr.at(1))) || unaCifra(firstChar)) {//potrebbe essere un numero intero (primo operando dell'or per numeri negativi, secondo per i positivi))
						int64_t val;
						std::stringstream supp(tokenStr);//creo uno stringstream di supporto alla conversione
						supp >> val;//utilizzo l'operatore di estrazione per convertire automaticamente nel tipo voluto (la stringa viene troncata al primo carattere "non numero" trovato)
						int nCifre = std::to_string(val).size();//calcolo il numero di cifre (tenendo conto del -)
						if (nCifre == tokenStr.length()) {//se il numero di cifre tra stringa e numero sono uguali oppure allora è un numero
							tokens.push_back(Token(Token::NUM, tokenStr, coord));//aggiungo al vector un Token "Num"
						}
						else {//se non era un numero
							std::stringstream error;
							error << "\"" << tokenStr << "\"" << " non risulta essere un numero (riga: " << coord.first << " il token n'" << coord.second << ") dopo \"" << tokens.back().word << "\"" << std::endl;
							throw LexicalError(error.str());//lancio un eccezione in caso di errore lessicale
						}
					}
					else {//se non è sicuramente un numero e non poteva essere una variabile segnalo un errore riguardate il primo carattere
						std::stringstream error;
						error << "Token \"" << tokenStr << "\"" << " non valido (riga: " << coord.first << " il token n'" << coord.second << ") dopo \"" << tokens.back().word << "\"" << std::endl;
						throw LexicalError(error.str());//lancio un eccezione in caso di errore lessicale
					}
				}
			}
			else {
				throw GenericError("Token vuoto...non pensavo potessi arrivare qui, come hai fatto?");//Questo punto del codice non dovrebbe mai essere raggiunta
				return;
			}
		}
	}
}