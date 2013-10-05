
#include <ww/boot.hpp>

namespace ww {

/*
 * identifiers
 */
std::string addCS(char c, std::string sfx) {
	std::ostringstream ss;
	ss << c;
	ss << sfx;
	return ss.str();
}
std::string addSC(std::string pfx, char c) {
	std::ostringstream ss;
	ss << pfx;
	ss << c;
	return ss.str();
}
char cid(char c) {
	return c;
}

std::string emptyS() {
	return std::string("");
}

/*
 * grammar construction from regular expressions
 */
RegexPatterns makeRegex(char ign0, RegexPatterns ps, char ign1) {
	return ps;
}

RegexPatterns makeRegexFromChars(char ign0, RegexChars cs, char ign1) {
	RegexPatterns result;
	for (RegexChars::const_iterator c = cs.begin(); c != cs.end(); ++c) {
		result.push_back(RegexPatternPtr(new RPatMatchChar(*c)));
	}
	return result;
}

RegexPatterns appendRgxPats(RegexPatterns ps, RegexPatternPtr p) {
	return add(ps, p);
}

RegexPatterns emptyRgxPats() {
	return RegexPatterns();
}

RegexPatternPtr makePatFromRgxchar(RegexCharPtr p) {
	return RegexPatternPtr(new RPatMatchChar(p));
}

RegexPatternPtr makePatFromRgxcharAlts(char ign0, RegexChars alts, char ign1) {
	return RegexPatternPtr(new RPatMatchOneOf(alts));
}

RegexPatternPtr makePatFromNegRgxcharAlts(char ign0, char ign1, RegexChars alts, char ign2) {
	return RegexPatternPtr(new RPatMatchOneNotOf(alts));
}

RegexPatternPtr makePatFromNestedRgxchars(char ign0, RegexChars cs, char ign1) {
	throw std::runtime_error("nyi");
}

RegexPatternPtr makeZeroOrMoreRgxpat(RegexPatternPtr p, char ign) {
	return RegexPatternPtr(new RPatMatch0toN(p));
}

RegexPatternPtr makeOneOrMoreRgxpat(RegexPatternPtr p, char ign) {
	return RegexPatternPtr(new RPatMatch1toN(p));
}

RegexPatternPtr makeZeroOrOneRgxpat(RegexPatternPtr p, char ign) {
	return RegexPatternPtr(new RPatMatch0to1(p));
}

RegexPatternPtr makeRepeatNRgxpat(RegexPatternPtr p, char ign0, nat n, char ign1) {
	return RegexPatternPtr(new RPatMatchN(p, n));
}

/*
 * grammar construction from single regex 'chars'
 */
RegexChars appendRChars(RegexChars cs, RegexCharPtr c) {
	return add(cs, c);
}

RegexChars makeRChars(RegexCharPtr c) {
	return list(c);
}

RegexChars makeEmptyRChars() {
	return RegexChars();
}

RegexCharPtr makeRcharAny(char ign) {
	return RegexCharPtr(new RCharAny());
}

RegexCharPtr makeRcharRange(char i, char ign, char f) {
	return RegexCharPtr(new RCharRange(i, f));
}

RegexCharPtr makeRcharLit(char c) {
	return RegexCharPtr(new RCharLit(c));
}

RegexCharPtr makeEscRcharLit(char ign, char c) {
	switch (c) {
	case 'r': return RegexCharPtr(new RCharLit('\r'));
	case 'n': return RegexCharPtr(new RCharLit('\n'));
	case 't': return RegexCharPtr(new RCharLit('\t'));
	default:  return RegexCharPtr(new RCharLit(c));
	}
}

/*
 * compile regex constructions to grammar rules
 */
rulebodies charAlts(symbolset& ss, const std::string& cs) {
	rulebodies r;
	for (std::string::const_iterator c = cs.begin(); c != cs.end(); ++c) {
		r.push_back(list(ss(*c)));
	}
	return r;
}

terminal* freshSym(symbolset& ss) {
	static nat c = 0;
	return ss(".gc" + toString(c++));
}

void define(const parser_state_ptr& ps, terminal* s, const terminals& ts, const TypePtr& ty, const evalp& eval) {
	ps->lex.asAllOf(s, ts, eval);
	declare(ps, unwrap<std::string>::value(s), ty);
}

void define(const parser_state_ptr& ps, terminal* s, const terminals& ts, const evalp& eval) {
	define(ps, s, ts, liftty<std::string>(), eval);
}

void define(const parser_state_ptr& ps, terminal* s, const rulebodies& rs, const TypePtr& ty, const evalp& eval) {
	for (rulebodies::const_iterator r = rs.begin(); r != rs.end(); ++r) {
		define(ps, s, *r, ty, eval);
	}
}

void define(const parser_state_ptr& ps, terminal* s, const rulebodies& rs, const evalp& eval) {
	define(ps, s, rs, liftty<std::string>(), eval);
}

terminal* collapse(const parser_state_ptr& ps, const rulebodies& rs) {
	if (rs.size() == 1 && rs[0].size() == 1) {
		return rs[0][0];
	} else {
		terminal* ns = freshSym(ps->ss);
		define(ps, ns, rs, newp<regexMatchEval>(ns, ps));
		return ns;
	}
}

terminals collapse(const parser_state_ptr& ps, const std::vector<rulebodies>& rss) {
	terminals r;
	for (std::vector<rulebodies>::const_iterator rs = rss.begin(); rs != rss.end(); ++rs) {
		r.push_back(collapse(ps, *rs));
	}
	return r;
}

terminal* collapseRegexTerminals(const parser_state_ptr& ps, const terminals& ts) {
	if (ts.size() == 1) {
		return ts[0];
	} else {
		terminal* ns = freshSym(ps->ss);
		define(ps, ns, ts, newp<regexMatchEval>(ns, ps));
		return ns;
	}
}

void define(const parser_state_ptr& ps, terminal* s, const RegexPatterns& rps, const TypePtr& ty, const evalp& eval) {
	define(ps, s, collapse(ps, switchOf(rps, compilePatternF(ps))), ty, eval);
}

void define(const parser_state_ptr& ps, terminal* s, const std::vector<RegexPatterns>& rpss, const TypePtr& ty, const evalp& eval) {
	terminals ts;
	for (std::vector<RegexPatterns>::const_iterator rps = rpss.begin(); rps != rpss.end(); ++rps) {
		ts.push_back(collapseRegexTerminals(ps, collapse(ps, switchOf(*rps, compilePatternF(ps)))));
	}
	define(ps, s, ts, ty, eval);
}

void define(const parser_state_ptr& ps, terminal* s, const RegexPatterns& rps, const evalp& eval) {
	define(ps, s, rps, liftty<std::string>(), eval);
}

void define(const parser_state_ptr& ps, terminal* s, const std::vector<RegexPatterns>& rpss, const evalp& eval) {
	define(ps, s, rpss, liftty<std::string>(), eval);
}

std::vector<RegexPatterns> patterns(const RegexBindings& bs) {
	std::vector<RegexPatterns> result;
	for (RegexBindings::const_iterator b = bs.begin(); b != bs.end(); ++b) {
		result.push_back(b->second);
	}
	return result;
}

VarPos varPos(const RegexBindings& bs) {
	VarPos result;
	for (unsigned int i = 0; i < bs.size(); ++i) {
		if (bs[i].first != "_") {
			result[bs[i].first] = i;
		}
	}
	return result;
}

std::vector<VarName> expressions(const Bindings& bs) {
	std::vector<VarName> result;
	for (Bindings::const_iterator b = bs.begin(); b != bs.end(); ++b) {
		result.push_back(b->second);
	}
	return result;
}

VarPos varPos(const Bindings& bs) {
	VarPos result;
	for (unsigned int i = 0; i < bs.size(); ++i) {
		if (bs[i].first != "_") {
			result[bs[i].first] = i;
		}
	}
	return result;
}

// nat reducers
nat accNat(nat n, nat d) {
	return n*10 + d;
}
nat makeNat(nat d) { return d; }
nat makeDigit(char c) { return c - '0'; }

struct boxUnitRule : public ruleEvaluator<terminal*> {
	terminal* s;
	parser_state_ptr ps;
	boxUnitRule(terminal* s, const parser_state_ptr& ps) : s(s), ps(ps) { }

