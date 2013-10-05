/*
 * lrparser : Make a parser interface over an LR parser
 */

#ifndef WW_LRPARSER_HPP_INCLUDED
#define WW_LRPARSER_HPP_INCLUDED

#include <ww/parser.hpp>
#include <ww/terminal.hpp>
#include <ww/data.hpp>
#include <map>
#include <vector>

namespace ww {

// a few different types of errors can happen while parsing
struct parse_error {
	virtual std::string show() const = 0;
};

// an exception was raised during a reduction
class reduce_exception : public parse_error {
public:
	reduce_exception(terminal* s, const terminals& args, nat pi, nat pf, const std::string& msg) : s(s), args(args), pi(pi), pf(pf), msg(msg) {
	}
	~reduce_exception() throw() { }

	terminal*          reduceFor()  const { return this->s; }
	const terminals&   reduceArgs() const { return this->args; }
	nat                initialPos() const { return this->pi; }
	nat                finalPos()   const { return this->pf; }
	const std::string& errorMsg()   const { return this->msg; }

	std::string show() const { return this->msg; }
private:
	terminal*   s;
	terminals   args;
	nat         pi, pf;
	std::string msg;
};

// we received a token that we weren't looking for
class unexpected_terminal : public parse_error {
public:
	unexpected_terminal(nat s, terminal* r, const terminals& e) : s(s), r(r), e(e) {
		setDefaultDesc();
	}
	~unexpected_terminal() throw() {
	}

	terminal*        received() const { return this->r; }
	const terminals& expected() const { return this->e; }

	std::string show() const { return this->desc; }
private:
	nat         s;
	terminal*   r;
	terminals   e;
	std::string desc;

	void setDefaultDesc() {
		std::ostringstream ss;
		ss << "Unexpected '" << ::ww::show(this->r) << "' encountered in input.  Expecting one of [";
		if (this->e.size() > 0) {
			ss << ::ww::show(this->e[0]);
			for (unsigned int i = 1; i < this->e.size(); ++i) {
				ss << ", ";
				ss << ::ww::show(this->e[i]);
			}
		}
		ss << "]";
		this->desc = ss.str();
	}
};
typedef shared_ptr<parse_error> parsefailp;

template <typename T>
	struct switchParseError {
		virtual T with(const reduce_exception*)    const = 0;
		virtual T with(const unexpected_terminal*) const = 0;
	};

template <typename T>
	T switchOf(const parse_error& e, const switchParseError<T>& f) {
		if (const reduce_exception* v = dynamic_cast<const reduce_exception*>(&e)) {
			return f.with(v);
		} else if (const unexpected_terminal* v = dynamic_cast<const unexpected_terminal*>(&e)) {
			return f.with(v);
		} else {
			throw std::runtime_error("Internal error, unexpected parse error type");
		}
	}

template <typename T> T switchOf(const parsefailp& p, const switchParseError<T>& f) { return switchOf<T>(*p, f); }

// an LR parser action (shift, reduce, or accept)
class action {
public:
	// go to state s
	static action goTo(nat s);

	// shift to state s
	static action shift(nat s);

	// for symbol x, rule r, reduce n states
	static action reduce(terminal* x, nat r, nat n);

	// the input has been accepted
	static action accept();

	// what type of action is this?
	bool isGoTo()    const;
	nat  goToState() const;

	bool isShift()    const;
	nat  shiftState() const;

	bool      isReduce()   const;
	terminal* reduceSym()  const;
	nat       reduceRule() const;
	nat       reduceSize() const;

	bool isAccept() const;

	// basic ops
	bool operator==(const action& rhs) const;
	bool operator!=(const action& rhs) const;

	std::ostream& fmt(std::ostream& out) const;
private:
	uint8_t act;
	union {
		// for shift
		nat s;

		// for reduce
		struct {
			terminal* x;
			nat       r;
			nat       n;
		};
	} d;

	action();
};

std::ostream& operator<<(std::ostream& out, const action& act);

// a parser state maps a set of terminals to actions
typedef std::map<terminal*, action> lrstate;

// an ordered sequence of states describes the whole behavior of an LR parser
typedef std::vector<lrstate> lrstate_table;

// a user-function to apply to reduce steps
template <typename T>
	struct reducer {
		typedef shared_ptr< reducer<T> > ptr;
		typedef std::vector<T>           Ts;

		virtual T primitive(terminal*,nat) = 0;
		virtual T reduce(terminal*,nat,const Ts&,const nats&,const nats&) = 0;
	};

// finally, an LR parser just interprets an LR parse table on a sequence of terminals it's given
template <typename T>
	class lrparser : public parser<T> {
	public:
		typedef typename reducer<T>::ptr R;

