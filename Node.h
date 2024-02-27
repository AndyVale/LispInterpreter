#ifndef NODE_H
#define NODE_H
#include <stack>
#include <vector>
#include "Token.h"
#include "Visitor.h"

class Visitor;
//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//Node
/*	Classe astratta che fa da superclasse a tutte quelle classi (o le loro figlie)
	che devono accettare i visitor.
*/
struct Node {
public:
	Node() = default;
	virtual void accept(Visitor*) = 0;//tutte le sottoclassi devono implementare questo metodo per essere classi concrete

private:
};

//		-		-		-		-		////		-		-		-		-		////		-		-		-		-		////		-		-		-		-		//
//StmtBlock
/*	Classe astratta che rappresenta "statment_block" della grammatica.
	Verrà implementato dalle classi concrete figlie di Statment e Block.
	Introduce un attributo vector "figli" che permette la gestione dell'albero.
*/

class StmtBlock : public Node {
public:
	std::vector<Node*>& getFigli() {
		return figli;
	}
	void aggiungiFiglio(Node* tmp) {
		getFigli().push_back(tmp);
	}

private:
	std::vector<Node*> figli;//deve essere privato perchè voglio che sia modificabile solo da "aggiungiFiglio"
};

#endif
