
#include <ww/boot.hpp>

#include <sstream>
#include <iostream>
#include <fstream>

#include <stdexcept>
#include <math.h>

using namespace ww;

// basic environment bindings
int toIntF(std::string x) {
	return fromString<int>(x);
}

int plusF(int x, int y) {
	std::cout << x << " + " << y << " = " << (x + y) << std::endl;
	return x + y;
}

int minusF(int x, int y) {
	std::cout << x << " - " << y << " = " << (x - y) << std::endl;
	return x - y;
}

int timesF(int x, int y) {
	std::cout << x << " * " << y << " = " << (x * y) << std::endl;
	return x * y;
}

int divideF(int x, int y) {
	std::cout << x << " / " << y << " = " << (x / y) << std::endl;
	return x / y;
}

int powerF(int x, int y) {
	std::cout << x << " ^ " << y << " = " << pow(x, y) << std::endl;
	return pow(x, y);
}

void declareAxioms(const parser_state_ptr& ps) {
	define(ps, "toInt",  reduceWith(&toIntF));
	define(ps, "plus",   reduceWith(&plusF));
	define(ps, "minus",  reduceWith(&minusF));
	define(ps, "times",  reduceWith(&timesF));
	define(ps, "divide", reduceWith(&divideF));
	define(ps, "power",  reduceWith(&powerF));
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

