<h1>ww : a "lexerless" LALR(1) parser generator allowing dynamic grammar extension</h1>
<h2>(named for Walt Whitman, the great Manish Tripathi's favorite poet)</h2>

> "Do I contradict myself?  Very well, then, I contradict myself." -- *Walt Whitman*

This attitude is completely bonkers.  We only want to admit self-consistent programs, and by extension efficient, deterministic parsers for _unambiguous_ grammars.

Still, a little dynamism can be good, and all of the machinery of flex/bison (or lex/yacc) can get tedious/repetitive -- even if the produced LALR(1) parsers are very efficient.  Why do we need a lexer anyway?  Aren't regular expressions just a subset of context-free grammars?  Yes, yes they are.

So, building on the efficient LALR(1) determination of DeRemer and Pennello[1], we can build two-level parsers, where the first level (the "lexer") is an LALR(1) parser on characters and the second is an LALR(1) parser on lexemes.  This is logically still just the traditional setup, but simplified by not duplicating machinery.

But we can still do better.  Traditional LALR(1) tools like 'bison' run in a "batch mode" on your grammar, and if there are errors in your grammar you are just told "there were errors".  However, if we view a set of grammar rules as the incremental construction of a language, then we can recompile our parser at each step and know immediately where ambiguities are introduced -- pointing to a specific file/line and explaining with a simplified view of the errant parse state (showing just the production items that would be shift/reduce or reduce/reduce conflicts).  This can make grammar debugging much simpler.

Finally, since we're already viewing grammar rules as the incremental construction of a parser, we can embed such rules _in the language to parse_ and so allow for dynamically reconfigurable (and efficient!) parsers.

For example, here's a file (taken from test/peval/test.ww) that initially starts with the least grammar (which only allows you to introduce lexer/parser rules) and then incrementally adds syntax for comments and arithmetic expressions, and then introduces an arithmetic expression:

```
syntax lexeme w = '//[^\n]*\n' -> null.

// now that we can write comments -- extend this grammar to accept arithmetic expressions
syntax {
	assocl p.
	lexeme p = "+".
	
	assocl t before p.
	lexeme t = "*".
	
	assocl e before t.
	lexeme e = "^".
	
	lexeme int       = x:'[0-9]+'      -> toInt x.
	rule   expr      = x:int           -> x.
	rule   expr      = x:expr p y:expr -> plus x y.
	rule   expr      = x:expr t y:expr -> times x y.
	rule   expr      = x:expr e y:expr -> power x y.
	rule   statement = e:expr          -> e.
}

// with that out of the way, let us now do a little arithmetic
1 + 3^2*2
```

References:
[1] - http://3e8.org/pub/scheme/doc/parsing/Efficient%20Computation%20of%20LALR(1)%20Look-Ahead%20Sets.pdf
