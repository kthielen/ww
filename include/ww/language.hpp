
/*
 * language : coordinate the basic parts of parser generation and application
 */

#ifndef WW_LANGUAGE_HPP_INCLUDED
#define WW_LANGUAGE_HPP_INCLUDED

#include <ww/lalr.hpp>
#include <ww/lrparser.hpp>
#include <ww/stream.hpp>
#include <ww/data.hpp>

#include <ww/type.hpp>
#include <ww/expr.hpp>

namespace ww {

template <typename T>
	struct primitiveEvaluator {
		typedef shared_ptr< primitiveEvaluator<T> > ptr;
    virtual ~primitiveEvaluator() { }

		virtual T eval(terminal*,nat) = 0;
	};

template <typename T>
	struct ruleEvaluator {
		typedef shared_ptr< ruleEvaluator<T> > ptr;
    virtual ~ruleEvaluator() { }

		virtual T eval(const std::vector<T>&,const nats&,const nats&) = 0;
	};

template <typename T>
	class languageEvaluator : public reducer<T> {
	public:
		typedef typename primitiveEvaluator<T>::ptr primcb;
		typedef typename ruleEvaluator<T>::ptr rulecb;

		void setDefPrimCB(const primcb& cb) {
			this->defpcb = cb;
		}

		void addPrimCB(terminal* t, const primcb& cb) {
			this->pcbs[t] = cb;
		}

		void addRuleCB(terminal* t, nat r, const rulecb& cb) {
			this->rcbs[ruleid(t, r)] = cb;
		}
	
		// reducer interface
		T primitive(terminal* t, nat index) {
			typename primcbs::const_iterator pcb = this->pcbs.find(t->token());
			if (pcb != this->pcbs.end()) {
				return pcb->second->eval(t, index);
			} else if (this->defpcb.get() != 0) {
				return this->defpcb->eval(t, index);
			} else {
				std::ostringstream ss;
				t->show(ss);
				throw std::runtime_error("Unexpected primitive token: " + ss.str());
			}
		}

		T reduce(terminal* s, nat r, const std::vector<T>& cs, const nats& pis, const nats& pfs) {
			typename rulecbs::const_iterator rcb = this->rcbs.find(ruleid(s->token(), r));
			if (rcb != this->rcbs.end()) {
				return rcb->second->eval(cs, pis, pfs);
			} else {
				std::ostringstream ss;
				s->show(ss); ss << "@" << r;
				throw std::runtime_error("Unexpected reduction rule: " + ss.str());
			}
		}
	private:
		typedef std::pair<terminal*, nat>    ruleid;
		typedef std::map<ruleid,     rulecb> rulecbs;
		typedef std::map<terminal*,  primcb> primcbs;
		
		primcb  defpcb;
		primcbs pcbs;
		rulecbs rcbs;
	};

class symbolset {
public:
	symbolset() : cm(new charmap()), sm(new symmap()) { }

	terminal* c(char               x) { return this->cm->apply(x); }
	terminal* s(const std::string& x) { return this->sm->apply(x); }
	
	terminals cs(const std::string& x) {
		terminals r;
		for (std::string::const_iterator i = x.begin(); i != x.end(); ++i) {
			r.push_back(c(*i));
		}
		return r;
	}

	terminals symbols(const std::vector<std::string>& syms) {
		terminals result;
		for (std::vector<std::string>::const_iterator sym = syms.begin(); sym != syms.end(); ++sym) {
			result.push_back(s(*sym));
		}
		return result;
	}

	terminal* operator()(char x)               { return c(x); }
	terminal* operator()(const char* x)        { return s(x); }
	terminal* operator()(const std::string& x) { return s(x); }

	terminals operator()(const std::vector<char>& xs) { return cs(std::string(xs.begin(), xs.end())); }

	stream<terminal*>::ptr chars(const stream<char>::ptr& x, const linedb::ptr& ldb) {
		return eofTerminated(stream<terminal*>::ptr(new map<char, terminal*>(this->cm, track(x, ldb))));
	}

	stream<terminal*>::ptr chars(const std::string& x, const linedb::ptr& ldb) {
		return chars(stream<char>::ptr(new str<char>(x)), ldb);
	}
private:
	charmap::ptr cm;
	symmap::ptr  sm;

	stream<terminal*>::ptr eofTerminated(const stream<terminal*>::ptr& s) {
		return stream<terminal*>::ptr(new withEOF(s));
	}

	stream<char>::ptr track(const stream<char>::ptr& s, const linedb::ptr& ldb) {
		return stream<char>::ptr(new trackpos(s, ldb));
	}
};

class rb {
public:
	rb(symbolset& ss) : ss(ss) { }

	rb& c(char x) {
		this->r.push_back(this->ss.c(x));
		return *this;
	}

	rb& cs(const std::string& x) {
		terminals ts = this->ss.cs(x);
		this->r.insert(this->r.end(), ts.begin(), ts.end());
		return *this;
	}

	rb& s(const std::string& x) {
		this->r.push_back(this->ss.s(x));
		return *this;
	}

	rb& x(terminal* t) {
		this->r.push_back(t);
		return *this;
	}

