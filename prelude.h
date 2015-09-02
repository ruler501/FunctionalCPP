#ifndef _HASKELL_H_
#define _HASKELL_H_

#include <algorithm>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

namespace prelude{

/********************************************************************************
 * Non Functional Section                                             *
 * Needs improvments                                                  *
 ********************************************************************************/
     
    //! \todo make functional. Maybe recursion with flags?
    template<typename T>
    std::ostream& operator<<(std::ostream& stream, std::vector<T> list){
        stream << "<";
        if(list.size() > 1)
            for(auto i=list.begin(); i != list.end()-1; i++)
                stream << *i << ", ";
        return (stream << list.back() << ">");
    }
     
    //! \todo Not quite functional, but implementing a prepend iterator is hard
    template<typename T>
    std::vector<T> prepend(T first, std::vector<T> list){
        std::vector<T> v(list);
        v.insert(v.begin(), first);
        return v;
    }

    //! \todo make functional. Could possibly break loop into a recursion
    template<typename T>
    std::vector<T> concat(std::vector<std::vector<T> > a){
        std::vector<T> out(a.front());
        for(auto i=a.begin()+1; i != a.end(); i++)
            out.insert(out.end(), i->begin(), i->end());
        return out;
    }

/********************************************************************************
 * End Non Functional Section                                         *
 ********************************************************************************/

    template<typename T, typename Callable>
    std::vector<typename std::result_of<Callable(T)>::type> map(std::vector<T> list,
                       Callable f){
        if(list.empty()) return std::vector<typename std::result_of<Callable(T)>::type>();
        return prepend(f(list.front()),
                      map(std::vector<T>(list.begin()+1, list.end()), f));
    }

    template<typename T, typename U, typename R, typename Callable>
    std::vector<typename std::result_of<Callable(T,U)>::type> zipWith(std::vector<T> list1,
                           std::vector<U> list2,
                           Callable f){
        if(list1.empty() || list2.empty())
            return std::vector<R>();
        return prepend(f(list1.front(), list2.front()),
                      zipWith(std::vector<T>(list1.begin()+1, list1.end()),
                              std::vector<T>(list2.begin()+1, list2.end()),
                              f));
    }

    template<typename T, typename U>
    std::vector<std::pair<T, U> > zip(std::vector<T> list1,
                                      std::vector<T> list2){
        return zipWith(list1, list2, std::make_pair);
    }

    template<typename T, typename U, typename Callable>
    U foldl(std::vector<T> list, Callable f, U svalue){
        if(list.empty()) return svalue;
        return foldl(std::vector<T>(list.begin()+1, list.end()),
                     f, f(svalue, list.front()));
    }

    template<typename T>
    std::vector<T> reverse(std::vector<T> list){
        return std::vector<T>(list.rbegin(), list.rend());
    }

    template<typename T>
    std::vector<T> pure(T a){
        return std::vector<T>(1,a);
    }

    template<typename T, typename Callable>
    bool all(Callable f, std::vector<T> list){
        if(list.empty()) return true;
        return f(list.front()) && all(f, std::vector<T>(list.begin()+1, list.end()));
    }
    
    template<typename T, typename Callable>
    bool any(Callable f, std::vector<T> list){
        if(list.empty()) return false;
        return f(list.front()) || any(f, std::vector<T>(list.begin()+1, list.end()));
    }
        
    template<typename T, typename Callable>
    std::pair<std::vector<T>, std::vector<T> > split(Callable f, std::vector<T> list){
        auto i = std::find_if(list.begin(), list.end(), f);
        return std::make_pair(std::vector<T>(list.begin(), i),
                              std::vector<T>(i, list.end()));
    }
    
    enum Ordering{
        LT = -1,
        EQ,
        GT
    };
    
    template<typename T>
    Ordering compare(T a, T b){
        return (a < b ? LT : a == b ? EQ : GT);
    }
    
    template<typename T, typename Callable>
    typename std::result_of<Callable(T)>::type operator>>=(std::vector<T> a, Callable f){
        return concat(map(a,f));
    }

    template<typename T, typename U>
    std::vector<U> operator>>(std::vector<T> a, std::vector<U> b){
        return a >>= [&](std::vector<T> m){return b;};
    }

    template<typename T>
    std::vector<T> repeat(int n, T a){
        if(n <= 0) return std::vector<T>();
        return prepend(a, repeat(n-1, a));
    }
}
#endif //_HASKELL_H_
