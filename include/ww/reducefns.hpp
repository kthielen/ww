

/*
 * NOTE: This is a generated file meant to be included inline -- it shouldn't be included explicitly or modified
 */


template < typename R >
    class reducefn0 : public reducefn {
    public:
        typedef R (*action)();
        reducefn0(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			return j.apply< R >(this->cb());
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< R >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };

template < typename R >
	reducefnp reduceWith(R(*cb)()) {
		return reducefnp(new reducefn0< R >(cb));
    }


template <  >
    class reducefn0<void> : public reducefn {
    public:
        typedef void (*action)();
        reducefn0(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			this->cb();
			return 0;
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< void >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };


template < typename R, typename A0 >
    class reducefn1 : public reducefn {
    public:
        typedef R (*action)(A0);
        reducefn1(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			return j.apply< R >(this->cb(unwrap<A0>::value(xs[0])));
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< R >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };

template < typename R, typename A0 >
	reducefnp reduceWith(R(*cb)(A0)) {
		return reducefnp(new reducefn1< R,A0 >(cb));
    }


template < typename A0 >
    class reducefn1<void, A0> : public reducefn {
    public:
        typedef void (*action)(A0);
        reducefn1(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			this->cb(unwrap<A0>::value(xs[0]));
			return 0;
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< void >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };


template < typename R, typename A0, typename A1 >
    class reducefn2 : public reducefn {
    public:
        typedef R (*action)(A0, A1);
        reducefn2(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			return j.apply< R >(this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1])));
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< R >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };

template < typename R, typename A0, typename A1 >
	reducefnp reduceWith(R(*cb)(A0,A1)) {
		return reducefnp(new reducefn2< R,A0,A1 >(cb));
    }


template < typename A0, typename A1 >
    class reducefn2<void, A0, A1> : public reducefn {
    public:
        typedef void (*action)(A0, A1);
        reducefn2(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]));
			return 0;
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< void >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };


template < typename R, typename A0, typename A1, typename A2 >
    class reducefn3 : public reducefn {
    public:
        typedef R (*action)(A0, A1, A2);
        reducefn3(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			return j.apply< R >(this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2])));
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< R >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };

template < typename R, typename A0, typename A1, typename A2 >
	reducefnp reduceWith(R(*cb)(A0,A1,A2)) {
		return reducefnp(new reducefn3< R,A0,A1,A2 >(cb));
    }


template < typename A0, typename A1, typename A2 >
    class reducefn3<void, A0, A1, A2> : public reducefn {
    public:
        typedef void (*action)(A0, A1, A2);
        reducefn3(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]));
			return 0;
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< void >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };


template < typename R, typename A0, typename A1, typename A2, typename A3 >
    class reducefn4 : public reducefn {
    public:
        typedef R (*action)(A0, A1, A2, A3);
        reducefn4(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			return j.apply< R >(this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3])));
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< R >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };

template < typename R, typename A0, typename A1, typename A2, typename A3 >
	reducefnp reduceWith(R(*cb)(A0,A1,A2,A3)) {
		return reducefnp(new reducefn4< R,A0,A1,A2,A3 >(cb));
    }


template < typename A0, typename A1, typename A2, typename A3 >
    class reducefn4<void, A0, A1, A2, A3> : public reducefn {
    public:
        typedef void (*action)(A0, A1, A2, A3);
        reducefn4(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]));
			return 0;
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< void >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };


template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4 >
    class reducefn5 : public reducefn {
    public:
        typedef R (*action)(A0, A1, A2, A3, A4);
        reducefn5(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			return j.apply< R >(this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4])));
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< R >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };

template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4 >
	reducefnp reduceWith(R(*cb)(A0,A1,A2,A3,A4)) {
		return reducefnp(new reducefn5< R,A0,A1,A2,A3,A4 >(cb));
    }


template < typename A0, typename A1, typename A2, typename A3, typename A4 >
    class reducefn5<void, A0, A1, A2, A3, A4> : public reducefn {
    public:
        typedef void (*action)(A0, A1, A2, A3, A4);
        reducefn5(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]));
			return 0;
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< void >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };


template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5 >
    class reducefn6 : public reducefn {
    public:
        typedef R (*action)(A0, A1, A2, A3, A4, A5);
        reducefn6(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			return j.apply< R >(this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5])));
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< R >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };

template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5 >
	reducefnp reduceWith(R(*cb)(A0,A1,A2,A3,A4,A5)) {
		return reducefnp(new reducefn6< R,A0,A1,A2,A3,A4,A5 >(cb));
    }


template < typename A0, typename A1, typename A2, typename A3, typename A4, typename A5 >
    class reducefn6<void, A0, A1, A2, A3, A4, A5> : public reducefn {
    public:
        typedef void (*action)(A0, A1, A2, A3, A4, A5);
        reducefn6(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]));
			return 0;
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< void >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };


template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6 >
    class reducefn7 : public reducefn {
    public:
        typedef R (*action)(A0, A1, A2, A3, A4, A5, A6);
        reducefn7(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			return j.apply< R >(this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]), unwrap<A6>::value(xs[6])));
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< R >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			result.push_back(typeName< A6 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };

template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6 >
	reducefnp reduceWith(R(*cb)(A0,A1,A2,A3,A4,A5,A6)) {
		return reducefnp(new reducefn7< R,A0,A1,A2,A3,A4,A5,A6 >(cb));
    }


template < typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6 >
    class reducefn7<void, A0, A1, A2, A3, A4, A5, A6> : public reducefn {
    public:
        typedef void (*action)(A0, A1, A2, A3, A4, A5, A6);
        reducefn7(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]), unwrap<A6>::value(xs[6]));
			return 0;
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< void >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			result.push_back(typeName< A6 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };


template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7 >
    class reducefn8 : public reducefn {
    public:
        typedef R (*action)(A0, A1, A2, A3, A4, A5, A6, A7);
        reducefn8(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			return j.apply< R >(this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]), unwrap<A6>::value(xs[6]), unwrap<A7>::value(xs[7])));
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< R >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			result.push_back(typeName< A6 >());
			result.push_back(typeName< A7 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };

template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7 >
	reducefnp reduceWith(R(*cb)(A0,A1,A2,A3,A4,A5,A6,A7)) {
		return reducefnp(new reducefn8< R,A0,A1,A2,A3,A4,A5,A6,A7 >(cb));
    }


template < typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7 >
    class reducefn8<void, A0, A1, A2, A3, A4, A5, A6, A7> : public reducefn {
    public:
        typedef void (*action)(A0, A1, A2, A3, A4, A5, A6, A7);
        reducefn8(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]), unwrap<A6>::value(xs[6]), unwrap<A7>::value(xs[7]));
			return 0;
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< void >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			result.push_back(typeName< A6 >());
			result.push_back(typeName< A7 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };


template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8 >
    class reducefn9 : public reducefn {
    public:
        typedef R (*action)(A0, A1, A2, A3, A4, A5, A6, A7, A8);
        reducefn9(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			return j.apply< R >(this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]), unwrap<A6>::value(xs[6]), unwrap<A7>::value(xs[7]), unwrap<A8>::value(xs[8])));
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< R >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			result.push_back(typeName< A6 >());
			result.push_back(typeName< A7 >());
			result.push_back(typeName< A8 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };

template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8 >
	reducefnp reduceWith(R(*cb)(A0,A1,A2,A3,A4,A5,A6,A7,A8)) {
		return reducefnp(new reducefn9< R,A0,A1,A2,A3,A4,A5,A6,A7,A8 >(cb));
    }


template < typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8 >
    class reducefn9<void, A0, A1, A2, A3, A4, A5, A6, A7, A8> : public reducefn {
    public:
        typedef void (*action)(A0, A1, A2, A3, A4, A5, A6, A7, A8);
        reducefn9(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]), unwrap<A6>::value(xs[6]), unwrap<A7>::value(xs[7]), unwrap<A8>::value(xs[8]));
			return 0;
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< void >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			result.push_back(typeName< A6 >());
			result.push_back(typeName< A7 >());
			result.push_back(typeName< A8 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };


template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9 >
    class reducefn10 : public reducefn {
    public:
        typedef R (*action)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9);
        reducefn10(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			return j.apply< R >(this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]), unwrap<A6>::value(xs[6]), unwrap<A7>::value(xs[7]), unwrap<A8>::value(xs[8]), unwrap<A9>::value(xs[9])));
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< R >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			result.push_back(typeName< A6 >());
			result.push_back(typeName< A7 >());
			result.push_back(typeName< A8 >());
			result.push_back(typeName< A9 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };

template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9 >
	reducefnp reduceWith(R(*cb)(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9)) {
		return reducefnp(new reducefn10< R,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9 >(cb));
    }


template < typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9 >
    class reducefn10<void, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9> : public reducefn {
    public:
        typedef void (*action)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9);
        reducefn10(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]), unwrap<A6>::value(xs[6]), unwrap<A7>::value(xs[7]), unwrap<A8>::value(xs[8]), unwrap<A9>::value(xs[9]));
			return 0;
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< void >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			result.push_back(typeName< A6 >());
			result.push_back(typeName< A7 >());
			result.push_back(typeName< A8 >());
			result.push_back(typeName< A9 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };


template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10 >
    class reducefn11 : public reducefn {
    public:
        typedef R (*action)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10);
        reducefn11(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			return j.apply< R >(this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]), unwrap<A6>::value(xs[6]), unwrap<A7>::value(xs[7]), unwrap<A8>::value(xs[8]), unwrap<A9>::value(xs[9]), unwrap<A10>::value(xs[10])));
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< R >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			result.push_back(typeName< A6 >());
			result.push_back(typeName< A7 >());
			result.push_back(typeName< A8 >());
			result.push_back(typeName< A9 >());
			result.push_back(typeName< A10 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };

template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10 >
	reducefnp reduceWith(R(*cb)(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10)) {
		return reducefnp(new reducefn11< R,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10 >(cb));
    }


template < typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10 >
    class reducefn11<void, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10> : public reducefn {
    public:
        typedef void (*action)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10);
        reducefn11(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]), unwrap<A6>::value(xs[6]), unwrap<A7>::value(xs[7]), unwrap<A8>::value(xs[8]), unwrap<A9>::value(xs[9]), unwrap<A10>::value(xs[10]));
			return 0;
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< void >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			result.push_back(typeName< A6 >());
			result.push_back(typeName< A7 >());
			result.push_back(typeName< A8 >());
			result.push_back(typeName< A9 >());
			result.push_back(typeName< A10 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };


template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11 >
    class reducefn12 : public reducefn {
    public:
        typedef R (*action)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11);
        reducefn12(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			return j.apply< R >(this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]), unwrap<A6>::value(xs[6]), unwrap<A7>::value(xs[7]), unwrap<A8>::value(xs[8]), unwrap<A9>::value(xs[9]), unwrap<A10>::value(xs[10]), unwrap<A11>::value(xs[11])));
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< R >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			result.push_back(typeName< A6 >());
			result.push_back(typeName< A7 >());
			result.push_back(typeName< A8 >());
			result.push_back(typeName< A9 >());
			result.push_back(typeName< A10 >());
			result.push_back(typeName< A11 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };

template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11 >
	reducefnp reduceWith(R(*cb)(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11)) {
		return reducefnp(new reducefn12< R,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11 >(cb));
    }


template < typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11 >
    class reducefn12<void, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11> : public reducefn {
    public:
        typedef void (*action)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11);
        reducefn12(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]), unwrap<A6>::value(xs[6]), unwrap<A7>::value(xs[7]), unwrap<A8>::value(xs[8]), unwrap<A9>::value(xs[9]), unwrap<A10>::value(xs[10]), unwrap<A11>::value(xs[11]));
			return 0;
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< void >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			result.push_back(typeName< A6 >());
			result.push_back(typeName< A7 >());
			result.push_back(typeName< A8 >());
			result.push_back(typeName< A9 >());
			result.push_back(typeName< A10 >());
			result.push_back(typeName< A11 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };


template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12 >
    class reducefn13 : public reducefn {
    public:
        typedef R (*action)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12);
        reducefn13(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			return j.apply< R >(this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]), unwrap<A6>::value(xs[6]), unwrap<A7>::value(xs[7]), unwrap<A8>::value(xs[8]), unwrap<A9>::value(xs[9]), unwrap<A10>::value(xs[10]), unwrap<A11>::value(xs[11]), unwrap<A12>::value(xs[12])));
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< R >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			result.push_back(typeName< A6 >());
			result.push_back(typeName< A7 >());
			result.push_back(typeName< A8 >());
			result.push_back(typeName< A9 >());
			result.push_back(typeName< A10 >());
			result.push_back(typeName< A11 >());
			result.push_back(typeName< A12 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };

template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12 >
	reducefnp reduceWith(R(*cb)(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12)) {
		return reducefnp(new reducefn13< R,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12 >(cb));
    }


template < typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12 >
    class reducefn13<void, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12> : public reducefn {
    public:
        typedef void (*action)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12);
        reducefn13(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]), unwrap<A6>::value(xs[6]), unwrap<A7>::value(xs[7]), unwrap<A8>::value(xs[8]), unwrap<A9>::value(xs[9]), unwrap<A10>::value(xs[10]), unwrap<A11>::value(xs[11]), unwrap<A12>::value(xs[12]));
			return 0;
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< void >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			result.push_back(typeName< A6 >());
			result.push_back(typeName< A7 >());
			result.push_back(typeName< A8 >());
			result.push_back(typeName< A9 >());
			result.push_back(typeName< A10 >());
			result.push_back(typeName< A11 >());
			result.push_back(typeName< A12 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };


template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13 >
    class reducefn14 : public reducefn {
    public:
        typedef R (*action)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13);
        reducefn14(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			return j.apply< R >(this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]), unwrap<A6>::value(xs[6]), unwrap<A7>::value(xs[7]), unwrap<A8>::value(xs[8]), unwrap<A9>::value(xs[9]), unwrap<A10>::value(xs[10]), unwrap<A11>::value(xs[11]), unwrap<A12>::value(xs[12]), unwrap<A13>::value(xs[13])));
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< R >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			result.push_back(typeName< A6 >());
			result.push_back(typeName< A7 >());
			result.push_back(typeName< A8 >());
			result.push_back(typeName< A9 >());
			result.push_back(typeName< A10 >());
			result.push_back(typeName< A11 >());
			result.push_back(typeName< A12 >());
			result.push_back(typeName< A13 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };

template < typename R, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13 >
	reducefnp reduceWith(R(*cb)(A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13)) {
		return reducefnp(new reducefn14< R,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13 >(cb));
    }


template < typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13 >
    class reducefn14<void, A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13> : public reducefn {
    public:
        typedef void (*action)(A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13);
        reducefn14(action cb) : cb(cb) { }

		terminal* apply(const inj& j, const terminals& xs) const {
			this->cb(unwrap<A0>::value(xs[0]), unwrap<A1>::value(xs[1]), unwrap<A2>::value(xs[2]), unwrap<A3>::value(xs[3]), unwrap<A4>::value(xs[4]), unwrap<A5>::value(xs[5]), unwrap<A6>::value(xs[6]), unwrap<A7>::value(xs[7]), unwrap<A8>::value(xs[8]), unwrap<A9>::value(xs[9]), unwrap<A10>::value(xs[10]), unwrap<A11>::value(xs[11]), unwrap<A12>::value(xs[12]), unwrap<A13>::value(xs[13]));
			return 0;
		}

		TypePtr type() const {
			return fn(ctys(argumentTypes()), cty(resultType()));
		}

		std::string resultType() const {
			return typeName< void >();
		}

		std::vector<std::string> argumentTypes() const {
			std::vector<std::string> result;
			result.push_back(typeName< A0 >());
			result.push_back(typeName< A1 >());
			result.push_back(typeName< A2 >());
			result.push_back(typeName< A3 >());
			result.push_back(typeName< A4 >());
			result.push_back(typeName< A5 >());
			result.push_back(typeName< A6 >());
			result.push_back(typeName< A7 >());
			result.push_back(typeName< A8 >());
			result.push_back(typeName< A9 >());
			result.push_back(typeName< A10 >());
			result.push_back(typeName< A11 >());
			result.push_back(typeName< A12 >());
			result.push_back(typeName< A13 >());
			return result;
		}

		action value() const { return this->cb; }
    private:
        action cb;
    };

