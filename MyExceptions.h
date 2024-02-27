#ifndef MYEXCEPTIONS_H
#define MYEXCEPTIONS_H
#include <stdexcept>
#include <sstream>

//dichiaro i parametri della classe sotto (costanti al tempo di compilazione altrimenti non possono essere parametri di un template)
static constexpr const char g[] = "ERRORE GENERICO";
static constexpr const char l[] = "ERRORE LESSICALE";
static constexpr const char p[] = "ERRORE SINTATTICO";
static constexpr const char v[] = "ERRORE IN FASE DI VALUTAZIONE";

template <const char* param>//utilizzo un tamplate per non dover scrivere 4 classi uguali
struct myExc : std::runtime_error {
	myExc(std::string s) : std::runtime_error(addPrefix(s).c_str()) { }//richiamo il costruttore della classe base
	std::string addPrefix(std::string s) {//funzione che aggiunge all'argomento dell'eccezione la stringa param (verrà passata al costruttore base di runtime_error)
		std::stringstream supp;
		supp << param <<": " << s;
		return supp.str();
	}
};

//Dichiaro gli alias che utilizzerò nel codice per non dover utilizzare la notazione "template":
using GenericError = myExc<g>;
using LexicalError = myExc<l>;
using ParseError = myExc<p>;
using EvaluationError = myExc<v>;

#endif