/*
 * parser : an interface for 'parsing machines'
 */

#ifndef WW_PARSER_HPP_INCLUDED
#define WW_PARSER_HPP_INCLUDED

#include <ww/terminal.hpp>
#include <ww/stream.hpp>
#include <ww/data.hpp>
#include <vector>
#include <stdexcept>

namespace ww {

// a parser is a state machine that can be
//  * given input
//  * queried for its control states
//  * reset
//  * asked to produce a value (if it's in a completion state)
template <typename T>
	struct parser {
		typedef shared_ptr< parser<T> > ptr;
		typedef std::vector<ptr>        seq;

		// reset the parser to its initial state
		virtual void reset() = 0;

		// is the parser looking for the given terminal next?
		virtual bool expects(terminal*) const = 0;

		// just what _is_ the parser looking for right now?
		virtual terminals expecting() const = 0;

		// transition to the next state from this terminal, given a position in the input sequence
		virtual void transition(nat, terminal*) = 0;

		// if a parser was given a wrong terminal somewhere, it'll get stuck in a failure state
		virtual bool failed() const = 0;

		// if a parser accepts, then we can get out a completed value
		virtual bool accepts() const = 0;
		virtual T    reduce() = 0;
	};

// an 'any' parallel parser will apply input to a sequence of parsers, accepting if any parser accepts
template <typename T>
	class parse_any : public parser<T> {
	public:
		typedef shared_ptr< parser<T> > ptr;
		typedef std::vector<ptr>        seqv;
		typedef shared_ptr<seqv>        seq;

		parse_any(const seq& ps) : ps(ps) {
		}

		void reset() {
			for (typename seqv::const_iterator p = this->ps->begin(); p != this->ps->end(); ++p) {
				(*p)->reset();
			}
		}

		bool expects(terminal* t) const {
			for (typename seqv::const_iterator p = this->ps->begin(); p != this->ps->end(); ++p) {
				if ((*p)->expects(t)) {
					return true;
				}
			}
			return false;
		}

		terminals expecting() const {
			terminals result;
			for (typename seqv::const_iterator p = this->ps->begin(); p != this->ps->end(); ++p) {
				terminals ts = (*p)->expecting();
				result.insert(result.end(), ts.begin(), ts.end());
			}
			return result;
		}

		void transition(nat c, terminal* t) {
			for (typename seqv::const_iterator p = this->ps->begin(); p != this->ps->end(); ++p) {
				(*p)->transition(c, t);
			}
		}

		bool failed() const {
			for (typename seqv::const_iterator p = this->ps->begin(); p != this->ps->end(); ++p) {
				if (!(*p)->failed()) {
					return false;
				}
			}
			return true;
		}

		bool accepts() const {
			for (typename seqv::const_iterator p = this->ps->begin(); p != this->ps->end(); ++p) {
				if ((*p)->accepts()) {
					return true;
				}
			}
			return false;
		}

		T reduce() {
			for (typename seqv::const_iterator p = this->ps->begin(); p != this->ps->end(); ++p) {
				if ((*p)->accepts()) {
					return (*p)->reduce();
				}
			}
			throw std::runtime_error("Internal error, reduce called on parser that does not accept");
		}
	private:
		seq ps;
	};

// we find the longest match for a parser by running it until we get a token that it doesn't expect
template <typename T>
	T longest_match(parser<T>& p, buffering_stream<terminal*>& s, nat* c) {
		// read input until we find a token that would cause the parser to fail
		// instead of giving it that, give it eof
		// (this should find the longest match that the parser is capable of)
		terminal* t = endOfFile::value();

		while (!s.end() && !p.accepts() && !p.failed()) {
			t = s.next();

			if (p.expects(t)) {
				p.transition((*c)++, t);
			} else {
				s.putback(t);
				p.transition(*c, endOfFile::value());
				break;
			}
		}

		// we've read up to the last acceptable character, just reduce
		if (p.failed()) {
			throw std::runtime_error("Unexpected '" + show(t) + "' in input (expected one of " + showTerminals(p.expecting()) + ")");
		} else if (!p.accepts()) {
			throw std::runtime_error("Unexpected end of input");
		} else {
			return p.reduce();
		}
	}

// we can make a longest-match recognizer itself a stream of values
template <typename T>
	class longest_match_stream : public stream<T> {
	public:
		typedef typename parser<T>::ptr           parserp;
		typedef typename buffering_stream<T>::ptr bstreamp;

		longest_match_stream(const parserp& p, const bstreamp& b) : p(p), b(b), c(0) {
		}

		T next() {
			this->p->reset();
			return longest_match<T>(*this->p, *this->b, &this->c);
		}

		bool end() const {
			return this->b->end();
		}
	private:
		parserp  p;
		bstreamp b;
		nat      c;
	};

}

#endif

