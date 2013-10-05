
/*
 * evaluator : automate the process of writing evaluators for parser rules
 */

#ifndef WW_EVALUATOR_HPP_INCLUDED
#define WW_EVALUATOR_HPP_INCLUDED

#include <ww/language.hpp>
#include <ww/type.hpp>
#include <ww/data.hpp>

namespace ww {

// inject a C++ value into a parser value box
struct inj {
	terminal*        s;
	parser_state_ptr ps;
	nat              pi, pf;

	inj(terminal* s, const parser_state_ptr& ps, nat pi, nat pf) : s(s), ps(ps), pi(pi), pf(pf) {
	}

	template <typename T>
		terminal* apply(T x) const {
			return this->ps->tb.apply(new lexwrap<T>(this->s, this->ps->ldb, pi, pf, x));
		}
};

// unwrap typed C++ values
template <typename T>
	struct unwrap {
		static T value(terminal* t) {
			if (lexwrap<T>* tt = dynamic_cast<lexwrap<T>*>(t)) {
				return tt->value();
			} else if (wrapper<T>* tt = dynamic_cast<wrapper<T>*>(t)) {
				return tt->value();
			} else {
				throw std::runtime_error("Internal error, unable to unwrap incorrectly-typed value, '" + show(t) + "'");
			}
		}
	};

template <>
	struct unwrap<char> {
		static char value(terminal* t) {
			if (character* c = dynamic_cast<character*>(t)) {
				return c->value();
			} else if (lexwrap<char>* tt = dynamic_cast<lexwrap<char>*>(t)) {
				return tt->value();
			} else if (wrapper<char>* tt = dynamic_cast<wrapper<char>*>(t)) {
				return tt->value();
			} else {
				throw std::runtime_error("Internal error, unable to unwrap incorrectly-typed value, '" + show(t) + "'");
			}
		}
	};

template <>
	struct unwrap<std::string> {
		static std::string value(terminal* t) {
			if (symbol* v = dynamic_cast<symbol*>(t)) {
				return v->name();
			} else if (character* v = dynamic_cast<character*>(t)) {
				return std::string(1, v->value());
			} else if (lexwrap<char>* v = dynamic_cast<lexwrap<char>*>(t)) {
				return std::string(1, v->value());
			} else if (lexwrap<std::string>* v = dynamic_cast<lexwrap<std::string>*>(t)) {
				return v->value();
			} else if (wrapper<std::string>* v = dynamic_cast<wrapper<std::string>*>(t)) {
				return v->value();
			} else {
				throw std::runtime_error("Internal error, unable to unwrap incorrectly-typed value, '" + show(t) + "'");
			}
		}
	};

template <>
	struct unwrap<terminal*> {
		static terminal* value(terminal* t) {
			return t;
		}
	};

// generic reduction functions injecting values into 'terminal'
typedef std::string CTypeName;
typedef std::vector<CTypeName> CTypeNames;

struct reducefn {
	virtual terminal* apply(const inj&, const terminals&) const = 0;
	virtual TypePtr type() const = 0;
};
typedef shared_ptr<reducefn> reducefnp;

#include <ww/reducefns.hpp>

// any reducefn can be made into a rule-evaluator
class reduceFnEvaluator : public ruleEvaluator<terminal*> {
public:
	reduceFnEvaluator(terminal* s, const parser_state_ptr& ps, const reducefnp& f) : s(s), ps(ps), f(f) { }

	terminal* eval(const terminals& cs, const nats& pis, const nats& pfs) {
		return this->f->apply(inj(this->s, this->ps, pis[0], pfs[pfs.size() - 1]), cs);
	}
private:
	terminal*        s;
	parser_state_ptr ps;
	reducefnp        f;
};

inline ruleEvaluator<terminal*>::ptr lexaction(terminal* s, const parser_state_ptr& ps, const reducefnp& f) {
	return ruleEvaluator<terminal*>::ptr(new reduceFnEvaluator(s, ps, f));
}

}

#endif
