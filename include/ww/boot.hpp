/*
 * boot : a "bootstrap" grammar for incrementally building grammars
 */

#ifndef WW_BOOT_HPP_INCLUDED
#define WW_BOOT_HPP_INCLUDED

#include <ww/language.hpp>
#include <ww/evaluator.hpp>
#include <ww/type.hpp>
#include <ww/data.hpp>

namespace ww {

typedef ruleEvaluator<terminal*>::ptr evalp;

// define a reduction function -- put it in the type and value environments
inline void define(const parser_state_ptr& ps, const std::string& vname, const reducefnp& f) {
	ps->tenv->define(vname, f->type());
	ps->venv->define(vname, new wrapper<reducefnp>(0, f));
}

// declare a symbol's type
inline void declare(const parser_state_ptr& ps, const std::string& pname, const TypePtr& ty) {
	if (ps->tenv->hasBinding(pname)) {
		TypePtr dty = ps->tenv->lookup(pname);
		if (!(*ty == *dty)) {
			std::ostringstream ss;
			ss << "Cannot redeclare '" << pname << "' with type '";
			ty->show(ss);
			ss << "', it was previously declared with type '";
			dty->show(ss);
			ss << "'";

			throw std::runtime_error(ss.str());
		}
	} else {
		ps->tenv->define(pname, ty);
	}
}

/*
 * identifiers
 */
std::string addCS(char c, std::string sfx);
std::string addSC(std::string pfx, char c);
char cid(char c);
std::string emptyS();

/*
 * regex patterns
 */
class RegexPattern {
public:
	virtual ~RegexPattern() { }
};
typedef shared_ptr<RegexPattern> RegexPatternPtr;
typedef std::vector<RegexPatternPtr> RegexPatterns;

class RegexChar;
typedef shared_ptr<RegexChar> RegexCharPtr;
typedef std::vector<RegexCharPtr> RegexChars;

class RPatMatchChar : public RegexPattern {
public:
	RPatMatchChar(const RegexCharPtr& p) : p(p) { }

	const RegexCharPtr& value() const { return this->p; }
private:
	RegexCharPtr p;
};

class RPatMatchOneOf : public RegexPattern {
public:
	RPatMatchOneOf(const RegexChars& cs) : cs(cs) { }

	const RegexChars& value() const { return this->cs; }
private:
	RegexChars cs;
};

class RPatMatchOneNotOf : public RegexPattern {
public:
	RPatMatchOneNotOf(const RegexChars& cs) : cs(cs) { }

	const RegexChars& value() const { return this->cs; }
private:
	RegexChars cs;
};

class RPatMatch0toN : public RegexPattern {
public:
	RPatMatch0toN(const RegexPatternPtr& p) : p(p) { }

	const RegexPatternPtr& value() const { return this->p; }
private:
	RegexPatternPtr p;
};

class RPatMatch1toN : public RegexPattern {
public:
	RPatMatch1toN(const RegexPatternPtr& p) : p(p) { }

	const RegexPatternPtr& value() const { return this->p; }
private:
	RegexPatternPtr p;
};

class RPatMatch0to1 : public RegexPattern {
public:
	RPatMatch0to1(const RegexPatternPtr& p) : p(p) { }

	const RegexPatternPtr& value() const { return this->p; }
private:
	RegexPatternPtr p;
};

class RPatMatchN : public RegexPattern {
public:
	RPatMatchN(const RegexPatternPtr& p, nat n) : p(p), n(n) { }

