#include "Visitor.h"
#include <iostream>
#include "Expr.h"
#include "Statement.h"
#include "MyExceptions.h"

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//EvaluationVisitor
/*Classe concreta del visitor utilizzata per esegurie il codice Lisp.*/

void EvaluationVisitor::visitNumOp(NumOp* n){
	int64_t val1, val2;
	n->getOp1()->accept(this);//visito l'operando per mettere il valore di esso sullo stack
	val1 = stack.top();//assegno il suo valore a val 1
	stack.pop();//levo dallo stack il valore
	n->getOp2()->accept(this);//ripeto la stessa cosa per val2
	val2 = stack.top();
	stack.pop();

	switch (n->getOperand()) {//in base all'operando metto sullo stack il valore calcolato
		case NumOp::ADD:
			stack.push(val1 + val2);
		break;
		case NumOp::SUB:
			stack.push(val1 - val2);
		break;
		case NumOp::MUL:
			stack.push(val1 * val2);
		break;
		case NumOp::DIV:
			if (val2 != 0)
				stack.push(val1 / val2);
			else
				throw EvaluationError("Impossibile dividere per zero");
		break;
	}
}
void EvaluationVisitor::visitVariable(Variable* v){
	if (variabili.find(v->getId()) != variabili.end()) {//controllo che l'elemento sia stato inizializzato
		stack.push(variabili[v->getId()]);//se è inizializzato lo metto sullo stack
	}
	else {//altrimenti lancio un'eccezione
		std::stringstream error;
		error << "La variabile " << v->getId() << " non risulta essere inizializzata" ;
		throw EvaluationError(error.str());
	}
	
}
void EvaluationVisitor::visitNumber(Number* n){
	stack.push(n->getValue());
}
void EvaluationVisitor::visitBoolOp(BoolOp* bo){
	int64_t val1 = 0, val2 = 0;

	//il primo operando lo devo prendere in ogni caso, il secondo lo carico solo se l'operazione specifica lo richiede:
	bo->getOp1()->accept(this);//visito l'operando per mettere il valore di esso sullo stack, nota che in tutti i casi sarà di tipo int
	val1 = stack.top();//assegno il suo valore a val 1
	stack.pop();//levo dallo stack il valore

	switch (bo->getOperand()) {//Per il funzionamento dei booleani in c, 0 significa falso e 1 significa vero, eseguo delle operazioni tra int che mi permettano di disambiguare
	
	case BoolOp::AND:
		if(val1 == 0)//per fare l'AND circuitato controllo prima val1 e poi val2
			stack.push(0);
		else {
			bo->getOp2()->accept(this);//ripeto la stessa cosa per val2 nel caso in cui val1 non sia falso
			val2 = stack.top();
			stack.pop();
			if(val2 == 0)
				stack.push(0);
			else//allora sono veri entrambi
				stack.push(1);
		}
		
	break;
	case BoolOp::OR:
		if (val1 > 0)//per fare l'OR circuitato controllo prima val1 e poi val2
			stack.push(1);
		else {
			bo->getOp2()->accept(this);//ripeto la stessa cosa per val2 nel caso in cui val1 non sia falso
			val2 = stack.top();
			stack.pop();
			if(val2 > 0)
				stack.push(1);
			else//allora sono entrambi falsi
				stack.push(0);
		}
	break;
	case BoolOp::NOT://nell'operatore not viene considerato solo il primo operando come utile, quindi non carico val2
		if(val1 == 0)
			stack.push(1);
		else
			stack.push(0);
		break;
	}
}

void EvaluationVisitor::visitRelOp(RelOp* ro) {
	int64_t val1 = 0, val2 = 0;

	//il primo operando lo devo prendere in ogni caso, il secondo lo carico solo se l'operazione specifica lo richiede:
	ro->getOp1()->accept(this);//visito l'operando per mettere il valore di esso sullo stack, nota che in tutti i casi sarà di tipo int
	val1 = stack.top();//assegno il suo valore a val 1
	stack.pop();//levo dallo stack il valore
	ro->getOp2()->accept(this);
	val2 = stack.top();
	stack.pop();
	switch (ro->getOperand()) {//Per il funzionamento dei booleani in c, 0 significa falso e 1 significa vero, eseguo delle operazioni tra int che mi permettano di disambiguare

	case RelOp::EQ:
		if (val1 == val2)
			stack.push(1);
		else
			stack.push(0);
		break;
	case RelOp::LT:
		if (val1 < val2)
			stack.push(1);
		else
			stack.push(0);
		break;
	case RelOp::GT:
		if (val1 > val2) {
			stack.push(1);
		}
		else
			stack.push(0);
		break;
	}
}

