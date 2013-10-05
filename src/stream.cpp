
#include <ww/stream.hpp>

namespace ww {

// a char stream supporting line lookups
trackpos::trackpos(const S& s, const LDB& ldb) : s(s), ldb(ldb) {
}

const trackpos::LDB& trackpos::lineDB() const {
	return this->ldb;
}

char trackpos::next() {
	char x = this->s->next();
	this->ldb->step(x);
	return x;
}

bool trackpos::end() const {
	return this->s->end();
}

}