	const RegexPatternPtr& value() const { return this->p; }
	nat                    size() const { return this->n; }
private:
	RegexPatternPtr p;
	nat             n;
};

template <typename T>
	struct switchRegexPattern {
		virtual T with(const RPatMatchChar*)     const = 0;
		virtual T with(const RPatMatchOneOf*)    const = 0;
		virtual T with(const RPatMatchOneNotOf*) const = 0;
		virtual T with(const RPatMatch0toN*)     const = 0;
		virtual T with(const RPatMatch1toN*)     const = 0;
		virtual T with(const RPatMatch0to1*)     const = 0;
		virtual T with(const RPatMatchN*)        const = 0;
	};

template <typename T>
	T switchOf(const RegexPattern& p, const switchRegexPattern<T>& f) {
		if        (const RPatMatchChar*     v = dynamic_cast<const RPatMatchChar*>(&p)) {
			return f.with(v);
		} else if (const RPatMatchOneOf*    v = dynamic_cast<const RPatMatchOneOf*>(&p)) {
			return f.with(v);
		} else if (const RPatMatchOneNotOf* v = dynamic_cast<const RPatMatchOneNotOf*>(&p)) {
			return f.with(v);
		} else if (const RPatMatch0toN*     v = dynamic_cast<const RPatMatch0toN*>(&p)) {
			return f.with(v);
		} else if (const RPatMatch1toN*     v = dynamic_cast<const RPatMatch1toN*>(&p)) {
			return f.with(v);
		} else if (const RPatMatch0to1*     v = dynamic_cast<const RPatMatch0to1*>(&p)) {
			return f.with(v);
		} else if (const RPatMatchN*        v = dynamic_cast<const RPatMatchN*>(&p)) {
			return f.with(v);
		} else {
			throw std::runtime_error("Internal error, no such regex pattern type");
		}
	}

template <typename T> T switchOf(const RegexPattern*    p, const switchRegexPattern<T>& f) { return switchOf(*p, f); }
template <typename T> T switchOf(const RegexPatternPtr& p, const switchRegexPattern<T>& f) { return switchOf(*p, f); }

template <typename T>
	std::vector<T> switchOf(const RegexPatterns& ps, const switchRegexPattern<T>& f) {
		std::vector<T> r;
		for (RegexPatterns::const_iterator p = ps.begin(); p != ps.end(); ++p) {
			r.push_back(switchOf(**p, f));
		}
		return r;
	}

RegexPatterns   makeRegex(char ign0, RegexPatterns ps, char ign1);
RegexPatterns   appendRgxPats(RegexPatterns ps, RegexPatternPtr p);
RegexPatterns   emptyRgxPats();
RegexPatternPtr makePatFromRgxchar(RegexCharPtr p);
RegexPatternPtr makePatFromRgxcharAlts(char ign0, RegexChars alts, char ign1);
RegexPatternPtr makePatFromNegRgxcharAlts(char ign0, char ign1, RegexChars alts, char ign2);
RegexPatternPtr makePatFromNestedRgxchars(char ign0, RegexChars cs, char ign1);
RegexPatternPtr makeZeroOrMoreRgxpat(RegexPatternPtr p, char ign);
RegexPatternPtr makeOneOrMoreRgxpat(RegexPatternPtr p, char ign);
RegexPatternPtr makeZeroOrOneRgxpat(RegexPatternPtr p, char ign);
RegexPatternPtr makeRepeatNRgxpat(RegexPatternPtr p, char ign0, nat n, char ign1);

/*
 * regex chars (primitive regex patterns)
 */
class RegexChar {
public:
	virtual ~RegexChar() { }
};

class RCharAny : public RegexChar {
public:
	RCharAny() { }
};

class RCharRange : public RegexChar {
public:
	RCharRange(char i, char f) : i(i), f(f) { }
	char initial() const { return this->i; }
	char final()   const { return this->f; }
private:
	char i, f;
};

class RCharLit : public RegexChar {
public:
	RCharLit(char c) : c(c) { }
	char value() const { return this->c; }
private:
	char c;
};

template <typename T>
	struct switchRegexChar {
		virtual T with(const RCharAny*)   const = 0;
		virtual T with(const RCharRange*) const = 0;
		virtual T with(const RCharLit*)   const = 0;
	};

template <typename T>
	T switchOf(const RegexChar& c, const switchRegexChar<T>& f) {
		if (const RCharAny* v = dynamic_cast<const RCharAny*>(&c)) {
			return f.with(v);
		} else if (const RCharRange* v = dynamic_cast<const RCharRange*>(&c)) {
			return f.with(v);
		} else if (const RCharLit* v = dynamic_cast<const RCharLit*>(&c)) {
			return f.with(v);
		} else {
			throw std::runtime_error("Internal error, no such regex char type");
		}
	}

template <typename T> T switchOf(const RegexChar*    c, const switchRegexChar<T>& f) { return switchOf(*c, f); }
template <typename T> T switchOf(const RegexCharPtr& c, const switchRegexChar<T>& f) { return switchOf(*c, f); }

template <typename T>
	std::vector<T> switchOf(const RegexChars& cs, const switchRegexChar<T>& f) {
		std::vector<T> r;
		for (RegexChars::const_iterator c = cs.begin(); c != cs.end(); ++c) {
			r.push_back(switchOf(**c, f));
		}
		return r;
	}

RegexChars appendRChars(RegexChars cs, RegexCharPtr c);
RegexChars makeRChars(RegexCharPtr c);
RegexCharPtr makeRcharAny(char ign);
RegexCharPtr makeRcharRange(char i, char ign, char f);
RegexCharPtr makeRcharLit(char c);
RegexCharPtr makeEscRcharLit(char ign, char c);

// the meaning of a regex char is as a set of characters
struct compileRCharF : public switchRegexChar<std::string> {
	std::string with(const RCharAny*) const {
		return printableChars();
	}