void EvaluationVisitor::visitBoolConst(BoolConst* b){
	stack.push(BoolConst::boolValueToInt(b->getValue()));//inserisco il valore dell'espressione sullo stack
}

void EvaluationVisitor::visitBlock(Block* bl){
	for (auto s : bl->getFigli()) {
		s->accept(this);
	}
}

void EvaluationVisitor::visitPrintStmt(PrintStmt* s){
	s->getOutput()->accept(this);//immetto il valore da stampare sullo stack
	std::cout << stack.top() << std::endl;//lo stampo
	stack.pop();//levo il valore letto
}

void EvaluationVisitor::visitSetStmt(SetStmt* s){
	s->getValue()->accept(this);//richiamo l'accettazione su getValue per mettere il valore di inizializzazione della variabile sullo stack
	if (!inizialize(s->getToSet(),stack.top())) {//se riesco ad inizializzarla basta
		variabili[s->getToSet()->getId()] = stack.top();//Se era inizializzata devo solo modifcare il valore nella mappa delle variabili
	}
	stack.pop();//faccio saltare il valore sullo stack riguardante s->getValue()
}

void EvaluationVisitor::visitInputStmt(InputStmt* s){
	//prendo in input il valore da assegnare alla variabile
	
	std::string supporto;//supporto al controllo della correttezza
	int nCifre = 0;
	int64_t val = 0;
	std::cout << "inserisci il valore di " << s->getInput()->getId() << ": ";
	std::getline(std::cin, supporto);

	try {
		val = std::stoll(supporto);
		nCifre = static_cast<int>(std::to_string(val).size());
	}
	catch (std::exception) {
		throw GenericError("Il valore inserito non risulta essere un intero");
	}
	if(std::cin.fail() || supporto.size() != nCifre){
		throw GenericError("Il valore inserito non risulta essere un intero");
	}
	else {
		if (!inizialize(s->getInput(), val)) {//se riesco ad inizializzarla basta
			variabili[s->getInput()->getId()] = val;//Se era inizializzata devo solo modifcare il valore nella mappa delle 
		}
	}
}

void EvaluationVisitor::visitWhileStmt(WhileStmt* s){
	s->getCondizione()->accept(this);//metto la condizione sullo stack
	while (stack.top() == 1) {//se la condizione è vera itero
		stack.pop();//levo la condizione appena guardata
		s->getBloccoLoop()->accept(this);//eseguo il blocco di loop
		s->getCondizione()->accept(this);//ricalcolo la condizione
	}
}

void EvaluationVisitor::visitIfStmt(IfStmt* s){
	s->getCondizione()->accept(this);//metto la condizione sullo stack
	if (stack.top() == 1) {//se la condizone è vera eseguo il blocco vero
		s->getBloccoVero()->accept(this);
	}
	else {//altrimenti eseguo quello falso
		s->getBloccoFalso()->accept(this);
	}
}

bool EvaluationVisitor::inizialize(Variable* var, int64_t value) {
	std::string id = var->getId();
	return variabili.insert(std::pair<std::string, int64_t>(id, value)).second;
}

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//PrintVisitor
/*Classe concreta del visitor che permette la stampa l'albero sintattico in "Lisp".*/


void PrintVisitor::stampIndentato() {//stampa tante tab quante richiesto
	for (int i = 0; i < indentazione; i++) {
		std::cout << "\t";
	}
}

void PrintVisitor::visitNumOp(NumOp* t) {//Stampa un'espressione numerica in C-like
	std::cout << "(" << NumOp::numOpCodeToString(t->getOperand()) << " ";//l'operando non è un oggetto che può essere visitato, lo stampo quindi così
	t->getOp1()->accept(this);
	std::cout << " ";
	t->getOp2()->accept(this);
	std::cout << ")";
}

void PrintVisitor::visitVariable(Variable* v) {
	std::cout << v->getId();
}

void PrintVisitor::visitNumber(Number* n) {
	std::stringstream t;//utilizzo uno stringstream di supporto
	t << n->getValue();
	std::cout << t.str();
}

void PrintVisitor::visitBoolOp(BoolOp* b) {//Stampa un PrintStmt visitando l'espressione che deve stampare 
	std::cout << "( " << BoolOp::boolOpCodeToString(b->getOperand()) << " "; //l'operando non è un oggetto che può essere visitato, lo stampo quindi così
	b->getOp1()->accept(this);
	std::cout << " ";
	if (b->getOp2() != nullptr) {
		b->getOp2()->accept(this);
		std::cout << " ";
	}
	std::cout << ")";
}

