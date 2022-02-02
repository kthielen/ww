
#include <ww/boot.hpp>

#include <sstream>
#include <iostream>
#include <fstream>

#include <stdexcept>
#include <math.h>

using namespace ww;

struct E {
	virtual void show(std::ostream&) const = 0;
	virtual int value() const = 0;
};

class I : public E {
public:
	I(int x) : x(x) { }
	void show(std::ostream& out) const { out << this->x; }
	int value() const { return this->x; }
private:
	int x;
};

class A : public E {
public:
	A(const std::string& op, const E* left, const E* right) : op(op), left(left), right(right) { }
	void show(std::ostream& out) const { out << "("; left->show(out); out << this->op; right->show(out); out << ")"; }
	const std::string& opname() const { return this->op; }
	const E* arg0() const { return this->left; }
	const E* arg1() const { return this->right; }

	int value() const {
		int a0 = this->left->value();
		int a1 = this->right->value();

		if (this->op == "+") {
			return a0 + a1;
		} else if (this->op == "-") {
			return a0 - a1;
		} else if (this->op == "*") {
			return a0 * a1;
		} else if (this->op == "/") {
			return a0 / a1;
		} else if (this->op == "^") {
			return pow(a0, a1);
		} else {
			std::cout << "impossible" << std::endl;
			return -1;
		}
	}
private:
	std::string op;
	const E* left;
	const E* right;
};

// basic environment bindings
E* toIntF(std::string x) {
	return new I(fromString<int>(x));
}

E* plusF(E* x, E* y) {
	return new A("+", x, y);
}

E* minusF(E* x, E* y) {
	return new A("-", x, y);
}

E* timesF(E* x, E* y) {
	return new A("*", x, y);
}

E* divideF(E* x, E* y) {
	return new A("/", x, y);
}

E* powerF(E* x, E* y) {
	return new A("^", x, y);
}

void evalExp(E* e) {
	e->show(std::cout);
	std::cout << " = " << e->value() << std::endl;
}

void declareAxioms(const parser_state_ptr& ps) {
	define(ps, "toInt",  reduceWith(&toIntF));
	define(ps, "plus",   reduceWith(&plusF));
	define(ps, "minus",  reduceWith(&minusF));
	define(ps, "times",  reduceWith(&timesF));
	define(ps, "divide", reduceWith(&divideF));
	define(ps, "power",  reduceWith(&powerF));
	define(ps, "eval",   reduceWith(&evalExp));
}

std::string slurpFile(const std::string& fname) {
	std::ifstream in(fname.c_str());
	if (!in.is_open()) {
		std::cout << "couldn't open '" << fname << "' for reading" << std::endl;
		exit(-1);
	}

	std::ostringstream ss;
	ss << in.rdbuf();
	return ss.str();
}

// a dynamic parser generator
int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "pgen <input-file>" << std::endl;
		return -1;
	}
	std::string input = slurpFile(argv[1]);
	
	// begin a parser state
	parser_state_ptr ps(new parser_state("s"));

	// choose a char-sequence to parse
	ps->ldb = linedb::ptr(new linedb(0, input));
	buffering_stream<terminal*>::ptr in(new buffering_stream<terminal*>(ps->ss.chars(input, ps->ldb)));

	try {
		// load built-in symbols / reduction functions
		declareAxioms(ps);

		// define the bootstrap rules
		mergeBootstrapRules(ps);

		// build the lexer
		stream<terminal*>::ptr lex = ps->lex.compile(in);

		// the parser driver
		nat c = 0;
		while (!lex->end()) {
			ps->parser->transition(c++, lex->next());

			if (ps->parser->failed()) {
				printParseError(std::cout, ps->ldb, ps->parser->why());
				break;
			} else if (ps->parser->accepts()) {
				std::cout << "accepted." << std::endl;
				break;
			}
		}
	} catch (compile_table_failure& fail) {
		std::cout << fail.what() << std::endl;
		fail.print(std::cout);
	} catch (std::exception& ex) {
		std::cout << "** " << ex.what() << std::endl;
	}

	return 0;
}

