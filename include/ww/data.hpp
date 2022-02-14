
#ifndef WW_DATAUTIL_HPP_INCLUDED
#define WW_DATAUTIL_HPP_INCLUDED

#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <stdexcept>
#include <typeinfo>
#include <cxxabi.h>

#ifdef USE_BOOST
#include <boost/shared_ptr.hpp>
#else
#include <memory>
#endif

namespace ww {

typedef unsigned int nat;
typedef std::vector<nat> nats;

typedef bool Unit;
extern const bool unit;

#ifdef USE_BOOST
using boost::shared_ptr;
#else
using std::shared_ptr;
#endif

template <typename T>
	std::string typeName() {
		int status = 0;
		char* dname = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
		std::string ret = dname ? dname : "";
		free(dname);
		return ret;
	}

template <typename T>
	std::string toString(T x) {
		std::ostringstream ss;
		ss << x;
		return ss.str();
	}

template <typename T>
	T fromString(const std::string& x) {
		std::istringstream ss(x);
		T r;
		ss >> r;
		return r;
	}

template <typename T>
	std::string show(const T* t) {
		std::ostringstream ss;
		t->show(ss);
		return ss.str();
	}

template <typename T>
	std::string show(const std::vector<T*>& ts) {
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

template <typename T>
	void pop(std::stack<T>& s, nat n) {
		for (nat i = 0; i < n; ++i) {
			if (s.size() == 0) {
				throw std::runtime_error("Internal error, stack underflow");
			} else {
				s.pop();
			}
		}
	}

template <typename T>
	std::vector<T> popv(std::stack<T>& s, nat n) {
		if (s.size() < n) {
			pop(s, s.size());
			throw std::runtime_error("Internal error, stack underflow");
		}

		std::vector<T> r;
		r.resize(n);
		for (nat i = 0; i < n; ++i) {
			r[(n-1) - i] = s.top();
			s.pop();
		}
		return r;
	}

// a little bit of state to map a sequence index to line numbers
// we just watch characters and record newlines
class linedb {
public:
	typedef shared_ptr<linedb>  ptr;
	typedef std::pair<nat, nat> LineCol;

	linedb(nat stype, const std::string& sdesc);

	nat                sourceType() const;
	const std::string& sourceDesc() const;

	void step(char x);
	void reset();

	LineCol pos(nat i) const;
private:
	nat         stype;
	std::string sdesc;
	nat         c;
	nats        lp;    // sequence positions corresponding to line index
};

// load a set of lines around an implicated set from the specified line database
typedef std::vector<std::string> strings;
typedef std::pair<nats, strings> ldblines;

ldblines load(const linedb::ptr& ldb, const linedb::LineCol& i, const linedb::LineCol& f);

// make it easier to allocate types in shared pointers
template <typename N>
	typename N::ptr newp() {
		return typename N::ptr(new N());
	}

template <typename N, typename A0>
	typename N::ptr newp(A0 a0) {
		return typename N::ptr(new N(a0));
	}

template <typename N, typename A0, typename A1>
	typename N::ptr newp(A0 a0, A1 a1) {
		return typename N::ptr(new N(a0, a1));
	}

template <typename N, typename A0, typename A1, typename A2>
	typename N::ptr newp(A0 a0, A1 a1, A2 a2) {
		return typename N::ptr(new N(a0, a1, a2));
	}

template <typename N, typename A0, typename A1, typename A2, typename A3>
	typename N::ptr newp(A0 a0, A1 a1, A2 a2, A3 a3) {
		return typename N::ptr(new N(a0, a1, a2, a3));
	}

template <typename N, typename A0, typename A1, typename A2, typename A3, typename A4>
	typename N::ptr newp(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {
		return typename N::ptr(new N(a0, a1, a2, a3, a4));
	}

template <typename N, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5>
	typename N::ptr newp(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {
		return typename N::ptr(new N(a0, a1, a2, a3, a4, a5));
	}

template <typename N, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	typename N::ptr newp(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) {
		return typename N::ptr(new N(a0, a1, a2, a3, a4, a5, a6));
	}

template <typename N, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	typename N::ptr newp(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7) {
		return typename N::ptr(new N(a0, a1, a2, a3, a4, a5, a6, a7));
	}

// see if a value is in a set
template <typename T>
	bool in(T x, const std::set<T>& xs) {
		return xs.find(x) != xs.end();
	}

// see if a value is in an ordered sequence (less optimal than set lookup, obviously)
template <typename T>
	bool in(T x, const std::vector<T>& xs) {
		for (typename std::vector<T>::const_iterator xi = xs.begin(); xi != xs.end(); ++xi) {
			if (x == *xi) {
				return true;
			}
		}
		return false;
	}

// extract the keys from a map
template <typename K, typename T>
	std::set<K> keys(const std::map<K, T>& m) {
		std::set<K> r;
		for (typename std::map<K, T>::const_iterator e = m.begin(); e != m.end(); ++e) {
			r.insert(e->first);
		}
		return r;
	}

// extract the values from a map
template <typename K, typename T>
	std::vector<T> values(const std::map<K, T>& m) {
		std::vector<T> r;
		for (typename std::map<K, T>::const_iterator e = m.begin(); e != m.end(); ++e) {
			r.push_back(e->second);
		}
		return r;
	}

// set functions (destructive or not)
template <typename K>
	void setUnion(std::set<K>* out, const std::set<K>& xs) {
		out->insert(xs.begin(), xs.end());
	}

template <typename K>
	std::set<K> setUnion(const std::set<K>& lhs, const std::set<K>& rhs) {
		std::set<K> r(lhs);
		setUnion(&r, rhs);
		return r;
	}

template <typename K>
	std::set<K> setDifference(const std::set<K>& lhs, const std::set<K>& rhs) {
		std::set<K> r;
		for (typename std::set<K>::const_iterator v = lhs.begin(); v != lhs.end(); ++v) {
			if (!in(*v, rhs)) {
				r.insert(*v);
			}
		}
		return r;
	}

template <typename K, typename V>
	V mapLookup(const std::map<K, V>& m, const K& k) {
		typedef typename std::map<K, V>::const_iterator CI;
		CI i = m.find(k);
		if (i == m.end()) {
			throw std::runtime_error("Unexpected map index undefined");
		} else {
			return i->second;
		}
	}

// container conversion
inline std::set<char> toSet(const std::string& xs) {
	std::set<char> r;
	for (std::string::const_iterator c = xs.begin(); c != xs.end(); ++c) {
		r.insert(*c);
	}
	return r;
}

inline std::string fromSet(const std::set<char>& xs) {
	std::ostringstream r;
	for (std::set<char>::const_iterator c = xs.begin(); c != xs.end(); ++c) {
		r << *c;
	}
	return r.str();
}

// make an array
template <typename T>
	std::vector<T> add(const std::vector<T>& xs, T x) {
		std::vector<T> r(xs);
		r.push_back(x);
		return r;
	}

template <typename T>
	std::vector<T> list() { return std::vector<T>(); }

template <typename T>
	std::vector<T> list(T t0) { return add(list<T>(), t0); }

template <typename T>
	std::vector<T> list(T t0, T t1) { return add(list(t0), t1); }

template <typename T>
	std::vector<T> list(T t0, T t1, T t2) { return add(list(t0, t1), t2); }

template <typename T>
	std::vector<T> list(T t0, T t1, T t2, T t3) { return add(list(t0, t1, t2), t3); }

template <typename T>
	std::vector<T> list(T t0, T t1, T t2, T t3, T t4) { return add(list(t0, t1, t2, t3), t4); }

template <typename T>
	std::vector<T> list(T t0, T t1, T t2, T t3, T t4, T t5) { return add(list(t0, t1, t2, t3, t4), t5); }

template <typename T>
	std::vector<T> list(T t0, T t1, T t2, T t3, T t4, T t5, T t6) { return add(list(t0, t1, t2, t3, t4, t5), t6); }

template <typename T>
	std::vector<T> list(T t0, T t1, T t2, T t3, T t4, T t5, T t6, T t7) { return add(list(t0, t1, t2, t3, t4, t5, t6), t7); }

template <typename T>
	std::vector<T> list(T t0, T t1, T t2, T t3, T t4, T t5, T t6, T t7, T t8) { return add(list(t0, t1, t2, t3, t4, t5, t6, t7), t8); }

template <typename T>
	std::vector<T> list(T t0, T t1, T t2, T t3, T t4, T t5, T t6, T t7, T t8, T t9) { return add(list(t0, t1, t2, t3, t4, t5, t6, t7, t8), t9); }

template <typename T>
	std::vector<T> list(T t0, T t1, T t2, T t3, T t4, T t5, T t6, T t7, T t8, T t9, T t10) { return add(list(t0, t1, t2, t3, t4, t5, t6, t7, t8, t9), t10); }

// flatten a sequence of arrays
template <typename CT>
	struct flatten {
		static CT seq(const std::vector<CT>& cs) {
			CT r;
			for (typename std::vector<CT>::const_iterator c = cs.begin(); c != cs.end(); ++c) {
				r.insert(r.end(), c->begin(), c->end());
			}
			return r;
		}
	};

template <>
	struct flatten<std::string> {
		static std::string seq(const std::vector<std::string>& cs) {
			std::ostringstream ss;
			for (std::vector<std::string>::const_iterator c = cs.begin(); c != cs.end(); ++c) {
				ss << *c;
			}
			return ss.str();
		}
	};

template <typename CT>
	CT concat(const std::vector<CT>& cs) {
		return flatten<CT>::seq(cs);
	}

// make a set
template <typename T>
	std::set<T> set(const std::vector<T>& xs) {
		std::set<T> r;
		for (typename std::vector<T>::const_iterator x = xs.begin(); x != xs.end(); ++xs) {
			r.insert(*x);
		}
		return r;
	}

// char set utilities
inline std::string charRange(char low, char high) {
	std::ostringstream ss;
	if (low < high) {
		for (char c = low; c <= high; ++c) {
			ss << c;
		}
	} else {
		for (char c = low; c >= high; --c) {
			ss << c;
		}
	}
	return ss.str();
}

inline std::string printableChars() {
	return charRange(0x20, 0x7e);
}

inline std::string difference(const std::string& x, const std::string& y) {
	return fromSet(setDifference(toSet(x), toSet(y)));
}

}

#endif

