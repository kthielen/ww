
#include <ww/lrparser.hpp>

namespace ww {

action::action() {
}

action action::goTo(nat s) {
	action a;
	a.act = 0;
	a.d.s = s;
	return a;
}
bool action::isGoTo()    const { return this->act == 0; }
nat  action::goToState() const { return this->d.s; }

action action::shift(nat s) {
	action a;
	a.act = 1;
	a.d.s = s;
	return a;
}
bool action::isShift()    const { return this->act == 1; }
nat  action::shiftState() const { return this->d.s; }

action action::reduce(terminal* x, nat r, nat n) {
	action a;
	a.act = 2;
	a.d.x = x;
	a.d.r = r;
	a.d.n = n;
	return a;
}
bool      action::isReduce()   const { return this->act == 2; }
terminal* action::reduceSym()  const { return this->d.x; }
nat       action::reduceRule() const { return this->d.r; }
nat       action::reduceSize() const { return this->d.n; }

action action::accept() {
	action a;
	a.act = 3;
	return a;
}
bool action::isAccept() const { return this->act == 3; }

std::ostream& action::fmt(std::ostream& out) const {
	switch (this->act) {
	case 0: out << this->d.s; break;
	case 1: out << "s" << this->d.s; break;
	case 2: out << "r" << this->d.r; break;
	case 3: out << "A"; break;
	}
	return out;
}

std::ostream& operator<<(std::ostream& out, const action& act) {
	return act.fmt(out);
}

bool action::operator==(const action& rhs) const {
	if (this->act != rhs.act) {
		return false;
	} else {
		switch (this->act) {
		default:
		case 0: return rhs.d.s == this->d.s;
		case 1: return rhs.d.s == this->d.s;
		case 2: return rhs.d.x == this->d.x && rhs.d.r == this->d.r && rhs.d.n == this->d.n;
		case 3: return true;
		}
	}
}

bool action::operator!=(const action& rhs) const {
	return !(*this == rhs);
}

}

