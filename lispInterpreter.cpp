#include <iostream>
#include <fstream>
#include "Tokenizer.h"
#include "Parser.h"
#include "Visitor.h"

int main(int argc, char** argv) {
    //FASE DI TOKENIZZAZIONE
     if (argc == 1) {
        std::cerr << "Specificare il nome del file da interpretare" << std::endl;
        std::cout << std::endl;
        return EXIT_FAILURE;
    }
    const std::string fileName(argv[1]);
    //std::cout << "Esecuzione di: " << argv[1] << std::endl;
    std::ifstream input;

    try {//tento di aprire il file
        input.open(fileName, std::ios::in);
        if (!input) throw std::exception();//lancio forzatamente un'eccezione
    }
    catch (std::exception&) {
        std::cout << "Errore, non riesco ad aprire il file: " << fileName << std::endl;
        std::cout << std::endl;
        return EXIT_FAILURE;
    }

    Tokenizer t = Tokenizer();
    std::vector<Token> tokens;
    try {//tokenizzo il file e lo chiudo
        tokens = std::move(t(input));
        input.close();
    }
    catch (std::runtime_error e) {
        std::cerr << e.what() << std::endl;
        std::cout << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception& e) {
        std::cerr << "Eccezione non gestita: " << e.what();
        std::cout << std::endl;
        return EXIT_FAILURE;
    }

    
    try {
        //FASE DI PARSING:
        NodeManager m;//creo un manager
        Parser parse(m);//creo il parser fornendo come manager da utilizzare quello appena creato

        StmtBlock* root = parse(tokens);//parso i token creando l'albero sintattico (solo se rispetta la grammatica, altrimenti vengono lanciate eccezioni)

        //FASE DI VALUTAZIONE:
        //std::cout << std::endl << std::endl << "Quello che stampa il programma: " << std::endl << std::endl;
        //Eseguo il programma accettando il visitor di valutazione sulla radice dell'albero sintattico:
        EvaluationVisitor* ev =new EvaluationVisitor();//creo un oggetto EvaluationVisitor per esegurie il codice a partire dall'albero
        root->accept(ev);//(se vengono riscontrati errori in fase di esecuzione vengono lanciate eccezioni)
        delete ev; //cancello gli oggetti allocati dinamicamente
        
        //Stampa (in LISP) del programma letto:
        
        //std::cout << "Il programma letto: " << std::endl << std::endl;
        //PrintVisitor* pv = new PrintVisitor();//creo un oggetto PrintVisitor per stampare l'albero
        //root->accept(pv);//faccio accettare la visita alla radice dell'albero 
        //delete pv; 
        
        //Stampa (in C++) del programma letto:
        
        //std::cout << std::endl << "Il programma letto: " << std::endl << std::endl;
        //PrintCLikeVisitor* pc = new PrintCLikeVisitor();//creo un oggetto PrintVisitor per stampare l'albero
        //pc->stamp(root);
        //delete pc;
    }
    catch (std::exception e) {
        std::cerr << e.what();
        std::cout << std::endl;
        return EXIT_FAILURE;
    }
}