	const rule& build() const {
		return this->r;
	}
private:
	symbolset& ss;
	rule       r;
};

// clean up allocated tokens
class tokenbin : public function<terminal*, terminal*> {
public:
	~tokenbin() {
		for (terminalset::const_iterator t = ts.begin(); t != ts.end(); ++t) {
			delete *t;
		}
	}

	terminal* apply(terminal* t) {
		ts.insert(t);
		return t;
	}
private:
	terminalset ts;
};

// utility terminal types
class lexterm : public terminal {
public:
	lexterm(terminal* t, const linedb::ptr& ldb, nat i, nat f) : t(t), ldb(ldb), i(i), f(f) {
	}

	lexterm(terminal* t, const terminals& toks) : t(t) {
		if (toks.size() == 0) {
			throw std::runtime_error("Internal error, can't deduce lexical structure from empty token list");
		}
		const lexterm* t0 = dynamic_cast<const lexterm*>(toks[0]);
		const lexterm* t1 = dynamic_cast<const lexterm*>(toks[toks.size()-1]);
		if (!t0) {
			throw std::runtime_error("Internal error, can't deduce lexical structure (0) from non-lexterm: " + ::ww::show(toks[0]));
		}
		if (!t1) {
			throw std::runtime_error("Internal error, can't deduce lexical structure (1) from non-lexterm: " + ::ww::show(toks[toks.size()-1]));
		}
		this->ldb = t0->ldb;
		this->i   = t0->i;
		this->f   = t1->f;
	}

	const linedb::ptr& lineDB()     const { return this->ldb; }
	nat                initialPos() const { return this->i; }
	nat                finalPos()   const { return this->f; }

	void initialPos(nat x) { this->i = x; }
	void finalPos  (nat x) { this->f = x; }

	void show(std::ostream& out) const {
		this->t->show(out);
	}

	terminal* token() {
		return this->t;
	}
private:
	terminal*   t;
	linedb::ptr ldb;
	nat         i;
	nat         f;
};

template <typename T>
	class lexwrap : public lexterm {
	public:
		lexwrap(terminal* t, const linedb::ptr& ldb, nat i, nat f, T x) : lexterm(t, ldb, i, f), x(x) {
		}

		lexwrap(terminal* t, const terminals& toks, T x) : lexterm(t, toks), x(x) {
		}

		T value() const {
			return this->x;
		}

		T& ref() {
			return this->x;
		}

		void value(T y) {
			this->x = y;
		}
	private:
		T x;
	};

template <>
	class lexwrap<void> : public lexterm {
	public:
		lexwrap(terminal* t, const linedb::ptr& ldb, nat i, nat f) : lexterm(t, ldb, i, f) {
		}

		lexwrap(terminal* t, const terminals& toks) : lexterm(t, toks) {
		}
	};

// utility reduction functions
template <typename T>
	class ConstR : public ruleEvaluator<T> {
	public:
		ConstR(T x) : x(x) { }
		T eval(const std::vector<T>& cs, const nats& pis, const nats& pfs) { return this->x; }
	private:
		T x;
	};

template <typename T>
	class ConstP : public primitiveEvaluator<T> {
	public:
		ConstP(T x) : x(x) { }
		T eval(terminal* t, nat index) { return this->x; }
	private:
		T x;
	};

template <typename T>
	inline typename ruleEvaluator<T>::ptr constR(T x) {
		return newp< ConstR<T> >(x);
	}

inline ruleEvaluator<terminal*>::ptr nilR() {
	return constR<terminal*>((terminal*)0);
}

template <typename T, int n>
	struct SelectR : public ruleEvaluator<T> {
		T eval(const std::vector<T>& cs, const nats& pis, const nats& pfs) { return cs[n]; }
	};

template <typename T>
	struct UnwrapP : public primitiveEvaluator<T> {
		T eval(terminal* t, nat index) {
			return ((lexwrap<T>*)t)->value();
		}
	};

struct IdP : public primitiveEvaluator<terminal*> {
	terminal* eval(terminal* t, nat) { return t; }
};

template <typename T, T (*shift)(T,T)>
	class accTTok : public ruleEvaluator<terminal*> {
	public:
		accTTok(terminal* p, const linedb::ptr& ldb, tokenbin& tb) : p(p), ldb(ldb), tb(tb) {
		}

		terminal* eval(const terminals& cs, const nats& pis, const nats& pfs) {
			if (cs.size() == 1) {
				return tb.apply(new lexwrap<T>(this->p, this->ldb, pis[0], pfs[0], ((lexwrap<T>*)cs[0])->value()));
			} else {
				lexwrap<T>* left  = (lexwrap<T>*)cs[0];
				lexwrap<T>* right = (lexwrap<T>*)cs[1];

				left->value(shift(left->value(), right->value()));
				left->finalPos(pfs[1]);
				return left;
			}
		}
	private:
		terminal*   p;
		linedb::ptr ldb;
		tokenbin&   tb;
	};


template <typename T, T (*fromChar)(char)>
	class charTok : public ruleEvaluator<terminal*> {
	public:
		charTok(terminal* p, const linedb::ptr& ldb, tokenbin& tb) : p(p), ldb(ldb), tb(tb) {
		}

