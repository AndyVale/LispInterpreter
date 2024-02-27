#ifndef PARSER_H
#define PARSER_H
#include "NodeManager.h"
#include "Token.h"
#include "MyExceptions.h"
#include <vector>

//la classe Parser si occupa, attraverso l'utilizzo di un oggetto nodeManagaer, di creare l'albero sintattico
//in base al vettore di token ottenuto dal tokenizer.
//Inoltre lancia errori nel caso ci siano errori nella creazioni degli oggetti: ad esempio si tenti di istanziare
//un oggetto Number passandogli una stringa.
//Anche per questa classe è stata presa grande ispirazione dalla classe sviluppata in aula

class Parser
{
public:
	Parser(NodeManager& m): manager{m}{}
	StmtBlock* operator()(const std::vector<Token>& tokenStream) {//riceve uno stream di token
		std::vector<Token>::const_iterator start = tokenStream.begin();//inizializzo un iteratore con all'inizio del vector di token
		std::vector<Token>::const_iterator tmp = start;//salvo la posizione iniziale del token
		end = tokenStream.end();//salvo la fine (elemento dopo l'ultimo del vector) nell'attributo end
		Node* root = recursiveParse(tmp);//richiamo la funzione di parsing passandogli come parametro l'iteratore
		if (StmtBlock* s = dynamic_cast<StmtBlock*>(root)) {//se l'oggetto restituito è un block o uno statement
			if (((tmp - start) != (tokenStream.size() - 1))) {//controllo non ci siano simboli non letti alla fine 
				std::stringstream error;
				error << "Token inutili alla fine del file dopo l'istruzione " << *(++tmp);
				throw ParseError(error.str());//se ci sono lancio un eccezione
			}
			return s;//restituisco l'oggetto
		}
		else {//altrimenti il programma non rispetta la grammatica
			throw ParseError("L'istruzione esterna deve essere di tipo BLOCK o uno STATEMENT");
			return nullptr;
		}
	}
private:
	NodeManager& manager;//l'oggetto deputato a creare i vari oggetti
	std::vector<Token>::const_iterator end;//segnala la fine dello stream di token così da evitare di iterare oltre l'ultimo token
	
	//metodo che effettivamente permette la creazione dell'albero:
	Node* recursiveParse(std::vector<Token>::const_iterator&);//lancia errori generici sul parsing del codice
	//Metodi ausiliari di recursiveParse che parsano i vari oggetti:
	Block* parseBlock(std::vector<Token>::const_iterator&);
	NumOp* parseNumOp(std::vector<Token>::const_iterator&);
	BoolOp* parseBoolOp(std::vector<Token>::const_iterator&);
	RelOp* parseRelOp(std::vector<Token>::const_iterator&);
	PrintStmt* parsePrintStmt(std::vector<Token>::const_iterator&);
	InputStmt* parseInputStmt(std::vector<Token>::const_iterator&);
	SetStmt* parseSetStmt(std::vector<Token>::const_iterator&);
	WhileStmt* parseWhileStmt(std::vector<Token>::const_iterator&);
	IfStmt* parseIfStmt(std::vector<Token>::const_iterator&);
	//i metodi ausiliari di parse lanciano eccezioni specifiche in base all'oggetto da creare.
	
	//funzione utilizzata per lanciare eccezioni riguardanti uno specifico token: la prima stringa viene stamapta prima
	//del token, la seconda dopo il token (utilizzata per evitare di riscrivere molte volte codice simile):
	void parseError(std::vector<Token>::const_iterator&, std::string, std::string);

	bool safeNext(std::vector<Token>::const_iterator& itr) {
		//restituisce true se posso ancora leggere un elemento, false se non posso
		++itr;
		if (itr == end) {
			throw ParseError("Fine del file inaspettata");
			itr = end;
			return false;
		}
		return true;
	}
};
#endif
