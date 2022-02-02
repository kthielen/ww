#ifndef WW_TERMINAL_HPP_INCLUDED
#define WW_TERMINAL_HPP_INCLUDED

#include <ww/stream.hpp>

#include <iostream>
#include <vector>
#include <map>

namespace ww {

/*
 * standard terminals
 */

// a "terminal" is an opaque value, assumed primitive
struct terminal {
	virtual ~terminal() { }

	// format this terminal for output on a text console
	virtual void show(std::ostream&) const = 0;

	// give the 'representative terminal' for this terminal
	//  (e.g.: X('abcd') and X('defg') have different content, but have equal representatives)
	virtual terminal* token() = 0;
};
typedef std::vector<terminal*> terminals;

// characters can be terminals
class character : public terminal {
public:
	character(char x = 0);
	char value() const;
	void value(char);
	void show(std::ostream&) const;
	terminal* token();
private:
	char x;
};

// a named symbol, distinct from other terminals
class symbol : public terminal {
public:
	symbol(const std::string& sname);

	const std::string& name() const;

	void show(std::ostream& out) const;
	terminal* token();
private:
	std::string sname;
};

// EOF is a special terminal
class endOfFile : public terminal {
public:
	endOfFile();
	void show(std::ostream&) const;
	terminal* token();

	static terminal* value();
};

// a terminal wrapper for a matched value
template <typename T>
	class wrapper : public terminal {
	public:
		wrapper(terminal* t, T x) : t(t), x(x) {
		}

		void show(std::ostream& out) const {
			if (this->t) {
				this->t->show(out);
			} else {
				out << "nil";
			}
		}

		terminal* token() {
			return this->t;
		}

		const T& value() const {
			return this->x;
		}

		T& value() {
			return this->x;
		}

		T& ref() {
			return this->x;
		}

		void value(T y) {
			this->x = y;
		}
	private:
		terminal* t;
		T         x;
	};

/*
 * terminal facades for primitive value types
 */
class charmap : public function<char, terminal*> {
public:
	charmap() {
		for (nat i = 0; i < 256; ++i) {
			this->cs[i].value((char)i);
		}
	}

	terminal* apply(char c) {
		return &this->cs[(unsigned char)c];
	}
private:
	character cs[256];
};

/*
 * maintain a set of distinct named terminals (such that f(x)==f(y) <-> x==y)
 */
class symmap : public function<const std::string&, terminal*> {
public:
	symmap() {
	}
	~symmap() {
		for (sym_set::const_iterator s = this->syms.begin(); s != this->syms.end(); ++s) {
			delete s->second;
		}
	}

	terminal* apply(const std::string& x) {
		sym_set::const_iterator s = this->syms.find(x);
		if (s != this->syms.end()) {
			return s->second;
		} else {
			terminal* r = new symbol(x);
			this->syms[x] = r;
			return r;
		}
	}
private:
	typedef std::map<std::string, terminal*> sym_set;
	sym_set syms;
};

/*
 * streams for standard terminals
 */

// a string made to end with the distinguished EOF 'terminal'
class withEOF : public stream<terminal*> {
public:
	typedef stream<terminal*>::ptr S;

	withEOF(const S& s);
	terminal* next();
	bool end() const;
private:
	S    s;
	bool gaveEof;
};

}

#endif