		terminal* eval(const terminals& cs, const nats& pis, const nats& pfs) {
			return tb.apply(new lexwrap<T>(this->p, this->ldb, pis[0], pfs[0], fromChar(((character*)cs[0])->value())));
		}
	private:
		terminal*   p;
		linedb::ptr ldb;
		tokenbin&   tb;
	};

// error reporting
inline std::string showTerminals(const terminals& ts) {
	std::ostringstream ss;
	ss << "[";
	if (ts.size() > 0) {
		ts[0]->show(ss);
		for (unsigned int i = 1; i < ts.size(); ++i) {
			ss << ", ";
			ts[i]->show(ss);
		}
	}
	ss << "]";
	return ss.str();
}

void printParseError(std::ostream& out, const linedb::ptr& ldb, const parsefailp& pf);

// a generic language
template <typename T>
	class language {
	public:
		typedef shared_ptr< lalrparser<T> > parserp;
		typedef languageEvaluator<T>        eval;
		typedef shared_ptr< eval >          evalp;

		language(terminal* s) : s(s), ev(new eval()) {
		}

		parserp compile(const precedence& px = precedence()) {
			return parserp(new lalrparser<T>(this->g, this->s, this->ev, px));
		}

		void asAllOf(terminal* s, const rule& r, const typename eval::rulecb& cb) {
			this->ev->addRuleCB(s, this->g[s].size(), cb);
			this->g[s].push_back(r);
		}

		void asOneOf(terminal* s, const terminals& ts, const typename eval::rulecb& cb) {
			for (terminals::const_iterator t = ts.begin(); t != ts.end(); ++t) {
				asAllOf(s, list(*t), cb);
			}
		}

		void translatePrimitive(terminal* p, const typename eval::primcb& cb) {
			this->ev->addPrimCB(p, cb);
		}

		void translatePrimitive(const typename eval::primcb& cb) {
			this->ev->setDefPrimCB(cb);
		}
	private:
		terminal* s;
		grammar   g;
		evalp     ev;
	};

// a lexer
class lexer {
public:
	typedef stream<terminal*>::ptr           S;
	typedef buffering_stream<terminal*>::ptr BS;
	typedef languageEvaluator<terminal*>     eval;
	typedef shared_ptr< eval >               evalp;

	typedef parse_any<terminal*> lparser;
	typedef lparser::seq         lpseq;

	lexer() : ev(new eval()), lps(new lparser::seqv()) {
		// by default, let terminals evaluate to themselves
		translatePrimitive(newp<IdP>());
	}

	S compile(const BS& in) {
		terminalset ts = topLevelSymbols(g);

		this->lps->clear();
		for (terminalset::const_iterator t = ts.begin(); t != ts.end(); ++t) {
			this->lps->push_back(lparser::ptr(new lalrparser<terminal*>(this->g, *t, this->ev)));
		}
	
		return
			newp< lazyFilter<terminal*> >(
				newp< fnot<terminal*> >(newp< eq<terminal*> >((terminal*)0)),
				newp< longest_match_stream<terminal*> >(lparser::ptr(new parse_any<terminal*>(this->lps)), in),
				endOfFile::value()
			);
	}

	void pushLexer(terminal* s) {
		lparser::ptr p(new lalrparser<terminal*>(this->g, s, this->ev));

		this->lps->insert(this->lps->begin(), p);
	}

	void keyword(symbolset& ss, const std::string& sym) {
		asAllOf(ss.s(sym), rb(ss).cs(sym).build(), constR(ss.s(sym)));
	}

	void asAllOf(terminal* s, const rule& r, const eval::rulecb& cb) {
		this->ev->addRuleCB(s, this->g[s].size(), cb);
		this->g[s].push_back(r);
	}

	void asOneOf(terminal* s, const terminals& ts, const eval::rulecb& cb) {
		for (terminals::const_iterator t = ts.begin(); t != ts.end(); ++t) {
			asAllOf(s, list(*t), cb);
		}
	}

	void translatePrimitive(terminal* p, const eval::primcb& cb) {
		this->ev->addPrimCB(p, cb);
	}

	void translatePrimitive(const eval::primcb& cb) {
		this->ev->setDefPrimCB(cb);
	}
private:
	grammar   g;
	evalp     ev;
	lpseq     lps;
};

// all of the common state used in parsers
struct parser_state {
	typedef language<terminal*> plang;

	parser_state(const char* s) : lang(ss(s)), tenv(new TypeEnv()), venv(new ValueEnv()) {
		// add global axioms
		tenv->define("null", liftty<int>()); // uh ...
		venv->define("null", 0);
	}

	symbolset      ss;
	linedb::ptr    ldb;
	tokenbin       tb;

	lexer          lex;
	plang          lang;
	plang::parserp parser;
    precedence     prec;
	plang::parserp lastp;

	// manage global type/value info for lexeme, rule, and expression types
	TypeEnvPtr  tenv;
	ValueEnvPtr venv;
};

typedef shared_ptr<parser_state> parser_state_ptr;

}

#endif