	std::string with(const RCharRange* v) const {
		return charRange(v->initial(), v->final());
	}

	std::string with(const RCharLit* v) const {
		return std::string(1, v->value());
	}
};

// the meaning of a regex pattern is as a set of rule definitions
typedef std::vector<terminals> rulebodies;

rulebodies charAlts(symbolset& ss, const std::string& cs);
terminal* freshSym(symbolset& ss);

struct regexMatchEval : public ruleEvaluator<terminal*> {
	terminal*        s;
	parser_state_ptr ps;

	regexMatchEval(terminal* s, const parser_state_ptr& ps) : s(s), ps(ps) { }

	terminal* eval(const terminals& ts, const nats& pis, const nats& pfs) {
		std::ostringstream ss;
		for (terminals::const_iterator t = ts.begin(); t != ts.end(); ++t) {
			ss << unwrap<std::string>::value(*t);
		}
		return this->ps->tb.apply(new lexwrap<std::string>(this->s, this->ps->ldb, pis[0], pfs[pfs.size() - 1], ss.str()));
	}
};
typedef shared_ptr<parser_state> parser_state_ptr;

void define(const parser_state_ptr& ps, terminal* s, const terminals& ts, const TypePtr& ty, const evalp& eval);
void define(const parser_state_ptr& ps, terminal* s, const terminals& ts, const evalp& eval);
void define(const parser_state_ptr& ps, terminal* s, const rulebodies& rs, const TypePtr& ty, const evalp& eval);
void define(const parser_state_ptr& ps, terminal* s, const rulebodies& rs, const evalp& eval);
terminal* collapse(const parser_state_ptr& ps, const rulebodies& rs);
terminals collapse(const parser_state_ptr& ps, const std::vector<rulebodies>& rss);
terminal* collapseRegexTerminals(const parser_state_ptr& ps, const terminals& ts);

struct compilePatternF : public switchRegexPattern<rulebodies> {
	compilePatternF(const parser_state_ptr& ps) : ps(ps) { }

	rulebodies with(const RPatMatchChar* c) const {
		return charAlts(this->ps->ss, switchOf(*c->value(), compileRCharF()));
	}

	rulebodies with(const RPatMatchOneOf* v) const {
		return charAlts(this->ps->ss, concat(switchOf(v->value(), compileRCharF())));
	}

	rulebodies with(const RPatMatchOneNotOf* v) const {
		return charAlts(this->ps->ss, difference(printableChars(), concat(switchOf(v->value(), compileRCharF()))));
	}

