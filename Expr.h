#ifndef EXPR_H
#define EXPR_H

#include "Node.h"
#include "Visitor.h"
#include <map>
#include "MyExceptions.h"

struct Node;
class Visitor;

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//NumExpr
/*È la classe astratta che rappresenta un'espressione numerica qualsiasi, l'utilizzo principale è quello
  di permettere di trattare, in alcuni ambiti, le classi concrete derivate in modo più semplice*/

class NumExpr : public Node
{
public:
    NumExpr() = default;
    ~NumExpr() = default;
private:
};

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//Variable
/*Classe concreta di NumExpr che rappresenta una variabile, non ha figli*/

class Variable : public NumExpr
{
public:
    Variable() = delete;
    Variable(std::string i) : id{ i } {//creo la variabile ma solo a livello di oggetto, diventerò una variabile vera e propria solo quando verrà richiamato il metodo inizialize()
    }
    void accept(Visitor* v) override {//metodo per l'accettazione del visitor
        v->visitVariable(this);
    }
    std::string getId() const {
        return id;
    }

private:
    std::string id;
};


//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//Number
/*Classe concreta di NumExpr che rappresenta un numero intero, non ha figli*/

class Number : public NumExpr
{
public:
    Number() = delete;
    Number(int64_t n) : value{ n } {}
    ~Number() = default;

    void accept(Visitor* v) override {
        v->visitNumber(this);
    }

    int64_t getValue() const {
        return value;
    }

private:
    int64_t value;
};

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//NumOp
/*Classe concreta che rappresenta un'espressione numerica, non ha figli*/

class NumOp: public NumExpr
{
//L'unico errore che la classe gestisce "in autonomia" è quello sulla gestione delle divisioni per 0 (costante)
public:
    //dicharo le tipologie di operazioni utilizzabili:
    enum numOpCode {//gli enum sono automaticamente static
        ADD, SUB, MUL, DIV
    };

    //funzioni statiche utili
    static numOpCode stringToNumOpCode(std::string s) {//converte una stringa negli enum
        if (s == "ADD") return ADD;
        if (s == "SUB") return SUB;
        if (s == "MUL") return MUL;
        if (s == "DIV") return DIV;
        throw GenericError("stringToNumOpCode failed");
    }
    static std::string numOpCodeToString(numOpCode op) {//converte gli enum in una stringa
        switch (op) {
        case ADD: return "ADD";
        case SUB: return "SUB";
        case MUL: return "MUL";
        case DIV: return "DIV";
        }
        throw GenericError("numOpCodeToString failed");
    }


    NumOp() = delete;
    NumOp(numOpCode o, NumExpr* o1, NumExpr* o2) : operand{ o }, op1{ o1 }, op2{ o2 } {}
    ~NumOp() = default;

    void accept(Visitor* v) override {
        v->visitNumOp(this);
    }

    numOpCode getOperand() const {
        return operand;
    }
    NumExpr* getOp1() const {
        return op1;
    }
    NumExpr* getOp2() const {
        return op2;
    }

private:
    numOpCode operand;
    NumExpr* op1;
    NumExpr* op2;
};

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//BoolExpr
/*È la classe astratta che, similmente a NumExpr, rappresenta un'espressione booleana qualsiasi.*/

class BoolExpr: public Node
{
public:
    BoolExpr() = default;
    ~BoolExpr() = default;

private:
};

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//BoolOp
/*Similmente a NumOp è la classe concreta che rappresenta operazioni booleane.*/

class BoolOp: public BoolExpr
{
public:
    enum boolOpCode {
        AND, OR, NOT
    };

    BoolOp() = delete;
    BoolOp(boolOpCode o, BoolExpr* o1, BoolExpr* o2) : operand{ o }, op1{ o1 }, op2{ o2 } {
        if (o == NOT) op2 = nullptr;//se è un NOT allora impongo il secondo operando a nullptr in ogni caso
    };

    static std::string boolOpCodeToString(boolOpCode op) {//converte l'enum in una stringa
        switch (op) {
        case AND: return "AND";
        case OR: return "OR";
        case NOT: return "NOT";
        }
        throw GenericError("boolOpCodeToString failed");
    }
    static boolOpCode stringToBoolOpCode(std::string s) {//converte la stringa in un enum 
        if (s == "AND") return AND;
        if (s == "OR") return OR;
        if (s == "NOT") return NOT;
        throw GenericError("stringToBoolOpCode failed");
    }
    void accept(Visitor* v) override {
        v->visitBoolOp(this);
    }
    
    boolOpCode getOperand() const{
    return operand;
}
    BoolExpr* getOp1() const {
        return op1;
    }
    BoolExpr* getOp2() const {
        return op2;
    }

private:
    boolOpCode operand;
    BoolExpr* op1;
    BoolExpr* op2;
};

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//BoolOp
/*È la classe concreta che rappresenta espressioni di confronto tra numeri interi (<,>,=).*/

class RelOp : public BoolExpr
{
    //Gli unici errori che gestisce questa classe "in autonomia" sulla costruzione degli oggetti sono le costruzioni di operatori sensati
public:
    //dichiaro i codici delle operazioni ammesse:
    enum relOpCode {
        LT, GT, EQ
    };

    //alcune funzioni utili:
    static std::string relOpCodeToString(relOpCode op) {
        switch (op) {
        case LT: return "LT";
        case GT: return "GT";
        case EQ: return "EQ";
        }
        throw GenericError("relOpCodeToString failed");
    }
    static relOpCode stringToRelOpCode(std::string s) {
        if (s == "LT") return LT;
        if (s == "GT") return GT;
        if (s == "EQ") return EQ;
        throw GenericError("stringToRelOpCode failed");
    }

    RelOp() = delete;
    RelOp(relOpCode o, NumExpr* o1, NumExpr* o2) : operand{ o }, op1{ o1 }, op2{ o2 } {}

    void accept(Visitor* v) override {
        v->visitRelOp(this);
    }

    relOpCode getOperand() const {
        return operand;
    }
    NumExpr* getOp1() const {
        return op1;
    }
    NumExpr* getOp2() const {
        return op2;
    }

private:
    relOpCode operand;
    NumExpr* op1;
    NumExpr* op2;
};

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//BoolConst
/*La classe che rappresenta un valore costante booleano (TRUE o FALSE).*/

class BoolConst: public BoolExpr {
    //Permette solo la creazione di oggetti 
public:
    //valori assumibili:
    enum boolValue {
        TRUE, FALSE
    };

    //funzioni utili:
    static std::string boolValueToString(boolValue b) {//per convertire gli enum in stringhe
        if (b == TRUE) return "TRUE";
        return "FALSE";
    }
    static int boolValueToInt(boolValue b) {//per convertire gli enum a valori interi (1=vero/0=falso)
        if (b == TRUE) return 1;
        return 0;
    }

    BoolConst() = delete;
    BoolConst(std::string val) {//costruttore che costruisce a partire da stringhe la BoolConst
        if (val == "TRUE" || val == "true" || val == "1") { value = BoolConst::TRUE; }
        else if (val == "FALSE" || val == "false" || val == "0") { value = BoolConst::FALSE; }
        else throw ParseError("Error in boolean value assignment");
    }

    boolValue getValue() {
        return value;
    }
    void accept(Visitor* v) override {
        v->visitBoolConst(this);
    }

private:
    boolValue value;
};
#endif