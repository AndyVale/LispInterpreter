#include "Statement.h"
#include <sstream>

/*
NOTA:
	Nei metodi "getNome" che si trovano di seguito tutti i dynamic_cast sono dentro un if: questo è per verificare
	che l'oggetto sia effettivamente facente parte della classe indicata per il downcast.
	In questo caso specifico questo non è tuttavia necessario: per come è strutturato il codice negli altri file
	è già sicuro che il downcast venga eseguito con successo.
	Ho deciso di mantenere questa soluzione commentata nel caso avessi voluto ampliare il progetto successivamente.
	Dal momento invece che sono sicuro a tempo di compilazione che gli oggetti restituiti possano essere "downcastati"
	a tempo di compilazione uso direttamente uno static_cast.
*/

//PrintStmt:
NumExpr* PrintStmt::getOutput(){
	return static_cast<NumExpr*>(getFigli()[0]);
	//if (NumExpr* v = dynamic_cast<NumExpr*>(getFigli()[0]))
	//	return v;
	// else throw ParseError("getOutput non restituirebbe una NumExpr");
	//return nullptr;
}

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//


//SetStmt:
Variable* SetStmt::getToSet() {
	return static_cast<Variable*>(getFigli()[0]);

	//if (Variable* v = dynamic_cast<Variable*>(getFigli()[0]))
	//	return v;
	//else throw ParseError("getToSet non restituirebbe una variabile");
	//return nullptr;
}

NumExpr* SetStmt::getValue(){
	return static_cast<NumExpr*>(getFigli()[1]);

	//if (NumExpr* v = dynamic_cast<NumExpr*>(getFigli()[1]))
	//	return v;
	//else throw ParseError("getValue non restituirebbe un'espressione numerica");
	//return nullptr;
}

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//


//InputStmt:
Variable* InputStmt::getInput() {
	return static_cast<Variable*>(getFigli()[0]);

	//if (Variable* v = dynamic_cast<Variable*>(getFigli()[0]))
	//	return v;
	//else throw ParseError("getInput non restituirebbe una variabile");
	//return nullptr;
}

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//WhileStmt:
BoolExpr* WhileStmt::getCondizione() {
	return static_cast<BoolExpr*>(getFigli()[0]);

	//if (BoolExpr* v = dynamic_cast<BoolExpr*>(getFigli()[0]))
	//	return v;
	//else throw ParseError("getCondizione non restituirebbe una condizione");
	//return nullptr;
}

StmtBlock* WhileStmt::getBloccoLoop() {
	return static_cast<StmtBlock*>(getFigli()[1]);

	//if (StmtBlock* b = dynamic_cast<StmtBlock*>(getFigli()[1]))
	//	return b;
	//else throw ParseError("getBloccoLoop non restituirebbe un block-Statement");
	//return nullptr;
}

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//IfStmt:

BoolExpr* IfStmt::getCondizione() {
	return static_cast<BoolExpr*>(getFigli()[0]);

	//if (BoolExpr* c = dynamic_cast<BoolExpr*>(getFigli()[0]))
	//	return c;
	//else throw ParseError("getCondizione non restituirebbe una condizione");
	//return nullptr;
}

StmtBlock* IfStmt::getBloccoVero() {
	return static_cast<StmtBlock*>(getFigli()[1]);

	//if (StmtBlock* b = dynamic_cast<StmtBlock*>(getFigli()[1]))
	//	return b;
	//else throw ParseError("getBloccoVero non restituirebbe un block-Statement");
	//return nullptr;
}

StmtBlock* IfStmt::getBloccoFalso() {
	return static_cast<StmtBlock*>(getFigli()[2]);

	//if (StmtBlock* b = dynamic_cast<StmtBlock*>(getFigli()[2]))
	//	return b;
	//else throw ParseError("getBloccoFalso non restituirebbe un block-Statement");
	//return nullptr;
}