	rulebodies with(const RPatMatch0toN* v) const {
		terminal* x   = collapse(switchOf(v->value(), *this));
		terminal* rep = freshSym();

		define(rep, list(rep, x));
		define(rep, terminals());

		return list(list(rep));
	}

	rulebodies with(const RPatMatch1toN* v) const {
		terminal* x   = collapse(switchOf(v->value(), *this));
		terminal* rep = freshSym();

		define(rep, list(rep, x));
		define(rep, list(x));

		return list(list(rep));
	}

	rulebodies with(const RPatMatch0to1* v) const {
		terminal* x   = collapse(switchOf(v->value(), *this));
		terminal* rep = freshSym();

		define(rep, list(x));
		define(rep, terminals());

		return list(list(rep));
	}

	rulebodies with(const RPatMatchN* v) const {
		terminal* x   = collapse(switchOf(v->value(), *this));
		terminal* rep = freshSym();

		terminals ts;
		for (nat i = 0; i < v->size(); ++i) {
			ts.push_back(x);
		}
		define(rep, ts);

		return list(list(rep));
	}

private:
	parser_state_ptr ps;

	terminal* freshSym() const {
		return ::ww::freshSym(this->ps->ss);
	}

	void define(terminal* s, const terminals& ts) const {
		::ww::define(this->ps, s, ts, newp<regexMatchEval>(s, this->ps));
	}

	terminal* collapse(const rulebodies& rs) const {
		return ::ww::collapse(this->ps, rs);
	}
};

void define(const parser_state_ptr& ps, terminal* s, const RegexPatterns& rps, const TypePtr& ty, const evalp& eval);
void define(const parser_state_ptr& ps, terminal* s, const std::vector<RegexPatterns>& rpss, const TypePtr& ty, const evalp& eval);

void define(const parser_state_ptr& ps, terminal* s, const RegexPatterns& rps, const evalp& eval);
void define(const parser_state_ptr& ps, terminal* s, const std::vector<RegexPatterns>& rpss, const evalp& eval);

typedef std::pair<VarName, RegexPatterns> RegexBinding;
typedef std::vector<RegexBinding>         RegexBindings;

std::vector<RegexPatterns> patterns(const RegexBindings& bs);
VarPos varPos(const RegexBindings& bs);

// lexer rule defs with binding annotations
struct accumLexBinds : public ruleEvaluator<terminal*> {
	parser_state_ptr ps;
	accumLexBinds(const parser_state_ptr& ps) : ps(ps) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		RegexBinding rb = unwrap<RegexBinding>::value(xs[1]);

		if (lexwrap<RegexBindings>* bs = dynamic_cast<lexwrap<RegexBindings>*>(xs[0])) {
			bs->ref().push_back(rb);
		} else {
			throw std::runtime_error("Internal error, binding sequence given wrong type");
		}
		return xs[0];
	}
};

struct makeLexBinds : public ruleEvaluator<terminal*> {
	terminal* s;
	parser_state_ptr ps;
	makeLexBinds(terminal* s, const parser_state_ptr& ps) : s(s), ps(ps) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		RegexBinding rb = unwrap<RegexBinding>::value(xs[0]);
		return this->ps->tb.apply(new lexwrap<RegexBindings>(this->s, this->ps->ldb, pis[0], pfs[pfs.size() - 1], list(rb)));
	}
};

struct makeLexBind : public ruleEvaluator<terminal*> {
	terminal* s;
	parser_state_ptr ps;
	makeLexBind(terminal* s, const parser_state_ptr& ps) : s(s), ps(ps) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		std::string   vn  = unwrap<std::string>::value(xs[0]);
		RegexPatterns rps = unwrap<RegexPatterns>::value(xs[2]);
		return this->ps->tb.apply(new lexwrap<RegexBinding>(this->s, this->ps->ldb, pis[0], pfs[pfs.size() - 1], RegexBinding(vn, rps)));
	}
};

