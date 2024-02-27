#ifndef STATEMENT_H
#define STATEMENT_H
#include "Expr.h"
#include <vector>
#include "Node.h"

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//Statement
/*	Classe virtuale che stabilisce la struttura di uno statement generico.
	Non aggiunge attributi ma è utile per la "gestione della gerarchia".
 */

class Statement : public StmtBlock
{
public:
	Statement() = default;
	~Statement() = default;//non implementando il metodo accept rimane una classe puramente virtuale
private:
};

//-------------------//										NOTA:								//-------------------//
/*	
	Tutte le classi derivanti da Statement hanno dei metodi getters (getNome), però sono sprovvisti di variabili private 
	aventi i nomi indicati dopo il get.
	Questi metodi getNome restituiscono un Node (opportuno e opportuanmente downcastato) del vector "figli" (che tutti gli 
	statement ereditano dalla superclasse StmtBlock:

	Ricordo la gerarchia delle classi:		Node->StmtBlock->Statement->"Concrete"Statement
*/


//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//Block
/*Classe concreta che rappresenta un BLOCK, cioè un "aggregato" di statement (che ha come figli).*/

class Block : public StmtBlock
{
public:
	Block(std::vector<Statement*> sv) {
		for (auto s : sv)	aggiungiFiglio(s);
	}
	~Block() = default;

	void accept(Visitor* v) override {
		v->visitBlock(this);
	}
private:
};

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//PrintStmt
/*Classe concreta che rappresenta un PRINT statement*/

class PrintStmt : public Statement {//Per mandare in output un valore (numerico o variabile)
public:
	PrintStmt(NumExpr* o) { aggiungiFiglio(o); }
	~PrintStmt() = default;

	void accept(Visitor* v) override {
		v->visitPrintStmt(this);
	}

	NumExpr* getOutput();
private:
};

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//SetStmt
/*Classe concreta che rappresenta un SET statement*/

class SetStmt : public Statement {//Pe definire e inizializzare una variabile
public:
	SetStmt(Variable* s, NumExpr* e){
		aggiungiFiglio(s); aggiungiFiglio(e);
	}
	~SetStmt() = default;


	void accept(Visitor* v) override {
		v->visitSetStmt(this);
	}

	Variable* getToSet();
	NumExpr* getValue();
private:
};
//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//InputStmt
/*Classe concreta che rappresenta un INPUT statement*/

class InputStmt : public Statement {//Assegnare un valore dato dall'utente tramite input (normalmente console) a una variabile
public:
	InputStmt(Variable* i) { aggiungiFiglio(i); }
	~InputStmt() = default;


	void accept(Visitor* v) override {
		v->visitInputStmt(this);
	}

	Variable* getInput();
private:
};

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//WhileStmt
/*Classe concreta che rappresenta un WHILE statement*/

class WhileStmt : public Statement {//Eseguire in loop il blocco fintanto che la condizione è vera
public:
	WhileStmt(BoolExpr* c, StmtBlock* b){
		aggiungiFiglio(c); aggiungiFiglio(b);
	}
	~WhileStmt() = default;

	void accept(Visitor* v) override {
		v->visitWhileStmt(this);
	}

	BoolExpr* getCondizione();
	StmtBlock* getBloccoLoop();
private:
};

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//IfStmt
/*Classe concreta che rappresenta un IF statement*/

class IfStmt : public Statement {//In base al fatto che la condizione sia vera o falsa viene eseguito il primo stmtblock altrimenti il secondo
public:
	IfStmt(BoolExpr* c, StmtBlock* b1, StmtBlock* b2){
		aggiungiFiglio(c); aggiungiFiglio(b1); aggiungiFiglio(b2);
	}
	~IfStmt() = default;

	void accept(Visitor* v) override {
		v->visitIfStmt(this);
	}

	BoolExpr* getCondizione();
	StmtBlock* getBloccoVero();
	StmtBlock* getBloccoFalso();
private:
};
#endif
