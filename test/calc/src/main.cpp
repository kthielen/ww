
#include <ww/language.hpp>
#include <ww/evaluator.hpp>

#include <iostream>
#include <stdexcept>

using namespace ww;

// lexer reducers
int accN(int a, int d) {
	return a*10 + d;
}
int Nid(int x) { return x; }

int makeD(char c) {
	return c - '0';
}

// parser reducers
struct evalSum : public ruleEvaluator<int> {
	int eval(const std::vector<int>& xs, const nats& pis, const nats& pfs) {
		return xs[0] + xs[2];
	}
};

struct evalDifference : public ruleEvaluator<int> {
	int eval(const std::vector<int>& xs, const nats& pis, const nats& pfs) {
		return xs[0] - xs[2];
	}
};

struct evalProduct : public ruleEvaluator<int> {
	int eval(const std::vector<int>& xs, const nats& pis, const nats& pfs) {
		return xs[0] * xs[2];
	}
};

struct evalQuotient : public ruleEvaluator<int> {
	int eval(const std::vector<int>& xs, const nats& pis, const nats& pfs) {
		return xs[0] / xs[2];
	}
};

// a simple interpreter for arithmetic expressions
int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "calc <input-string>" << std::endl;
		return -1;
	}
	std::string input(argv[1]);

	symbolset   ss;
	linedb::ptr ldb(new linedb(0, input));
	tokenbin    tb;

	stream<terminal*>::ptr in = ss.chars(input, ldb);

	try {
		/*
		 * the calculator lexer
		 */

		lexer ll;

		// n -> d+
		ll.asAllOf(ss("n"), list(ss("n"), ss("d")), lexaction(ss("n"), ldb, tb, &accN));
		ll.asAllOf(ss("n"), list(ss("d")),          lexaction(ss("n"), ldb, tb, &Nid));

		// d -> '[0-9]'
		ll.asOneOf(ss("d"), ss.cs("0123456789"), lexaction(ss("d"), ldb, tb, &makeD));

		ll.keyword(ss, "+");
		ll.keyword(ss, "-");
		ll.keyword(ss, "*");
		ll.keyword(ss, "/");
		ll.keyword(ss, "(");
		ll.keyword(ss, ")");

		// ws -> w+
		ll.asAllOf(ss("ws"), list(ss("ws"), ss("w")), nilR());
		ll.asAllOf(ss("ws"), list(ss("w")),           nilR());

		// ws -> '[ \t\r\n]'
		ll.asOneOf(ss("w"), ss.cs(" \t\r\n"), nilR());

		// make the lexer (trimming out null terminals)
		stream<terminal*>::ptr lex = ll.compile(in);

		/*
		 * the calculator parser/evaluator
		 */

		language<int> lang(ss.s("s"));

		lang.asAllOf(ss.s("s"), rb(ss).s("e").build(), newp< SelectR<int, 0> >());

		lang.asAllOf(ss.s("e"), rb(ss).s("e").s("+").s("e").build(), newp<evalSum>());
		lang.asAllOf(ss.s("e"), rb(ss).s("e").s("-").s("e").build(), newp<evalDifference>());
		lang.asAllOf(ss.s("e"), rb(ss).s("e").s("*").s("e").build(), newp<evalProduct>());
		lang.asAllOf(ss.s("e"), rb(ss).s("e").s("/").s("e").build(), newp<evalQuotient>());
		lang.asAllOf(ss.s("e"), rb(ss).s("n").build(),               newp< SelectR<int, 0> >());
		lang.asAllOf(ss.s("e"), rb(ss).s("(").s("e").s(")").build(), newp< SelectR<int, 1> >());

		lang.translatePrimitive(ss.s("n"), newp< UnwrapP<int> >()); // pull the number out of number tokens
		lang.translatePrimitive(newp< ConstP<int> >(0));            // else we don't care about underlying values

		precedence px;
		px[ss.s("+")] = prec(0, assoc::left);
		px[ss.s("-")] = prec(0, assoc::left);
		px[ss.s("*")] = prec(1, assoc::left);
		px[ss.s("/")] = prec(1, assoc::left);

		language<int>::parserp p = lang.compile(px);

		/*
		 * the parser driver
		 */

		while (!lex->end()) {
			p->transition(lex->next());
	
			if (p->failed()) {
				printParseError(std::cout, ldb, p->why());
				break;
			} else if (p->accepts()) {
				std::cout << input << " = " << p->reduce() << std::endl;
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