struct makeAnonLexBind : public ruleEvaluator<terminal*> {
	terminal* s;
	parser_state_ptr ps;
	makeAnonLexBind(terminal* s, const parser_state_ptr& ps) : s(s), ps(ps) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		RegexPatterns rps = unwrap<RegexPatterns>::value(xs[0]);
		return this->ps->tb.apply(new lexwrap<RegexBinding>(this->s, this->ps->ldb, pis[0], pfs[pfs.size() - 1], RegexBinding("_", rps)));
	}
};

// parser rule defs with binding annotations
typedef std::pair<VarName, VarName> Binding;
typedef std::vector<Binding>        Bindings;

std::vector<VarName> expressions(const Bindings& bs);
VarPos varPos(const Bindings& bs);

struct accumRuleBinds : public ruleEvaluator<terminal*> {
	terminal* s;
	parser_state_ptr ps;
	accumRuleBinds(terminal* s, const parser_state_ptr& ps) : s(s), ps(ps) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		Binding b = unwrap<Binding>::value(xs[1]);

		if (wrapper<Bindings>* bs = dynamic_cast<wrapper<Bindings>*>(xs[0])) {
			bs->ref().push_back(b);
		} else {
			throw std::runtime_error("Internal error, binding sequence given wrong type");
		}
		return xs[0];
	}
};

struct makeEmptyRuleBinds : public ruleEvaluator<terminal*> {
	terminal* s;
	parser_state_ptr ps;
	makeEmptyRuleBinds(terminal* s, const parser_state_ptr& ps) : s(s), ps(ps) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		return this->ps->tb.apply(new wrapper<Bindings>(this->s, Bindings()));
	}
};

struct makeRuleBind : public ruleEvaluator<terminal*> {
	terminal* s;
	parser_state_ptr ps;
	makeRuleBind(terminal* s, const parser_state_ptr& ps) : s(s), ps(ps) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		std::string vn = unwrap<std::string>::value(xs[0]);
		std::string vv = unwrap<std::string>::value(xs[2]);

		return this->ps->tb.apply(new wrapper<Binding>(this->s, Binding(vn, vv)));
	}
};

struct makeAnonRuleBind : public ruleEvaluator<terminal*> {
	terminal* s;
	parser_state_ptr ps;
	makeAnonRuleBind(terminal* s, const parser_state_ptr& ps) : s(s), ps(ps) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		std::string vv = unwrap<std::string>::value(xs[0]);
		return this->ps->tb.apply(new wrapper<Binding>(this->s, Binding("_", vv)));
	}
};

// nat reducers
nat accNat(nat n, nat d);
nat makeNat(nat d);
nat makeDigit(char c);

/*
 * produce new actions as evaluations on expressions
 */
struct makeReduction : public ruleEvaluator<terminal*> {
	terminal* s;
	parser_state_ptr ps;
	makeReduction(terminal* s, const parser_state_ptr& ps) : s(s), ps(ps) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		ExprPtr e = unwrap<ExprPtr>::value(xs[0]);

		// if this was misclassified as an app, 'unbox' it from app
		if (const App* v = dynamic_cast<const App*>(e.get())) {
			if (v->value().size() == 1) {
				e = v->value()[0];
			}
		}

		return this->ps->tb.apply(new lexwrap<ExprPtr>(this->s, xs, e));
	}
};

struct accumRApp : public ruleEvaluator<terminal*> {
	terminal* s;
	parser_state_ptr ps;
	accumRApp(terminal* s, const parser_state_ptr& ps) : s(s), ps(ps) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		ExprPtr ap = unwrap<ExprPtr>::value(xs[0]);
		ExprPtr a  = unwrap<ExprPtr>::value(xs[1]);

		App*     app = dynamic_cast<App*>(ap.get());
		lexterm* t0  = dynamic_cast<lexterm*>(xs[0]);
		lexterm* t1  = dynamic_cast<lexterm*>(xs[1]);

		if (!app)       { throw std::runtime_error("Internal error, mis-application"); }
		if (!t0 || !t1) { throw std::runtime_error("Internal error, non-lexical tokens produced below accumRApp"); }

		app->value().push_back(a);
		t0->finalPos(t1->finalPos());

		return xs[0];
	}
};

