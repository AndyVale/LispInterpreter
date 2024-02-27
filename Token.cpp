#include <sstream>
#include "Token.h"
std::ostream& operator<<(std::ostream& os, const Token& t) {
	std::stringstream supp;
	supp <<"\"" << t.word << "\" (alla riga " << t.coordinate.first << " il token n'" << t.coordinate.second <<")";
	os << supp.str();
	return os;
}
