#include "Parser.h"

void Parser::parseError(std::vector<Token>::const_iterator& itr, std::string pre, std::string post) { //funzione che permette di comporre le eccezioni in modo un modo stabilito (e per evitare di dover riscrivere ogni volta il codice)
	std::stringstream error;
	error << pre << *itr << post;
	throw ParseError(error.str());
}

Block* Parser::parseBlock(std::vector<Token>::const_iterator& itr) {
	std::vector<Token>::const_iterator old = itr;
	Block* bl = nullptr;//creo l'oggetto blocco
	std::vector<Statement*> tmp;
	safeNext(itr);//vado avanti il l'iterator
	if (itr->tag != Token::SP) {//se non incontro una parentesi aperta
		parseError(old, "", " deve contenere almeno uno statement racchiuso da parentesi");
	}
	else {
		for (; itr != end && itr->tag == Token::SP;
			safeNext(itr)) {//faccio avanzare il token inizialmente
			old = itr;//salvo la posizione dell'itr in caso ci fossero degli errori
			Node* t1 = recursiveParse(itr);//leggo lo statement successivo e lo parso usando la funzione principale
			if (Statement* tmp1 = dynamic_cast<Statement*>(t1)) {//controllo sia uno statement
				tmp.push_back(tmp1);//aggiungo al vector temporaneo lo statement appena downcastato
			}
			else {//altrimenti lancio un errore
				parseError(old, "L'espressione dopo ", " deve essere uno statement dato che fa parte di un \"BLOCK\"");
			} 
		}
		--itr;//mando indietro l'iteratore perchè ha letto una token in più a causa dell'operazione eseguita dal for
			  //operazione lecita dato che dovrebbe aver letto la ), se questa non ci fosse il problema verrebbe gestito da recursiveParse
			  //se fossi invece alla fine del file safeNext lancierebbe un'eccezione "in anticipo" rispetto a recursiveParse
		      //ma questo comportamento non genera problematicità
		bl = manager.makeBlock(tmp);//creo l'oggetto block se non ho restituito prima errore
		return bl;
	}
}

NumOp* Parser::parseNumOp(std::vector<Token>::const_iterator& itr) {
	//Tento di creare una NumOp
	NumOp* num = nullptr;
	std::vector<Token>::const_iterator old = itr;//salvo il token in caso di errore
	NumOp::numOpCode tmp = NumOp::stringToNumOpCode(itr->word);//salvo l'operatore
	safeNext(itr);

	Node* t1 = recursiveParse(itr);//parso il primo operando
	if (NumExpr* tmp1 = dynamic_cast<NumExpr*>(t1)) {//se è un'espressione numerica
		safeNext(itr);
		Node* t2 = recursiveParse(itr);//parso il secondo
		if (NumExpr* tmp2 = dynamic_cast<NumExpr*>(t2)) {//se è un'espressione numerica
			num = manager.makeNumOp(tmp, tmp1, tmp2);//allora creo effettivamente l'oggetto num
			return num;
		}else {
			parseError(old, "La seconda espressione dopo ", " deve essere un'espressione numerica");
		}
	}
	else {
		parseError(old, "La seconda espressione dopo ", " deve essere un'espressione numerica");
	}
}

BoolOp* Parser::parseBoolOp(std::vector<Token>::const_iterator& itr) {//funzione che parsa le operatori con AND e OR
	BoolOp* op = nullptr;
	std::vector<Token>::const_iterator old = itr;//salvo il token in caso di errore
	BoolOp::boolOpCode tmp = BoolOp::stringToBoolOpCode(itr->word);//salvo l'operatore
	safeNext(itr);
	Node* t1 = recursiveParse(itr);//parso il primo operando
	if (BoolExpr* tmp1 = dynamic_cast<BoolExpr*>(t1)) {//se è un'espressione booleana
		if (tmp != BoolOp::boolOpCode::NOT) {
			safeNext(itr);
			Node* t2 = recursiveParse(itr);//parso il secondo
			if (BoolExpr* tmp2 = dynamic_cast<BoolExpr*>(t2)) {//se è un'espressione booleana
				op = manager.makeBoolOp(tmp, tmp1, tmp2);//allora creo l'oggetto
				return op;
			}
			else {
				parseError(old, "La seconda espressione dopo ", " deve essere un'espressione booleana");
			}
		}
		else {
			op = manager.makeBoolOp(tmp, tmp1, tmp1);//creo l'oggetto passando al nodeManager solo tmp1
			return op;
		}
	}
	else {
		parseError(old, "La prima espressione dopo ", " deve essere un'espressione booleana");
	}
}