		lrparser() : table(), failure(), inAccept(false), r() {
		}

		lrparser(const lrstate_table& table, const R& r) : table(table), failure(), inAccept(false), r(r) {
			this->states.push(0);
		}

		void reset(const lrstate_table& table, const R& r) {
			this->table = table;
			this->r     = r;

			reset();
		}

		void reset() {
			this->states = std::stack<nat>();
			this->states.push(0);
			this->rv = std::stack<T>();

			this->failure  = parsefailp();
			this->inAccept = false;
		}

		bool expects(terminal* t) const {
			if (t == 0 || failed()) {
				return false;
			} else {
				const lrstate& st = this->table[this->states.top()];
				return st.find(t->token()) != st.end();
			}
		}

		terminals expecting() const {
			terminals result;
			const lrstate& st = this->table[this->states.top()];
			for (lrstate::const_iterator a = st.begin(); a != st.end(); ++a) {
				result.push_back(a->first);
			}
			return result;
		}

		void transition(nat p, terminal* t) {
			while (terminal* nt = consume(p, t)) {
				consume(p, nt);
			}
		}

		bool failed() const {
			return this->failure.get() != 0;
		}

		parsefailp why() const {
			return this->failure;
		}

		bool accepts() const {
			return this->inAccept;
		}

		T reduce() {
			if (!accepts() || this->rv.size() != 1) {
				throw std::runtime_error("Internal error, reduce invoked for invalid state");
			} else {
				T r = this->rv.top();
				this->rv.pop();
				return r;
			}
		}
	private:
		typedef std::stack<nat> statestack;
		typedef std::stack<nat> posstack;
		typedef std::stack<T>   Ts;
		typedef std::vector<T>  Tseq;

		lrstate_table table;           // the compiled parser logic
		statestack    states;          // states pending completion
		posstack      initps;          // initial positions of parsed values
		posstack      finps;           // final positions of parsed values
		parsefailp    failure;         // information on a parse failure (or null if there's been no failure)
		bool          inAccept;        // are we in a completion state?
		R             r;               // the procedure for reducing parse sequences
		Ts            rv;              // accumulated result values

		void goTo(const action& a) {
			this->states.push(a.goToState());
		}

		void shift(nat p, terminal* t, const action& a) {
			this->rv.push(this->r->primitive(t, p));
			this->initps.push(p);
			this->finps.push(p + 1);

			this->states.push(a.shiftState());
		}

		void reduce(nat p, const action& a) {
			Tseq ts  = popv(this->rv,     a.reduceSize());
			nats pis = popv(this->initps, a.reduceSize());
			nats pfs = popv(this->finps,  a.reduceSize());

			if (a.reduceSize() == 0) {
				pis = list(p);
				pfs = pis;
			}

			try {
				this->rv.push(this->r->reduce(a.reduceSym(), a.reduceRule(), ts, pis, pfs));
				this->initps.push(pis[0]);
				this->finps.push(pfs[pfs.size()-1]);

				pop(this->states, a.reduceSize());
			} catch (std::exception& e) {
				this->failure = reduceException(a.reduceSym(), ts, pis[0], pfs[pfs.size()-1], e.what());
			}
		}

		// use the input terminal to perform an LR action
		// if the LR action is ever 'shift' or 'accept' then we consume the input terminal and return no continuation
		// else the continuation is the 'non-terminal' just reduced
		terminal* consume(nat p, terminal* t) {
			if (failed()) {
				return 0;
			}

			const lrstate&          st = this->table[this->states.top()];
			lrstate::const_iterator a  = st.find(t->token());

			if (a == st.end()) {
				this->failure = unexpectedTokenFailure(t);
				return 0;
			} else if (a->second.isGoTo()) {
				goTo(a->second);
				return 0;
			} else if (a->second.isShift()) {
				shift(p, t, a->second);
				return 0;
			} else if (a->second.isReduce()) {
				reduce(p, a->second);
				return a->second.reduceSym();
			} else if (a->second.isAccept()) {
				this->inAccept = true;
				return 0;
			} else {
				throw std::runtime_error("Internal error, invalid LR parser action.");
			}
		}

		// we received an unexpected token -- gather up all relevant information
		parsefailp unexpectedTokenFailure(terminal* t) const {
			return parsefailp(new unexpected_terminal(this->states.top(), t, expecting()));
		}

		// we encountered an exception while trying to apply a reduce function
		parsefailp reduceException(terminal* t, const terminals& args, nat pi, nat pf, const std::string& msg) const {
			return parsefailp(new reduce_exception(t, args, pi, pf, msg));
		}
	};

}

#endif

