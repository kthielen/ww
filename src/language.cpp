
#include <ww/language.hpp>

namespace ww {

/*
 * print the 'reduction exception' error
 */
void printGenErrorDesc(std::ostream& out, const linedb::ptr& ldb, const reduce_exception* pf) {
	out << "While reducing '" << show(pf->reduceFor()) << "': " << pf->errorMsg() << std::endl;
}

/*
 * print the 'unexpected_terminal' error
 */
void printGenErrorDesc(std::ostream& out, const linedb::ptr& ldb, const unexpected_terminal* pf) {
	out << "Unexpected '" << show(pf->received()) << "' encountered in input";

	terminals ets = pf->expected();
	if (ets.size() == 1) {
		out << " (expected '" << show(ets[0]) << "')" << std::endl;
	} else {
		out << " (expected one of " << showTerminals(ets) << ")" << std::endl;
	}
}

/*
 * print highlighted source lines to show where errors came from
 */
void highlightLine(std::ostream& out, const std::string& line, nat hi, nat hf) {
	hi -= 1; hf -= 1;
	out << line.substr(0, hi);

	char bgccmd[] = { 27, '[', '4', '1', 'm' };
	char fgccmd[] = { 27, '[', '3', '7', 'm' };
	char rstcmd[] = { 27, '[', '0',  'm' };

	out.write(bgccmd, sizeof(bgccmd));
	out.write(fgccmd, sizeof(fgccmd));

	out << line.substr(hi, hf - hi);
	out.write(rstcmd, sizeof(rstcmd));

	out << line.substr(hf, line.size() - hf);
}

void printLocErrorDesc(std::ostream& out, const ldblines& lines, const linedb::LineCol& i, const linedb::LineCol& f) {
	for (nat ln = 0; ln < lines.first.size(); ++ln) {
		nat                line = lines.first[ln];
		const std::string& txt  = lines.second[ln];

		if (line < i.first || line > f.first) {
			out << txt;
		} else if (i.first == f.first) {
			highlightLine(out, txt, i.second, f.second);
		} else if (line == i.first) {
			highlightLine(out, txt, i.second, txt.size());
		} else if (line == f.first) {
			highlightLine(out, txt, 0, f.second);
		} else {
			highlightLine(out, txt, 0, txt.size());
		}

		out << std::endl;
	}
}

/*
 * generic error type dispatch
 */
struct printParseFailF : public switchParseError<Unit> {
	std::ostream& out;
	const linedb::ptr& ldb;
	printParseFailF(std::ostream& out, const linedb::ptr& ldb) : out(out), ldb(ldb) { }

	Unit with(const reduce_exception* v) const {
		printGenErrorDesc(this->out, this->ldb, v);

		const terminals& args = v->reduceArgs();

		if (lexterm* t1 = dynamic_cast<lexterm*>(args[args.size() - 1])) {
			linedb::LineCol i = t1->lineDB()->pos(t1->initialPos());
			linedb::LineCol f = t1->lineDB()->pos(t1->finalPos());
			
			printLocErrorDesc(this->out, load(this->ldb, i, f), i, f);
		}

		return unit;
	}

	Unit with(const unexpected_terminal* v) const {
		printGenErrorDesc(this->out, this->ldb, v);

		if (lexterm* t = dynamic_cast<lexterm*>(v->received())) {
			linedb::LineCol i = t->lineDB()->pos(t->initialPos());
			linedb::LineCol f = t->lineDB()->pos(t->finalPos());

			printLocErrorDesc(this->out, load(this->ldb, i, f), i, f);
		}
		return unit;
	}
};

void printParseError(std::ostream& out, const linedb::ptr& ldb, const parsefailp& pf) {
	switchOf(pf, printParseFailF(out, ldb));
}

}

