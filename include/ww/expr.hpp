
/*
 * expr : manage expression definitions and bindings
 */

#ifndef WW_EXPR_HPP_INCLUDED
#define WW_EXPR_HPP_INCLUDED

#include <ww/type.hpp>

#include <ww/data.hpp>
#include <ww/terminal.hpp>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <iostream>

namespace ww {

/*
 * expressions for constructing actions
 */
struct Expr {
  virtual ~Expr() { }
  virtual void show(std::ostream&) const = 0;
};
typedef shared_ptr<Expr>     ExprPtr;
typedef std::vector<ExprPtr> Exprs;

class Var : public Expr {
public:
  Var(const std::string& n) : n(n) { }
  const std::string& value() const { return this->n; }

  void show(std::ostream& out) const {
    out << this->n;
  }
private:
  std::string n;
};

class App : public Expr {
public:
  App(const Exprs& es) : es(es) { }
  const Exprs& value() const { return this->es; }
  Exprs& value() { return this->es; }

  void show(std::ostream& out) const {
    out << "(";
    if (this->es.size() > 0) {
      this->es[0]->show(out);
      for (unsigned int i = 1; i < this->es.size(); ++i) {
        out << " ";
        this->es[i]->show(out);
      }
    }
    out << ")";
  }
private:
  Exprs es;
};

template <typename T>
  struct switchExpr {
    virtual T with(const Var*) const = 0;
    virtual T with(const App*) const = 0;
  };

template <typename T>
  T switchOf(const Expr& e, const switchExpr<T>& f) {
    if (const Var* v = dynamic_cast<const Var*>(&e)) {
      return f.with(v);
    } else if (const App* v = dynamic_cast<const App*>(&e)) {
      return f.with(v);
    } else {
      throw std::runtime_error("Internal error, can't switch on unknown term");
    }
  }

template <typename T> T switchOf(const Expr*    e, const switchExpr<T>& f) { return switchOf(*e, f); }
template <typename T> T switchOf(const ExprPtr& e, const switchExpr<T>& f) { return switchOf(*e, f); }

template <typename T>
  std::vector<T> switchOf(const Exprs& es, const switchExpr<T>& f) {
    std::vector<T> result;
    for (Exprs::const_iterator e = es.begin(); e != es.end(); ++e) {
      result.push_back(switchOf(**e, f));
    }
    return result;
  }

// determine the type of an expression
struct typeOfF : public switchExpr<TypePtr> {
  TypeEnvPtr tenv;

  typeOfF(const TypeEnvPtr& tenv) : tenv(tenv) {
  }

  TypePtr with(const Var* v) const {
    return this->tenv->lookup(v->value());
  }

  TypePtr with(const App* v) const {
    if (const Fn* fty = dynamic_cast<const Fn*>(switchOf(v->value()[0], *this).get())) {
      Types ats = switchOf(Exprs(v->value().begin() + 1, v->value().end()), *this);
      if (ats.size() != fty->arguments().size()) {
        throw std::runtime_error("Function argument length mismatch.");
      } else {
        for (unsigned int i = 0; i < ats.size(); ++i) {
          if (!(*ats[i] == *fty->arguments()[i])) {
            throw std::runtime_error("Function argument type mismatch.");
          }
        }
        return fty->result();
      }
    } else {
      throw std::runtime_error("Cannot apply non-function type.");
    }
  }
};

inline TypePtr typeOf(const TypeEnvPtr& tenv, const ExprPtr& e) {
  return switchOf(e, typeOfF(tenv));
}

// keep track of variable:value bindings for evaluation
class ValueEnv {
public:
  ValueEnv()                              : p()  { }
  ValueEnv(const shared_ptr<ValueEnv>& p) : p(p) { }

  ValueEnv(const VarPos& vpos, const terminals& ts) : p() {
    define(vpos, ts);
  }

  ValueEnv(const VarPos& vpos, const terminals& ts, const shared_ptr<ValueEnv> p) : p(p) {
    define(vpos, ts);
  }

  terminal* lookup(const VarName& vn) const {
    VEnv::const_iterator v = this->venv.find(vn);
    if (v != this->venv.end()) {
      return v->second;
    } else if (this->p.get() != 0) {
      return this->p->lookup(vn);
    } else {
      throw std::runtime_error("Undefined variable in reduction rule: " + vn);
    }
  }

  void define(const VarName& vn, terminal* v) {
    this->venv[vn] = v;
  }

  void define(const VarPos& vpos, const terminals& ts) {
    for (VarPos::const_iterator vp = vpos.begin(); vp != vpos.end(); ++vp) {
      if (vp->first != "_") {
        define(vp->first, ts[vp->second]);
      }
    }
  }
private:
  const shared_ptr<ValueEnv> p;

  typedef std::map<VarName, terminal*> VEnv;
  VEnv venv;
};
typedef shared_ptr<ValueEnv> ValueEnvPtr;

}

#endif