void PrintVisitor::visitRelOp(RelOp* r) {
	std::cout << "( " << RelOp::relOpCodeToString(r->getOperand()) << " "; //l'operando non è un oggetto che può essere visitato, lo stampo quindi così
	r->getOp1()->accept(this);
	std::cout << " ";
	if (r->getOp2() != nullptr) {
		r->getOp2()->accept(this);
		std::cout << " ";
	}
	std::cout << ")";
}

void PrintVisitor::visitBoolConst(BoolConst* b) {
	std::cout << BoolConst::boolValueToString(b->getValue());
}

void PrintVisitor::visitBlock(Block* bl) {
	std::cout << "(BLOCK";
	indentazione++;
	for (auto t : bl->getFigli()) {
		std::cout << std::endl;
		stampIndentato();
		t->accept(this);
	}std::cout << std::endl;
	indentazione--;
	stampIndentato();
	std::cout << ")";
}

void PrintVisitor::visitPrintStmt(PrintStmt* s) {//Stampa un PrintStmt visitando l'espressione che deve stampare
	std::cout << "(PRINT ";
	s->getOutput()->accept(this);
	std::cout << ")";
}

void PrintVisitor::visitSetStmt(SetStmt* s) {
	std::cout << "(SET ";
	std::cout << s->getToSet()->getId();//stampo l'id della variabile
	std::cout << " ";
	s->getValue()->accept(this);//Stampo attraverso il visitor il valore della variabile
	std::cout << ")";
}
void PrintVisitor::visitInputStmt(InputStmt* s) {
	std::cout << "(INPUT ";
	s->getInput()->accept(this);
	std::cout << ")";
}
void PrintVisitor::visitWhileStmt(WhileStmt* s) {
	BoolExpr* cond = s->getCondizione();
	StmtBlock* loop = s->getBloccoLoop();
	std::cout << "(WHILE  ";
	cond->accept(this);
	std::cout << std::endl;
	indentazione++;
	stampIndentato();
	loop->accept(this);
	std::cout << std::endl;
	indentazione--;
	stampIndentato();
	std::cout << ")";
}
void PrintVisitor::visitIfStmt(IfStmt* s) {
	BoolExpr* cond = s->getCondizione();
	StmtBlock* vero = s->getBloccoVero();
	StmtBlock* falso = s->getBloccoFalso();
	std::cout << "(IF ";//stamp l'if e la condizione
	cond->accept(this);
	std::cout << std::endl;

	indentazione++;//aumento l'indentazione
	stampIndentato();
	vero->accept(this);//e stampo il ramo vero
	std::cout << std::endl;
	stampIndentato();
	falso->accept(this);//e dopo essere andato a capo il ramo falso
	std::cout << std::endl;
	indentazione--;//diminuisco l'indentazione

	stampIndentato();
	std::cout << ")";
}


