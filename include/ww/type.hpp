
/*
 * type : manage type definitions and bindings
 */

#ifndef WW_TYPE_HPP_INCLUDED
#define WW_TYPE_HPP_INCLUDED

#include <ww/data.hpp>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <iostream>

namespace ww {

typedef std::string                     VarName; // the name of a user-defined variable
typedef std::map<VarName, unsigned int> VarPos;  // variable->position mappings for lexeme/rule-defs

/*
 * types for values referenced in syntax lexemes/rules/actions
 */
struct Type {
  virtual ~Type() { }
  virtual void show(std::ostream&)     const = 0;
  virtual bool operator==(const Type&) const = 0;
};
typedef shared_ptr<Type> TypePtr;
typedef std::vector<TypePtr> Types;

class CType : public Type {
public:
  CType(const std::string& ctype) : ctype(ctype) { }
  const std::string& value() const { return this->ctype; }

  void show(std::ostream& out) const {
    out << this->ctype;
  }

  bool operator==(const Type& rhs) const {
    if (const CType* t = dynamic_cast<const CType*>(&rhs)) {
      return this->ctype == t->ctype;
    } else {
      return false;
    }
  }
private:
  std::string ctype;
};

class Fn : public Type {
public:
  Fn(const Types& args, const TypePtr& r) : args(args), r(r) { }

  const Types&   arguments() const { return this->args; }
  const TypePtr& result()    const { return this->r; }

  void show(std::ostream& out) const {
    out << "(";
    if (this->args.size() > 0) {
      this->args[0]->show(out);
      for (unsigned int i = 1; i < this->args.size(); ++i) {
        out << ", ";
        this->args[i]->show(out);
      }
    }
    out << ") -> ";
    this->r->show(out);
  }

  bool operator==(const Type& rhs) const {
    const Fn* trhs = dynamic_cast<const Fn*>(&rhs);
    if (!trhs || trhs->args.size() != this->args.size()) return false;

    for (unsigned int i = 0; i < this->args.size(); ++i) {
      if (!(*this->args[i] == *trhs->args[i])) {
        return false;
      }
    }

    return *this->r == *trhs->r;
  }
private:
  Types   args;
  TypePtr r;
};

template <typename T>
  struct switchType {
    virtual T with(const CType*) const = 0;
    virtual T with(const Fn*)    const = 0;
  };

template <typename T>
  T switchOf(const Type& t, const switchType<T>& f) {
    if (const CType* v = dynamic_cast<const CType*>(&t)) {
      return f.with(v);
    } else if (const Fn* v = dynamic_cast<const Fn*>(&t)) {
      return f.with(v);
    } else {
      throw std::runtime_error("Internal error, can't switch on unknown type");
    }
  }

template <typename T> T switchOf(const Type*    t, const switchType<T>& f) { return switchOf(*t, f); }
template <typename T> T switchOf(const TypePtr& t, const switchType<T>& f) { return switchOf(*t, f); }

template <typename T>
  std::vector<T> switchOf(const Types& ts, const switchType<T>& f) {
    std::vector<T> result;
    for (Types::const_iterator t = ts.begin(); t != ts.end(); ++t) {
      result.push_back(switchOf(*t, f));
    }
    return result;
  }

// keep track of variable:type bindings for type checking
class TypeEnv {
public:
  TypeEnv()                            : p()  { }
  TypeEnv(const shared_ptr<TypeEnv> p) : p(p) { }

  void define(const VarName& vn, const TypePtr& t) {
    this->tenv[vn] = t;
  }

  bool hasBinding(const VarName& vn) const {
    return (this->tenv.find(vn) != this->tenv.end()) || (this->p && this->p->hasBinding(vn));
  }

  TypePtr lookup(const VarName& vn) const {
    TEnv::const_iterator t = this->tenv.find(vn);
    if (t != this->tenv.end()) {
      return t->second;
    } else if (this->p.get() != 0) {
      return this->p->lookup(vn);
    } else {
      throw std::runtime_error("Undefined variable: " + vn);
    }
  }
private:
  const shared_ptr<TypeEnv> p;

  typedef std::map<VarName, TypePtr> TEnv;
  TEnv tenv;
};
typedef shared_ptr<TypeEnv> TypeEnvPtr;

// type construction utilities
inline TypePtr fn(const Types& args, const TypePtr& r) {
  return TypePtr(new Fn(args, r));
}

inline TypePtr cty(const std::string& n) {
  return TypePtr(new CType(n));
}

inline Types ctys(const std::vector<std::string>& ns) {
  Types result;
  for (std::vector<std::string>::const_iterator n = ns.begin(); n != ns.end(); ++n) {
    result.push_back(cty(*n));
  }
  return result;
}

template <typename T>
  TypePtr liftty() {
    return cty(typeName<T>());
  }

}

#endif