	terminal* eval(const terminals& xs, const nats& pis, const nats& pfs) {
		return this->ps->tb.apply(new lexwrap<void>(this->s, xs));
	}
};

// define all of the basic rules for dynamically declaring lexemes and parse rules
void mergeBootstrapRules(const parser_state_ptr& ps) {
	/*
	 * the lexer
	 */
	lexer&      ll  = ps->lex;
	symbolset&  ss  = ps->ss;

	ll.keyword(ss, "syntax");
	ll.keyword(ss, "{");
	ll.keyword(ss, "}");
	ll.keyword(ss, "lexeme");
	ll.keyword(ss, "assocl");
	ll.keyword(ss, "assocr");
	ll.keyword(ss, "before");
	ll.keyword(ss, "rule");
	ll.keyword(ss, "=");
	ll.keyword(ss, ":");
	ll.keyword(ss, "->");
	ll.keyword(ss, ".");

	// id -> '[a-zA-Z_][a-zA-Z_0-9]*'
	ll.asAllOf(ss("id"),     /* -> */ list(ss("idpfxc"), ss("idsfx")),                  lexaction(ss("id"),     ps, reduceWith(&addCS)));
	ll.asOneOf(ss("idpfxc"), /* -> */ ss.cs(charRange('a','z')+charRange('A','Z')+"_"), lexaction(ss("idpfxc"), ps, reduceWith(&cid)));

	ll.asAllOf(ss("idsfx"), list(ss("idsfx"), ss("idsfxc")), lexaction(ss("idsfx"), ps, reduceWith(&addSC)));
	ll.asAllOf(ss("idsfx"), terminals(),                     lexaction(ss("idsfx"), ps, reduceWith(&emptyS)));

	ll.asAllOf(ss("idsfxc"), list(ss("idpfxc")),        lexaction(ss("idsfxc"), ps, reduceWith(&cid)));
	ll.asOneOf(ss("idsfxc"), ss.cs(charRange('0','9')), lexaction(ss("idsfxc"), ps, reduceWith(&cid)));

	// regex -> '"' char* '"' (string literals)
	// regex -> "'" rgxpat* "'" (regular expressions)
	ll.asAllOf(ss("regex"), list(ss('"'),  ss("chars"),   ss('"')),  lexaction(ss("regex"), ps, reduceWith(&makeRegexFromChars)));
	ll.asAllOf(ss("regex"), list(ss('\''), ss("rgxpats"), ss('\'')), lexaction(ss("regex"), ps, reduceWith(&makeRegex)));

	ll.asAllOf(ss("rgxpats"), list(ss("rgxpats"), ss("rgxpat")), lexaction(ss("rgxpats"), ps, reduceWith(&appendRgxPats)));
	ll.asAllOf(ss("rgxpats"), terminals(),                       lexaction(ss("rgxpats"), ps, reduceWith(&emptyRgxPats)));

	// rgxpat -> rchar | [rchars] | [^rchars] | (rchars) | rgxpat* | rgxpat+ | rgxpat? | rgxpat{nat}
	ll.asAllOf(ss("rgxpat"), list(ss("rchar")),                               lexaction(ss("rgxpat"), ps, reduceWith(&makePatFromRgxchar)));
	ll.asAllOf(ss("rgxpat"), list(ss('['), ss("rchars"), ss(']')),            lexaction(ss("rgxpat"), ps, reduceWith(&makePatFromRgxcharAlts)));
	ll.asAllOf(ss("rgxpat"), list(ss('['), ss('^'), ss("rchars"), ss(']')),   lexaction(ss("rgxpat"), ps, reduceWith(&makePatFromNegRgxcharAlts)));
	ll.asAllOf(ss("rgxpat"), list(ss('('), ss("rchars"), ss(')')),            lexaction(ss("rgxpat"), ps, reduceWith(&makePatFromNestedRgxchars)));
	ll.asAllOf(ss("rgxpat"), list(ss("rgxpat"), ss('*')),                     lexaction(ss("rgxpat"), ps, reduceWith(&makeZeroOrMoreRgxpat)));
	ll.asAllOf(ss("rgxpat"), list(ss("rgxpat"), ss('+')),                     lexaction(ss("rgxpat"), ps, reduceWith(&makeOneOrMoreRgxpat)));
	ll.asAllOf(ss("rgxpat"), list(ss("rgxpat"), ss('?')),                     lexaction(ss("rgxpat"), ps, reduceWith(&makeZeroOrOneRgxpat)));
	ll.asAllOf(ss("rgxpat"), list(ss("rgxpat"), ss('{'), ss("nat"), ss('}')), lexaction(ss("rgxpat"), ps, reduceWith(&makeRepeatNRgxpat)));

	// rchars -> rchar+
	ll.asAllOf(ss("rchars"), list(ss("rchars"), ss("rchar")), lexaction(ss("rchars"), ps, reduceWith(&appendRChars)));
	ll.asAllOf(ss("rchars"), list(ss("rchar")),               lexaction(ss("rchars"), ps, reduceWith(&makeRChars)));

	// rchar -> . | [^<control-char>] | \.
	ll.asAllOf(ss("rchar"), list(ss('.')),                           lexaction(ss("rchar"), ps, reduceWith(&makeRcharAny)));
	ll.asAllOf(ss("rchar"), list(ss("lchar"), ss('-'), ss("lchar")), lexaction(ss("rchar"), ps, reduceWith(&makeRcharRange)));
	ll.asOneOf(ss("rchar"), list(ss("lchar")),                       lexaction(ss("rchar"), ps, reduceWith(&makeRcharLit)));
	ll.asAllOf(ss("rchar"), list(ss('\\'), ss("achar")),             lexaction(ss("rchar"), ps, reduceWith(&makeEscRcharLit)));

	// lchar -> <any printable char> minus regex control characters
	std::string controlChars = " \t\r\n-.'[]{}()?^*+\\";
	ll.asOneOf(ss("lchar"), ss.cs(difference(printableChars(), controlChars)), lexaction(ss("lchar"), ps, reduceWith(&cid)));

	// achar -> <any printable char>
	ll.asOneOf(ss("achar"), ss.cs(printableChars()), lexaction(ss("achar"), ps, reduceWith(&cid)));

	// chars -> char*
	ll.asAllOf(ss("chars"), list(ss("chars"), ss("char")), lexaction(ss("chars"), ps, reduceWith(&appendRChars)));
	ll.asAllOf(ss("chars"), terminals(),                   lexaction(ss("chars"), ps, reduceWith(&makeEmptyRChars)));

	// char -> <any printable char> minus \, "
	// char -> \x
	ll.asOneOf(ss("char"), ss.cs(difference(printableChars(), "\\\"")), lexaction(ss("char"), ps, reduceWith(&makeRcharLit)));
	ll.asAllOf(ss("char"), list(ss('\\'), ss("achar")),                 lexaction(ss("char"), ps, reduceWith(&makeEscRcharLit)));

	// nat -> '[0-9]+'
	ll.asAllOf(ss("nat"), list(ss("nat"), ss("digit")), lexaction(ss("nat"), ps, reduceWith(&accNat)));
	ll.asAllOf(ss("nat"), list(ss("digit")),            lexaction(ss("nat"), ps, reduceWith(&makeNat)));

	ll.asOneOf(ss("digit"), ss.cs(charRange('0','9')), lexaction(ss("digit"), ps, reduceWith(&makeDigit)));

	// whitespace (ignored)
	// ws -> [ \t\r\n]+
	ll.asAllOf(ss("ws"), list(ss("ws"), ss("w")), nilR());
	ll.asAllOf(ss("ws"), list(ss("w")),           nilR());
	ll.asOneOf(ss("w"),  ss.cs(" \t\r\n"),        nilR());

	/*
	 * the parser
	 */
	ps->lang.asAllOf(ss("s"), list(ss("statements")), nilR());

	ps->lang.asAllOf(ss("statements"), list(ss("statements"), ss("statement")), nilR());
	ps->lang.asAllOf(ss("statements"), list(ss("statement")),                   nilR());

	// allow new syntax to be introduced
	ps->lang.asAllOf(ss("statement"), list(ss("syntax"), ss("tsyntaxdef")),                    nilR());
	ps->lang.asAllOf(ss("statement"), list(ss("syntax"), ss("{"), ss("nsyntaxdefs"), ss("}")), nilR());

	ps->lang.asAllOf(ss("nsyntaxdefs"), list(ss("nsyntaxdefs"), ss("nsyntaxdef")), nilR());
	ps->lang.asAllOf(ss("nsyntaxdefs"), terminals(),                               nilR());

	ps->lang.asAllOf(ss("tsyntaxdef"), list(ss("lexemedef"), ss(".")), nilR());
	ps->lang.asAllOf(ss("tsyntaxdef"), list(ss("assocdef"),  ss(".")), nilR());
	ps->lang.asAllOf(ss("tsyntaxdef"), list(ss("truledef"),  ss(".")), nilR());

	ps->lang.asAllOf(ss("nsyntaxdef"), list(ss("lexemedef"), ss(".")), nilR());
	ps->lang.asAllOf(ss("nsyntaxdef"), list(ss("assocdef"),  ss(".")), nilR());
	ps->lang.asAllOf(ss("nsyntaxdef"), list(ss("nruledef"),  ss(".")), nilR());

	// allow new lexemes to be introduced
	ps->lang.asAllOf(ss("lexemedef"), list(ss("lexeme"), ss("id"), ss("="), ss("lexbinds"), ss("->"), ss("reduction")), newp<evalLexemeDef>(ps));
	ps->lang.asAllOf(ss("lexemedef"), list(ss("lexeme"), ss("id"), ss("="), ss("lexbinds")),                            newp<evalLexemeDefUnit>(ps));

	ps->lang.asAllOf(ss("lexbinds"), list(ss("lexbinds"), ss("lexbind")), newp<accumLexBinds>(ps));
	ps->lang.asAllOf(ss("lexbinds"), list(ss("lexbind")),                 newp<makeLexBinds>(ss("lexbinds"), ps));

	ps->lang.asAllOf(ss("lexbind"), list(ss("id"), ss(":"), ss("regex")), newp<makeLexBind>(ss("lexbind"), ps));
	ps->lang.asAllOf(ss("lexbind"), list(ss("regex")),                    newp<makeAnonLexBind>(ss("lexbind"), ps));

	// allow lexemes to be given declared associativity
	ps->lang.asAllOf(ss("assocdef"),  list(ss("assocl"), ss("id")),                         newp<evalAssocDef>   (ps, true));
	ps->lang.asAllOf(ss("assocdef"),  list(ss("assocl"), ss("id"), ss("before"), ss("id")), newp<evalRelAssocDef>(ps, true));
	ps->lang.asAllOf(ss("assocdef"),  list(ss("assocr"), ss("id")),                         newp<evalAssocDef>   (ps, false));
	ps->lang.asAllOf(ss("assocdef"),  list(ss("assocr"), ss("id"), ss("before"), ss("id")), newp<evalRelAssocDef>(ps, false));

	// allow new parse rules to be introduced
	ps->lang.asAllOf(ss("truledef"), list(ss("rule"), ss("id"), ss("="), ss("rulebinds"), ss("->"), ss("reduction")), newp<evalRuleDef>(ps, false));
	ps->lang.asAllOf(ss("nruledef"), list(ss("rule"), ss("id"), ss("="), ss("rulebinds"), ss("->"), ss("reduction")), newp<evalRuleDef>(ps, true));

	ps->lang.asAllOf(ss("rulebinds"), list(ss("rulebinds"), ss("rulebind")), newp<accumRuleBinds>(ss("rulebinds"), ps));
	ps->lang.asAllOf(ss("rulebinds"), terminals(),                           newp<makeEmptyRuleBinds>(ss("rulebinds"), ps));

	ps->lang.asAllOf(ss("rulebind"), list(ss("id"), ss(":"), ss("id")), newp<makeRuleBind>(ss("rulebind"), ps));
	ps->lang.asAllOf(ss("rulebind"), list(ss("id")),                    newp<makeAnonRuleBind>(ss("rulebind"), ps));

	// decide how to reduce a lexeme or rule
	ps->lang.asAllOf(ss("reduction"), list(ss("rapp")),              newp<makeReduction>(ss("reduction"), ps));
	ps->lang.asAllOf(ss("rapp"),      list(ss("rapp"), ss("value")), newp<accumRApp>(ss("rapp"), ps));
	ps->lang.asAllOf(ss("rapp"),      list(ss("value")),             newp<makeRApp>(ss("rapp"), ps));

	ps->lang.asAllOf(ss("value"), list(ss("id")), newp<makeVarRef>(ss("value"), ps));

	// productions evaluate to themselves unless otherwise noted
	ps->lang.translatePrimitive(newp<IdP>());

	// now we have enough to make the parser
	ps->parser = ps->lang.compile();
}

}