//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//PrintCLikeVisitor
//In Visitor.h maggiori spiegazioni
/*
void PrintCLikeVisitor::visitNumOp(NumOp* t) {//Stampa un'espressione numerica in C-like
	NumOp::numOpCode op = t->getOperand();
	char segno = '#';
	switch (op) {
	case NumOp::ADD:
		segno = '+';
	break;
	case NumOp::SUB:
		segno = '-';
	break;
	case NumOp::MUL:
		segno = '*';
	break;
	case NumOp::DIV:
		segno = '/';
	break;
	}
	t->getOp1()->accept(this);
	std::cout << " " << segno << " ";//l'operando non è un oggetto che può essere visitato, lo stampo quindi così
	t->getOp2()->accept(this);
}

void PrintCLikeVisitor::visitBoolOp(BoolOp* b) {//Stampa un PrintStmt visitando l'espressione che deve stampare 
	BoolOp::boolOpCode op = b->getOperand();
	std::string segno = "";
	switch (op) {
	case BoolOp::AND:
		segno = "&&";
		break;
	case BoolOp::OR:
		segno = "||";
		break;
	case BoolOp::NOT:
		segno = "!";
		break;
	}
	if (segno == "!") {
		std::cout << segno;
		b->getOp1()->accept(this);
		return;
	}
	b->getOp1()->accept(this);
	std::cout << " " << segno << " ";
	b->getOp2()->accept(this);
}

void PrintCLikeVisitor::visitBoolConst(BoolConst* b) {//Stampa un PrintStmt visitando l'espressione che deve stampare 
	switch (b->getValue()) {
	case BoolConst::TRUE:
		std::cout << "true";
		break;
	case BoolConst::FALSE:
		std::cout << "false";
		break;
	}
}

void PrintCLikeVisitor::visitRelOp(RelOp* r) {
	RelOp::relOpCode op = r->getOperand();
	std::string segno = "";
	switch (op) {
	case RelOp::LT:
		segno = "<";
		break;
	case RelOp::GT:
		segno = ">";
		break;
	case RelOp::EQ:
		segno = "==";
		break;
	}
	r->getOp1()->accept(this);
	std::cout << " "<<  segno <<" ";
	r->getOp1()->accept(this);
}

void PrintCLikeVisitor::visitBlock(Block* bl) {
	for (auto t : bl->getFigli()) {
		std::cout << std::endl;
		stampIndentato();
		t->accept(this);
	}std::cout << std::endl;
}

void PrintCLikeVisitor::visitPrintStmt(PrintStmt* s) {//Stampa un PrintStmt visitando l'espressione che deve stampare
	std::cout << "std::cout << ";
	s->getOutput()->accept(this);
	std::cout << ";" ;
}

void PrintCLikeVisitor::visitSetStmt(SetStmt* s) {
	std::vector<std::string>::iterator it = std::find(inizializzate.begin(), inizializzate.end(), s->getToSet()->getId());//controllo se la stringa "id" della variabile è nel vector
	if (it == inizializzate.end()){//se non c'è allora stampo la dichiarazione del tipo e aggiungo l'id nel vector delle variabili inizializzate
		std::cout << "int64_t ";
		inizializzate.push_back(s->getToSet()->getId());
	}
	std::cout << s->getToSet()->getId();//stampo l'id della variabile
	std::cout << " = ";
	s->getValue()->accept(this);//Stampo attraverso il visitor il valore della variabile
	std::cout << ";";
}
void PrintCLikeVisitor::visitInputStmt(InputStmt* s) {
	std::vector<std::string>::iterator it = std::find(inizializzate.begin(), inizializzate.end(), s->getInput()->getId());//controllo se la stringa "id" della variabile è nel vector
	if (it == inizializzate.end()) {//se non c'è allora stampo la dichiarazione del tipo e aggiungo l'id nel vector delle variabili inizializzate
		std::cout << "int64_t ";
		inizializzate.push_back(s->getInput()->getId());
		std::cout << s->getInput()->getId() << ";"<< std::endl;
	}
	stampIndentato();
	std::cout << "std::cout << \"inserisci il valore di: \" << " << s->getInput()->getId() << ";"<<std::endl;
	stampIndentato();
	std::cout << "std::cin >> ";
	s->getInput()->accept(this);
	std::cout << ";";
}
void PrintCLikeVisitor::visitWhileStmt(WhileStmt* s) {
	BoolExpr* cond = s->getCondizione();
	StmtBlock* loop = s->getBloccoLoop();
	std::cout << "while(";
	cond->accept(this);
	std::cout << "){ ";
	std::cout << std::endl;
	indentazione++;
	stampIndentato();
	loop->accept(this);
	std::cout << std::endl;
	indentazione--;
	stampIndentato();
	std::cout << "}";
}
void PrintCLikeVisitor::visitIfStmt(IfStmt* s) {
	BoolExpr* cond = s->getCondizione();
	StmtBlock* vero = s->getBloccoVero();
	StmtBlock* falso = s->getBloccoFalso();
	std::cout << "if(";//stamp l'if e la condizione
	cond->accept(this);
	std::cout <<"){" << std::endl;

	indentazione++;//aumento l'indentazione
	stampIndentato();
	vero->accept(this);//e stampo il ramo vero
	indentazione--;
	std::cout << std::endl;
	stampIndentato();
	std::cout <<"}";
	std::cout << "else{" << std::endl;
	indentazione++;
	stampIndentato();
	falso->accept(this);//e dopo essere andato a capo il ramo falso
	indentazione--;//diminuisco l'indentazione
	std::cout << std::endl;
	stampIndentato();
	std::cout << "}" << std::endl;
}
void PrintCLikeVisitor::stamp(StmtBlock* bl) {
	indentazione++;
	std::cout << "int main(){";
	bl->accept(this);
	stampIndentato();
	std::cout << "return 0;"<<std::endl<<"}" << std::endl;
	indentazione--;
}*/