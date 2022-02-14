#ifndef WW_STREAM_HPP_INCLUDED
#define WW_STREAM_HPP_INCLUDED

#include <ww/data.hpp>
#include <vector>
#include <stack>
#include <iostream>

namespace ww {

// an interface for 'lazy streams'
template <typename T>
	struct stream {
		typedef shared_ptr< stream<T> > ptr;
    virtual ~stream() { }

		virtual T next() = 0;
		virtual bool end() const = 0;
	};

// a buffering stream (allows putting values back)
template <typename T>
	class buffering_stream {
	public:
		typedef typename stream<T>::ptr           S;
		typedef shared_ptr< buffering_stream<T> > ptr;
		typedef std::stack<T>                     buffer;

		buffering_stream(const S& s) : s(s) {
		}

		void putback(T t) {
			this->b.push(t);
		}

		T next() {
			if (this->b.size() > 0) {
				T r = this->b.top();
				this->b.pop();
				return r;
			} else {
				return this->s->next();
			}
		}

		bool end() const {
			return this->b.size() == 0 && this->s->end();
		}
	private:
		S      s;
		buffer b;
	};

// common streams
template <typename T>
	class append : public stream<T> {
	public:
		typedef typename stream<T>::ptr S;

		append(const S& s1, const S& s2) : s1(s1), s2(s2), f(s1->end()) {
		}

		T next() {
			if (this->f) {
				return this->s2->next();
			} else {
				T x = this->s1->next();
				this->f = this->s1->end();
				return x;
			}
		}

		bool end() const {
			return this->f && this->s2->end();
		}
	private:
		S    s1;
		S    s2;
		bool f;
	};

template <typename A, typename B>
	struct function {
    virtual ~function() { }
		typedef shared_ptr< function<A, B> > ptr;

		virtual B apply(A) = 0;
	};

template <typename T>
	struct id : public function<T, T> {
		T apply(T x) {
			return x;
		}
	};

template <typename T>
	struct eq : public function<T, bool> {
		T x;

		eq(const T& x) : x(x) {
		}

		bool apply(T y) {
			return this->x == y;
		}
	};

template <typename T>
	class fnot : public function<T, bool> {
	public:
		typedef typename function<T, bool>::ptr F;

		fnot(const F& f) : f(f) {
		}

		bool apply(T x) {
			return !this->f->apply(x);
		}
	private:
		F f;
	};

template <typename A, typename B>
	class map : public stream<B> {
	public:
		typedef typename function<A, B>::ptr F;
		typedef typename stream<A>::ptr S;

		map(const F& f, const S& s) : f(f), s(s) {
		}

		B next() {
			return this->f->apply(this->s->next());
		}

		bool end() const {
			return this->s->end();
		}
	private:
		F f;
		S s;
	};

template <typename T>
	class filter : public stream<T> {
	public:
		typedef typename function<T, bool>::ptr P;
		typedef typename stream<T>::ptr S;

		filter(const P& p, const S& s) : p(p), s(s) {
			findNext();
		}

		T next() {
			T r(this->x);
			findNext();
			return r;
		}

		bool end() const {
			return this->e;
		}
	private:
		P p;
		S s;
		T x;
		bool e;

		void findNext() {
			while (!this->s->end()) {
				this->x = this->s->next();
				if (this->p->apply(this->x)) {
					this->e = false;
					return;
				}
			}
			this->e = true;
		}
	};

template <typename T>
	class lazyFilter : public stream<T> {
	public:
		typedef typename function<T, bool>::ptr P;
		typedef typename stream<T>::ptr S;

		lazyFilter(const P& p, const S& s, T d) : p(p), s(s), d(d) {
		}

		T next() {
			while (!this->s->end()) {
				T x = this->s->next();
				if (this->p->apply(x)) {
					return x;
				}
			}
			return this->d;
		}

		bool end() const {
			return this->s->end();
		}
	private:
		P p;
		S s;
		T d;
	};

template <typename T>
	class takeWhile : public stream<T> {
	public:
		typedef function<T, bool> P;
		typedef typename stream<T>::ptr S;

		takeWhile(const P& p, const S& s) : p(p), s(s) {
			fetch();
		}

		T next() {
			T r = this->x;
			fetch();
			return r;
		}

		bool end() const {
			return this->done;
		}
	private:
		P    p;
		S    s;
		T    x;
		bool done;

		void fetch() {
			if (this->s->end()) {
				this->done = true;
			} else {
				this->x    = this->s->next();
				this->done = !this->p->apply(this->x);
			}
		}
	};

template <typename T>
	class vec : public stream<T> {
	public:
		vec(const std::vector<T>& xs) : xs(xs), i(0) {
		}

		template <typename TIter>
			vec(TIter begin, TIter end) : xs(begin, end), i(0) {
			}

		T next() {
			return this->xs[this->i++];
		}

		bool end() const {
			return this->i >= this->xs.size();
		}
	private:
		std::vector<T> xs;
		nat            i;
	};

template <typename C>
	class str : public stream<C> {
	public:
		str(const std::basic_string<C>& cs) : cs(cs), i(0) {
		}

		C next() {
			return this->cs[this->i++];
		}

		bool end() const {
			return this->i >= this->cs.size();
		}
	private:
		std::basic_string<C> cs;
		nat                  i;
	};

template <typename T>
	class once : public stream<T> {
	public:
		once(const T& x) : x(x), f(false) {
		}

		T next() {
			this->f = true;
			return this->x;
		}

		bool end() const {
			return this->f;
		}
	private:
		T    x;
		bool f;
	};

template <typename T>
	class repeat : public stream<T> {
	public:
		repeat(const T& x) : x(x) {
		}

		T next() {
			return this->x;
		}

		bool end() const {
			return false;
		}
	private:
		T x;
	};

template <typename T>
	class range : public stream<T> {
	public:
		range(const T& x0, const T& x1) : x(x0), xf(x1), inf(false) {
		}

		range(const T& x0) : x(x0), inf(true) {
		}

		T next() {
			T r = this->x;
			++this->x;
			return r;
		}

		bool end() const {
			return !this->inf && this->x == this->xf;
		}
	private:
		T    x;
		T    xf;
		bool inf;
	};

template <typename T>
	class iterate : public stream<T> {
	public:
		typedef typename function<T, T>::ptr F;
		typedef typename stream<T>::ptr S;

		iterate(const F& f, const T& x, const S& s) : f(f), s(s) {
		}

		T next() {
			T r = this->x;
			this->x = this->f->apply(this->x);
			return r;
		}

		bool end() const {
			return false;
		}
	private:
		F f;
		T x;
		S s;
	};

template <typename C>
	class istream : public stream<C> {
	public:
		typedef shared_ptr< std::basic_istream<C> > S;

		istream(const S& s) : s(s) {
		}

		C next() {
			C c;
			this->s->get(c);
			return c;
		}

		bool end() const {
			return this->s->eof() || this->s->bad();
		}
	private:
		S s;
	};

class trackpos : public stream<char> {
public:
	typedef linedb::ptr       LDB;
	typedef stream<char>::ptr S;

	trackpos(const S& s, const LDB& ldb);
	const LDB& lineDB() const;

	char next();
	bool end() const;
private:
	S   s;
	LDB ldb;
};

// stream utilities
template <typename T>
	std::vector<T> slurp(const stream<T>& s) {
		std::vector<T> r;
		while (!s.end()) {
			r.push_back(s.next());
		}
		return r;
	}

template <typename T>
	std::vector<T> slurp(const typename stream<T>::ptr& s) {
		return slurp(*s);
	}

}

#endif
