#include "NodeManager.h"

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//Classi derivanti da NumExpr:

Number* NodeManager::makeNumber(int64_t n){//dato una stringa crea dinamicamente un oggetto di tipo Number
	Number* tmp = new Number(n);
	allocated.push_back(tmp);
	return tmp;
}
Variable* NodeManager::makeVariable(std::string name) {//dato una stringa crea dinamicamente un oggetto di tipo variable
	Variable* tmp = new Variable(name);
	allocated.push_back(tmp);
	return tmp;
}
NumOp* NodeManager::makeNumOp(NumOp::numOpCode o, NumExpr* o1, NumExpr* o2){//date due NumExpr e un numOpCode crea dinamicamente un oggetto NumOp
	NumOp* tmp = new NumOp(o, o1, o2);
	allocated.push_back(tmp);
	return tmp;
}

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//Classi derivanti da BoolExpr

BoolOp* NodeManager::makeBoolOp(BoolOp::boolOpCode operand, BoolExpr* op1, BoolExpr* op2) {//per qualsiasi operando
	BoolOp* tmp = nullptr;
	if (operand == BoolOp::NOT) {
			tmp = new BoolOp(operand, op1, nullptr);//richiamo il costruttore del not
	}
	else {
			tmp = new BoolOp(operand, op1, op2);//chiamo il costruttore generico
	}
	allocated.push_back(tmp);
	return tmp;
}

RelOp* NodeManager::makeRelOp(RelOp::relOpCode operand, NumExpr* op1, NumExpr* op2) {//per qualsiasi operando
	RelOp* tmp = new RelOp(operand, op1, op2);//richiamo il costruttore del relOp
	allocated.push_back(tmp);
	return tmp;
}

BoolConst* NodeManager::makeBoolConst(std::string val) {
	BoolConst* tmp = new BoolConst(val);
	allocated.push_back(tmp);
	return tmp;
}

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//Classi derivanti da Statement

PrintStmt* NodeManager::makePrintStmt(NumExpr* o) {
	PrintStmt* tmp = new PrintStmt(o);
	allocated.push_back(tmp);
	return tmp;
}
SetStmt* NodeManager::makeSetStmt(Variable* s, NumExpr* e) {
	SetStmt* tmp = new SetStmt(s, e);
	allocated.push_back(tmp);
	return tmp;
}
InputStmt* NodeManager::makeInputStmt(Variable* i) {
	InputStmt* tmp = new InputStmt(i);
	allocated.push_back(tmp);
	return tmp;
}
WhileStmt* NodeManager::makeWhileStmt(BoolExpr* c, StmtBlock* b) {
	WhileStmt* tmp = new WhileStmt(c, b);
	allocated.push_back(tmp);
	return tmp;
}
IfStmt* NodeManager::makeIfStmt(BoolExpr* c, StmtBlock* b1, StmtBlock* b2) {
	IfStmt* tmp = new IfStmt(c, b1, b2);
	allocated.push_back(tmp);
	return tmp;
}

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//Classe Block

Block* NodeManager::makeBlock(std::vector<Statement*> sv) {
	Block* tmp = new Block(sv);
	allocated.push_back(tmp);
	return tmp;
}