RelOp* Parser::parseRelOp(std::vector<Token>::const_iterator& itr) {
	RelOp* op = nullptr;
	std::vector<Token>::const_iterator old = itr;
	RelOp::relOpCode tmp = RelOp::stringToRelOpCode(itr->word);//salvo l'operatore
	safeNext(itr);
	Node* t1 = recursiveParse(itr);//parso il primo operando
	if (NumExpr* tmp1 = dynamic_cast<NumExpr*>(t1)) {//controllo sia un'espressione numerica
		safeNext(itr);
		Node* t2 = recursiveParse(itr);//parso il secondo operando
		if (NumExpr* tmp2 = dynamic_cast<NumExpr*>(t2)) {//controllo sia un'espressione numerica
			op = manager.makeRelOp(tmp, tmp1, tmp2);
			return op;
		}
		else {
			parseError(old, "La seconda espressione dopo ", " deve essere un'espressione numerica");
		}
	}
	else{//se avviene qualsiasi problema e quindi non restituisco op sono incorso in un errore
		parseError(old, "La prima espressione dopo ", " deve essere un'espressione numerica");
	}
}

PrintStmt* Parser::parsePrintStmt(std::vector<Token>::const_iterator& itr) {
	PrintStmt* stmt = nullptr;
	std::vector<Token>::const_iterator old = itr;
	safeNext(itr);
	Node* t1 = recursiveParse(itr);
	if (NumExpr* tmp1 = dynamic_cast<NumExpr*>(t1)) {//il figlio dei PrintStmt devono essere NumExpr
		stmt = manager.makePrintStmt(tmp1);
		return stmt;
	}
	else{
		parseError(old, "La prima espressione dopo ", " deve essere un'espressione numerica");
	}
}

InputStmt* Parser::parseInputStmt(std::vector<Token>::const_iterator& itr) {
	InputStmt* stmt = nullptr;
	std::vector<Token>::const_iterator old = itr;

	safeNext(itr);
	Node* t1 = recursiveParse(itr);
	if (Variable* tmp1 = dynamic_cast<Variable*>(t1)) {//il figlio dei InputStmt devono essere una Variable
		stmt = manager.makeInputStmt(tmp1);
		return stmt;
	}
	else{
		parseError(old, "La prima espressione dopo ", " deve essere un identificativo di variabile valido");
	}
}

SetStmt* Parser::parseSetStmt(std::vector<Token>::const_iterator& itr) {
	SetStmt* stmt = nullptr;
	std::vector<Token>::const_iterator old = itr;
	safeNext(itr);
	Node* t1 = recursiveParse(itr);						//per creare un oggetto SetStmt
	if (Variable* tmp1 = dynamic_cast<Variable*>(t1)) { //il primo elemento deve essere una variabile
		safeNext(itr);
		Node* t2 = recursiveParse(itr);
		if (NumExpr* tmp2 = dynamic_cast<NumExpr*>(t2)) {//la seconda una numExpr
			stmt = manager.makeSetStmt(tmp1, tmp2);
			return stmt;
		}
		else {
			parseError(old, "La seconda espressione dopo ", " deve essere un'espressione numerica");
		}
	}
	else{
		parseError(old, "La prima espressione dopo ", " deve essere un identificativo di variabile valido");
	}
}

IfStmt* Parser::parseIfStmt(std::vector<Token>::const_iterator& itr) {
	IfStmt* stmt = nullptr;
	std::vector<Token>::const_iterator old = itr;
	safeNext(itr);
	Node* t1 = recursiveParse(itr);
	if (BoolExpr* tmp1 = dynamic_cast<BoolExpr*>(t1)) {//il primo elemento deve essere una variabile
		safeNext(itr);
		Node* t2 = recursiveParse(itr);
		if (StmtBlock* tmp2 = dynamic_cast<StmtBlock*>(t2)) {//il secondo uno statement-block
			safeNext(itr);
			Node* t3 = recursiveParse(itr);
			if (StmtBlock* tmp3 = dynamic_cast<StmtBlock*>(t3)) {//e anche il terzo uno statement-block
				stmt = manager.makeIfStmt(tmp1, tmp2, tmp3);
				return stmt;
			}
			else {
				parseError(old, "La terza istruzione dopo ", " dovrebbe essere uno statement o un blocco");
			}
		}
		else {
			parseError(old, "La seconda istruzione dopo ", " dovrebbe essere uno statement o un blocco");
		}
	}
	else{
		parseError(old, "La prima istruzione dopo ", " dovrebbe essere un'espressione booleana");
	}
}
WhileStmt* Parser::parseWhileStmt(std::vector<Token>::const_iterator& itr) {
	WhileStmt* stmt = nullptr;
	std::vector<Token>::const_iterator old = itr;//in caso di errore salvo il valore del token "WHILE"
	safeNext(itr);
	Node* t1 = recursiveParse(itr);
	if (BoolExpr* tmp1 = dynamic_cast<BoolExpr*>(t1)) {//il primo elemento deve essere un'espressione booleana
		safeNext(itr);
		Node* t2 = recursiveParse(itr);
		if (StmtBlock* tmp2 = dynamic_cast<StmtBlock*>(t2)) {//il secondo uno statement-block
			stmt = manager.makeWhileStmt(tmp1, tmp2);
			return stmt;
		}
		else {
			parseError(old, "La seconda istruzione dopo ", " dovrebbe essere uno statement o un blocco");
		}
	}
	else{
		parseError(old, "La prima istruzione dopo ", " dovrebbe essere un'espressione booleana");
	}
}