struct makeRApp : public ruleEvaluator<terminal*> {
	terminal* s;
	parser_state_ptr ps;
	makeRApp(terminal* s, const parser_state_ptr& ps) : s(s), ps(ps) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		ExprPtr e = unwrap<ExprPtr>::value(xs[0]);

		Exprs es;
		es.push_back(e);

		return this->ps->tb.apply(new lexwrap<ExprPtr>(this->s, xs, ExprPtr(new App(es))));
	}
};

struct makeVarRef : public ruleEvaluator<terminal*> {
	terminal* s;
	parser_state_ptr ps;
	makeVarRef(terminal* s, const parser_state_ptr& ps) : s(s), ps(ps) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		std::string n = unwrap<std::string>::value(xs[0]);

		return this->ps->tb.apply(new lexwrap<ExprPtr>(this->s, xs, ExprPtr(new Var(n))));
	}
};

struct debugAction : public ruleEvaluator<terminal*> {
	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		std::cout << "debug action received [";
		if (xs.size() > 0) {
			std::cout << show(xs[0]);
			for (unsigned int i = 1; i < xs.size(); ++i) {
				std::cout << " ";
				std::cout << show(xs[i]);
			}
		}
		std::cout << "]" << std::endl;
		return xs[0];
	}
};

class evaluateF : public switchExpr<terminal*> {
public:
	evaluateF(const inj& j, const ValueEnvPtr& env) : j(j), env(env) { }

	terminal* with(const Var* v) const {
		return this->env->lookup(v->value());
		//return this->j.apply(this->env->lookup(v->value()));
	}

	terminal* with(const App* v) const {
		const Exprs& es = v->value();

		return unwrap<reducefnp>::value(switchOf(es[0], *this))->apply(this->j, switchOf(Exprs(es.begin() + 1, es.end()), *this));
	}
private:
	const inj&         j;
	const ValueEnvPtr& env;
};

inline terminal* evaluate(const inj& j, const ValueEnvPtr& vs, const ExprPtr& e) {
	return switchOf(e, evaluateF(j, vs));
}

struct evalExpr : public ruleEvaluator<terminal*> {
	terminal*        s;
	parser_state_ptr ps;
	ExprPtr          e;
	VarPos           vpos;

	evalExpr(terminal* s, const parser_state_ptr& ps, const ExprPtr& e, const VarPos& vpos) : s(s), ps(ps), e(e), vpos(vpos) {
	}

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		return evaluate(inj(this->s, this->ps, pis[0], pfs[pfs.size() - 1]), ValueEnvPtr(new ValueEnv(this->vpos, xs, ps->venv)), this->e);
	}
};

struct boxUnitTok : public ruleEvaluator<terminal*> {
	terminal* s;
	parser_state_ptr ps;
	boxUnitTok(terminal* s, const parser_state_ptr& ps) : s(s), ps(ps) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		return this->ps->tb.apply(new lexwrap<void>(this->s, this->ps->ldb, pis[0], pfs[pfs.size()-1]));
	}
};

/*
 * define new lexemes
 */
inline TypeEnvPtr lexTEnv(const TypeEnvPtr& tenv, const VarPos& vp) {
	TypeEnvPtr result(new TypeEnv(tenv));
	for (VarPos::const_iterator v = vp.begin(); v != vp.end(); ++v) {
		result->define(v->first, liftty<std::string>());
	}
	return result;
}

struct evalLexemeDef : public ruleEvaluator<terminal*> {
	parser_state_ptr ps;
	evalLexemeDef(const parser_state_ptr& ps) : ps(ps) { }

	terminal* eval(const std::vector<terminal*>& xs, const nats& pis, const nats& pfs) {
		terminal*     t    = this->ps->ss(unwrap<std::string>::value(xs[1]));
		RegexBindings bs   = unwrap<RegexBindings>::value(xs[3]);
		ExprPtr       expr = unwrap<ExprPtr>::value(xs[5]);

		define(this->ps, t, patterns(bs), typeOf(lexTEnv(this->ps->tenv, varPos(bs)), expr), newp<evalExpr>(t, this->ps, expr, varPos(bs)));
		this->ps->lex.pushLexer(t);

		return 0;
	}
};

