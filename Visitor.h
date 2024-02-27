#ifndef VISITOR_H
#define VISITOR_H
#include <stack>
#include <vector>
#include <string>
#include <map>

class  NumOp;
class  Variable;
class  Number;
class  BoolOp;
class  RelOp;
struct BoolConst;
class  Block;
class  PrintStmt;
class  SetStmt;
class  InputStmt;
class  WhileStmt;
class  IfStmt;
class  StmtBlock;

class Visitor
{//Classe astratta che stabilisce i metodi di visita che ogni Visitor concreto deve fornire
public:
	virtual void visitNumOp(NumOp*)         = 0;
	virtual void visitVariable(Variable*)   = 0;
	virtual void visitNumber(Number*)       = 0;
	virtual void visitBoolOp(BoolOp*)       = 0;
	virtual void visitRelOp(RelOp*)			= 0;
	virtual void visitBoolConst(BoolConst*) = 0;
	virtual void visitBlock(Block*)         = 0;
	virtual void visitPrintStmt(PrintStmt*) = 0;
	virtual void visitSetStmt(SetStmt*)     = 0;
	virtual void visitInputStmt(InputStmt*) = 0;
	virtual void visitWhileStmt(WhileStmt*) = 0;
	virtual void visitIfStmt(IfStmt*)       = 0;
private:
};

class EvaluationVisitor : public Visitor {
public:
	void visitNumOp(NumOp*)			override;
	void visitVariable(Variable*)   override;
	void visitNumber(Number*)       override;
	void visitBoolOp(BoolOp*)       override;
	void visitRelOp(RelOp*)         override;
	void visitBoolConst(BoolConst*) override;
	void visitBlock(Block*)         override;
	void visitPrintStmt(PrintStmt*) override;
	void visitSetStmt(SetStmt*)     override;
	void visitInputStmt(InputStmt*) override;
	void visitWhileStmt(WhileStmt*) override;
	void visitIfStmt(IfStmt*)       override;
private:
	bool inizialize(Variable*, int64_t);//metodo che permette l'inizializzazione di una variabile nella mappa delle variabili a partire dalla variabile e il valore
	std::map<std::string, std::int64_t> variabili;//tabella delle variabili
	std::stack<int64_t> stack;//stack utilizzato per la valutazione
};

class PrintVisitor : public Visitor {//classe non utilizzata nel main (parte commentata nel main in LispInterpreter.cpp)
public:
	PrintVisitor() : indentazione{ 0 } {}
	virtual void visitNumOp(NumOp*)			override;//tutte virtual per permettere la riscrittura agli altri tipi di printVisitor (come ad esempio il CLIKE)
	virtual void visitVariable(Variable*)   override;
	virtual void visitNumber(Number*)       override;
	virtual void visitBoolOp(BoolOp*)       override;
	virtual void visitRelOp(RelOp*)         override;
	virtual void visitBoolConst(BoolConst*) override;
	virtual void visitBlock(Block*)         override;
	virtual void visitPrintStmt(PrintStmt*) override;
	virtual void visitSetStmt(SetStmt*)     override;
	virtual void visitInputStmt(InputStmt*) override;
	virtual void visitWhileStmt(WhileStmt*) override;
	virtual void visitIfStmt(IfStmt*)       override;
protected:
	void stampIndentato();//funzione che stampa l'indentazione corretta sulla riga
	int indentazione;//attributo che tiene conto dell'indentazione corrente
};

/*PRINTCLIKE

Classe concreta del visitor, realizza la stampa su standardOutput del programma letto convertendolo in linguaggio C++,
è commentato perchè non utile al progetto.
L'ho realizzato per curiosità: volevo vedere se fosse possibile, impostando come standardOutput un file, eseguire il codice "LISP"
passando per il compilatore C++ (A questo punto penso non si parlerebbe più di interprete).

class PrintCLikeVisitor : public PrintVisitor {//classe non utilizzata nel main (parte commentata nel main in LispInterpreter.cpp)
public:
	void stamp(StmtBlock*);//per come è stata realizzata questa classe voglio che sia accessibile dall'esterno solo il metodo che stampa l'intero programma
private:
	void visitNumOp(NumOp*)			override;
	void visitBoolOp(BoolOp*)       override;
	void visitRelOp(RelOp*)         override;
	void visitBoolConst(BoolConst*) override;
	void visitBlock(Block*)         override;
	void visitPrintStmt(PrintStmt*) override;
	void visitSetStmt(SetStmt*)     override;
	void visitInputStmt(InputStmt*) override;
	void visitWhileStmt(WhileStmt*) override;
	void visitIfStmt(IfStmt*)       override;
	std::vector<std::string> inizializzate;//tiene conto delle variabili già inizializzate per evitare la "ridichiarazione con il tipo" ogni volta
};
*/
#endif
