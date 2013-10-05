
#include <ww/terminal.hpp>

namespace ww {

// characters
character::character(char x) : x(x) { }
char character::value() const { return this->x; }
void character::value(char c) { this->x = c; }
terminal* character::token() { return this; }

void character::show(std::ostream& out) const {
	out << this->x;
}

// symbols
symbol::symbol(const std::string& sname) : sname(sname) { }
const std::string& symbol::name() const { return this->sname; }
terminal* symbol::token() { return this; }

void symbol::show(std::ostream& out) const {
	out << this->sname;
}

// eof
endOfFile::endOfFile() { }
terminal* endOfFile::token() { return this; }

void endOfFile::show(std::ostream& out) const {
	out << "$";
}

terminal* endOfFile::value() {
	static endOfFile e;
	return &e;
}

// streams with a terminating sentinal
withEOF::withEOF(const S& s) : s(s), gaveEof(false) {
}

terminal* withEOF::next() {
	if (this->s->end()) {
		this->gaveEof = true;
		return endOfFile::value();
	} else {
		return this->s->next();
	}
}

bool withEOF::end() const {
	return this->gaveEof;
}

}