struct evalLexemeDefUnit : public ruleEvaluator<terminal*> {
	parser_state_ptr ps;
	evalLexemeDefUnit(const parser_state_ptr& ps) : ps(ps) { }

	terminal* eval(const std::vector<terminal*>& xs, const nats& pis, const nats& pfs) {
		terminal*     t    = this->ps->ss(unwrap<std::string>::value(xs[1]));
		RegexBindings bs   = unwrap<RegexBindings>::value(xs[3]);

		define(this->ps, t, patterns(bs), liftty<void>(), newp<boxUnitTok>(t, this->ps));
		this->ps->lex.pushLexer(t);

		return 0;
	}
};

/*
 * declare lexeme associativity
 */
struct evalAssocDef : public ruleEvaluator<terminal*> {
	parser_state_ptr ps;
	bool             assocl;
	evalAssocDef(const parser_state_ptr& ps, bool assocl) : ps(ps), assocl(assocl) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		std::string sn = unwrap<std::string>::value(xs[1]);
		this->ps->prec[this->ps->ss(sn)] = prec(0, this->assocl ? assoc::left : assoc::right);
		return 0;
	}
};

struct evalRelAssocDef : public ruleEvaluator<terminal*> {
	parser_state_ptr ps;
	bool             assocl;
	evalRelAssocDef(const parser_state_ptr& ps, bool assocl) : ps(ps), assocl(assocl) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		std::string sn = unwrap<std::string>::value(xs[1]);
		std::string an = unwrap<std::string>::value(xs[3]);

		precedence::const_iterator rp = this->ps->prec.find(this->ps->ss(an));
		if (rp == this->ps->prec.end()) {
			throw std::runtime_error("Cannot define precedence for '" + sn + "' before undefined lexeme '" + an + "'");
		}

		this->ps->prec[this->ps->ss(sn)] = prec(rp->second.level + 1, this->assocl ? assoc::left : assoc::right);
		return 0;
	}
};

/*
 * define new rules
 */
inline TypeEnvPtr bindTEnv(const TypeEnvPtr& tenv, const Bindings& bs) {
	TypeEnvPtr result(new TypeEnv(tenv));
	for (Bindings::const_iterator b = bs.begin(); b != bs.end(); ++b) {
		result->define(b->first, tenv->lookup(b->second));
	}
	return result;
}

struct evalRuleDef : public ruleEvaluator<terminal*> {
	parser_state_ptr ps;
	bool nested;
	evalRuleDef(const parser_state_ptr& ps, bool nested) : ps(ps), nested(nested) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		std::string n    = unwrap<std::string>::value(xs[1]);
		Bindings    bs   = unwrap<Bindings>::value(xs[3]);
		ExprPtr     expr = unwrap<ExprPtr>::value(xs[5]);
		terminal*   t    = this->ps->ss(n);

		declare(this->ps, n, typeOf(bindTEnv(this->ps->tenv, bs), expr));
		this->ps->lang.asAllOf(t, this->ps->ss.symbols(expressions(bs)), newp<evalExpr>(t, this->ps, expr, varPos(bs)));

		// we've changed it, recompile the parser
		// (since we're called here from the parser, set aside an old copy to prevent an access violation)
		parser_state::plang::parserp lp = this->ps->parser;
		this->ps->parser = this->ps->lang.compile(this->ps->prec);
		this->ps->lastp = lp;

		// if this was a 'nested' rule definition, we have to be sure to remember that we're in the 'nested definition' state
		if (this->nested) {
			this->ps->parser->transition(0, this->ps->ss("syntax"));
			this->ps->parser->transition(0, this->ps->ss("{"));
		}

		return 0;
	}
};

// add lex/parse rules for the basic parser-generator statements
void mergeBootstrapRules(const parser_state_ptr& ps);

}

#endif