Node* Parser::recursiveParse(std::vector<Token>::const_iterator& itr) {
	Node* root = nullptr;
	int tipo;
	if (itr->tag == Token::SP) {//allora ho un'istruzione di tipo StmtBlock (ha dei figli) -> dovrò chiamare una funzione parse appropriata
		safeNext(itr);//ora l'iterator dovrebbe puntare a una parola chiave che ci permetta di disambiguare quello che dobbiamo fare in seguito, altrimenti siamo di fronte a un errore		
		if (Token::BLOCK <= itr->tag && itr->tag <= Token::NOT) {//tutti i valori di tag esclusi da questo if sono quelli associati ai token di : {(, ), TRUE, FALSE, VAR, NUM}
			//senza spostare l'iterator controllo il tag, in base al valore assunto dovrò richiamare la funzione di gestione corretta dell'operazione
			tipo = itr->tag;
			switch (tipo) {

			case Token::BLOCK://Block: devo eseguire tante parseRicorsive quante parentesi aperte trovo dopo
				root = parseBlock(itr);
			break;

			case Token::ADD: case Token::SUB: case Token::MUL: case Token::DIV://Tento di creare una NumOp
				root = parseNumOp(itr);
			break;

			case Token::AND: case Token::OR: case Token::NOT://Tento di creare una BoolOp
				root = parseBoolOp(itr);
			break;
			case  Token::GT: case Token::LT: case Token::EQ:
				root = parseRelOp(itr);
			break;
			case Token::PRINT://Tento di creare un PrintStmt
				root = parsePrintStmt(itr);
			break;
			case Token::INPUT://Tento di creare un InputStmt
				root = parseInputStmt(itr);
			break;
			case Token::SET://Tento di creare un InputStmt
				root = parseSetStmt(itr);
			break;
			case Token::IF://Tento di creare un whileStmt
				root = parseIfStmt(itr);
			break;
			case Token::WHILE://Tento di creare un IfStmt
				root = parseWhileStmt(itr);
			break;
			default: {//qui dentro non dovrebbe mai arrivare l'esecuzione
					parseError(itr, "Token non riconosciuto:  ", "");
				}
			}
			safeNext(itr);//devo aumentare l'iteratore per controllare che la produzione termini con una parentesi chiusa
			if (itr->tag != Token::CP) {//se è diverso allora lancio un'eccezione
				parseError(itr, "Token non previsto: ", ", era prevista la chiusura di una parentesi");
			}
		}
		else {//non sono riuscito a disambiguare la parola
			parseError(itr, "Simbolo inaspettato ", " deve essere una parola chiave valida");
		}
	}
	else {//sono in un caso in cui non devo chiamare funzioni parse: guardo se è una variabile, un espressione numerica "semplice" oppure un espressione booleana semplice, in quei casi ci sta che possa fare la converisone senza parentesi
		if (itr->tag == Token::NUM) {//Se è un numero
			int64_t val;
			std::stringstream supp(itr->word);//crea un Number convertendo il valore word del tag in intero
			supp >> val;
			root = manager.makeNumber(val);
		}
		else if (itr->tag == Token::VAR) {//Se è una variabile
			root = manager.makeVariable(itr->word);//crea un Variable
		}
		else if (itr->tag == Token::TRUE || itr->tag == Token::FALSE) {//Se è una parola chiave dei valori booleani
			root = manager.makeBoolConst(itr->word);//crea una BoolConst settandola al valore di word
		}
		else//se non è nulla di questi Token allora vuol dire che non è stata letta la parentesi aperta
			{
				parseError(itr, "Il token ", " non risulta essere valido, probabilmente dovrebbe essere preceduto da altri token");
			}
	}
	return root;
}
