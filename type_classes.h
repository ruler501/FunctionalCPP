#ifndef _TYPE_CLASSES_H_
#define _TYPE_CLASSES_H_

//! \todo Default implementations?
//! \todo Compile time checking of requirements
//! \note Need a Rational type to implement Fractional, Integral, and Real

namespace prelude{
    template<typename T>
    struct Eq{
        //bool operator==(T,T)
        //bool operator!=(T,T)
        
        //(x == y)  = !(x != y)
        
        static constexpr bool is_valid = false;
    };
    
    enum Ordering{
        LT = -1,
        EQ,
        GT
    };
    
    template<typename T>
    struct Ord : public Eq{
        //Ordering compare(T,T)
        //bool operator<, operator<=, operator>=, operator>(T,T)
        //T max,min(T,T)
        
        static constexpr bool is_valid = false;
    };
    
    //! \todo implement Show and Read
    //! \todo decide if Enum should be implemented
    
    template<typename T>
    struct Bounded{
        //T minBound,maxBound()
        
        static constexpr bool is_valid = false;
    }
    
    template<typename T>
    struct Num : public Eq{
        //T operator+, operator-, operator*(T,T)
        //T negate(T)
        //T abs, signum(T)
        //! \todo Figure out what Integer should be
        
        static constexpr bool is_valid = false;
    }
    
    //! \note is not a subclass of Fractional since we haven't implemented Fractional
    //! \note ** is not a valid operator in C++ so we use pow
    template<typename T>
    struct Floating{
        //T pi()
        //T exp, log, sqrt(T)
        //T pow, logBase(T,T)
        //T sin, cos, tan(T)
        //T asin, acos, atan(T)
        //T sinh, cosh, tanh(T)
        //T asinh, acosh, atanh(T)
        
        static constexpr bool is_valid = false;
    }
    
	template<typename T>
	struct Functor{
		//T<V> fmap(V(U), T<U>)
		
		//fmap(id,x) = id(x)
		//fmap(compose(g,h),x) = compose(fmap(g,x),fmap(h,x))
		
		static constexpr bool is_valid = false;
	};

	//! \note <*> is not a valid operator in C++ so we use ap
	template<typename T>
	struct Applicative : public Functor{
		//T<U> pure(U)
		//T<V> ap(T<V(U)>, T<U>)
		
		//ap(pure(id), x) = x
		//ap(pure(f), pure(x)) = pure(f(x))
		//ap(u,pure(y)) = ap(pure($ y), u) I'm not actually sure about this
		//ap(u,ap(v,w)) = ap(ap(ap(pure(compose),u),v),w)
		
		static constexpr bool is_valid = false;
		
	};
	
	//! \todo Implement Transformers at some point(after figuring out what they are)
	template<typename T>
	struct Monad : public Applicative{
		//T<V> operator>>=(T<U>, T<V>(U))
		//T<V> operator>>(T<U>, T<V>)
		//T<U> pure(U)
		
		//pure(a) >>= k = k(a)
		//m >>= pure = m
		//m >>= [](auto x){k(x) >>= h} = (m >>= k) >>= h
		//fmap(f, xs) = xs >>= compose(pure, f)
		
		static constexpr bool is_valid = false;
	};
	
	template<typename T>
	struct Semigroup{
		//T mappend(T,T)
		//T sconcat(std::vector<T>) non-empty input
		//T times1p(int, T)
		
		//mappend(mappend(a,b),c) = mappend(a,mappend(b,c))
		
		static constexpr bool is_valid = false;
	};

	template<typename T>
	struct Monoid{
		//T mappend(T, T)
		//T mempty()
		//T mconcat(std::vector<T>)
		
		//mappend(mempty(), x) = x
		//mappend(x, mempty()) = x
		//mappend(mappend(x,y),z) = mappend(x,mappend(y,z))
		
		static constexpr bool is_valid = false;
	};
	
	template<typename T>
	struct Foldable{
		//Monoid<U> => U fold(T<M>)
		//Monoid<U> => U foldMap(U(V), T<V>, U)
		//V foldr(V(U,V), V, T<U>)
		//U foldl(U(U,V), U, T<V>)
		//U foldr1(U(U,U), T<U>)
		//U foldl1(U(U,U), T<U>)
		
		static constexpr bool is_valid = false;
	};
	
	template<typename T>
	struct Traversable : public Functor, public Foldable{
		//Applicative<W> => W<T<V> > traverse(W<V>(U), T<U>)
		//Applicative<W> => W<T<U> > sequenceA(T<W<U> >)
		//Monad<R> => R<T<V> > mapM(R<V>(U), T<U>)
		//Monad<R> => R<T<U> > sequence(T<R<U> >)
		
		//I don't even know them laws
		
		static constexpr bool is_valid = false;
	};
	
	//! \todo Implement a Category and Arrow typeclass. They're scary
	
	template<typename T>
	struct Comonad : public Functor{
		//U extract(T<U>)
		//T<T<U> > duplicate(T<U>)
		//T<V> extend(V(T<U>), T<U>)
		
		static constexpr bool is_valid = false;
	};
}
#endif //_TYPE_CLASSES_H_
