#ifndef NODEMANAGER_H
#define NODEMANAGER_H
#include <vector>
#include "Node.h"
#include "Expr.h"
#include "Statement.h"

//La classse NodeManager si occupa di gestire l'istanziazione e la deallocazione dei vari oggetti.
//Ho preso forte ispirazione dall' ExpressionManager visto a lezione.

class NodeManager
{
public:
    NodeManager() = default;
    ~NodeManager() {//il distruttore si deve occupare di pulire la memoria occupata
        clearMemory();
    }

    NodeManager(const NodeManager& other) = delete;
    NodeManager& operator=(const NodeManager& other) = delete;

    //Tutti i metodi make ricevono i dati da inserire nel costruttore dell'oggetto specificato e restituiscono un puntatore a un'oggetto creato dinamicamente e lo aggiungono al vector "allocated".
   
    Number* makeNumber(int64_t n);
    Variable* makeVariable(std::string name);
    NumOp* makeNumOp(NumOp::numOpCode o, NumExpr* o1, NumExpr* o2);
    BoolConst* makeBoolConst(std::string val);
    BoolOp* makeBoolOp(BoolOp::boolOpCode operand, BoolExpr* op1, BoolExpr* op2);
    RelOp* makeRelOp(RelOp::relOpCode operand, NumExpr* op1, NumExpr* op2);
    PrintStmt* makePrintStmt(NumExpr* o);
    SetStmt* makeSetStmt(Variable* s, NumExpr* e);
    InputStmt* makeInputStmt(Variable* i);
    WhileStmt* makeWhileStmt(BoolExpr* c, StmtBlock* b);
    IfStmt* makeIfStmt(BoolExpr* c, StmtBlock* b1, StmtBlock* b2);
    Block* makeBlock(std::vector<Statement*> sv);

private:
    std::vector<Node*> allocated;
    void clearMemory() {
        auto i = allocated.begin();
        // allocated.end() "marca" la fine del vettore
        // ++i sposta di un elemento la posizione "puntata" dall'iteratore
        for (; i != allocated.end(); ++i) {
            // *i restituisce l'elemento "puntato" dall'iteratore
            delete(*i);//dealloco l'elemento allocato dinamicamente dai metodi make"*"();
        }
        allocated.resize(0);
    }
};
#endif
