#ifndef _PRELUDE_H_
#define _PRELUDE_H_

#include <algorithm>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

#include "type_classes.h"

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
    
    //! \note I don't think there's a way to do this without modifying an object
    template<typename T>
    std::string show(T a){
        std::istringstream out;
        out << a;
        return out.str();
    }

/********************************************************************************
 * End Non Functional Section                                         *
 ********************************************************************************/

    template<typename T, typename Callable>
    std::vector<typename std::result_of<Callable(T)>::type> map(Callable f,
                                                                std::vector<T> list){
        if(list.empty()) return std::vector<typename std::result_of<Callable(T)>::type>();
        return prepend(f(list.front()),
                      map(f, std::vector<T>(list.begin()+1, list.end())));
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
    U foldl(Callable f, U svalue, std::vector<T> list){
        if(list.empty()) return svalue;
        return foldl(f, f(svalue, list.front()),
                     std::vector<T>(list.begin()+1, list.end()));
    }
    
    template<typename T, typename U, typename Callable>
    U foldr(Callable f, U svalue, std::vector<T> list){
        if(list.empty()) return svalue;
        return foldr(f, f(list.back(), svalue),
                     std::vector<T>(list.begin(), list.end()-1));
    }

    template<typename T>
    std::vector<T> reverse(std::vector<T> list){
        return std::vector<T>(list.rbegin(), list.rend());
    }

    //! \note also the same as return in Haskell, but return is a keyword in C++
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
    
    //! \note Called break in Haskell, but that's a reserved keyword in C++
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
    
    //! \note In Haskell this throws an exception if c is not a digit
    //! we just return -1 to be more C-like
    int digitToInt(char c){
        if('0' <= c && c <= '9') return c-'0';
        if('a' <= c && c <= 'z') return c-'a'+10;
        if('A' <= c && c <= 'Z') return c-'A'+10;
        return -1;
    }
    
    template<typename T>
    std::vector<T> drop(int n, std::vector<T> list){
        if(n <= 0 || list.empty()) return list;
        return drop(n-1, std::vector<T>(list.begin()+1, list.end()));
    }
    
    //! \note is implemented functionally, but could also be done with find_if
    template<typename T, typename Callable>
    std::vector<T> dropWhile(Callable f, std::vector<T> list){
        if(list.empty() || !f(list.front())) return list;
        return dropWhile(f, std::vector<T>(list.begin()+1, list.end()));
    }
    
    template<typename T>
    bool elem(T a, std::vector<T> list){
        return std::find(list.begin(), list.end(), a) != list.end();
    }
    
    template<typename T>
    bool even(T n){
        return !(n & 1);
    }
    
    template<typename T, typename Callable>
    std::vector<T> filter(Callable f, std::vector<T> list){
        if(list.empty()) return std::vector<T>();
        if(f(list.front())) return prepend(list.front(), filter(f, std::vector<T>(list.begin()+1, list.end())));
        return filter(f, std::vector<T>(list.begin()+1, list.end()));
    }
    
    template<typename T, typename U>
    T fst(std::pair<T,U> a){
        return a.first;
    }
    
    //! \note We return -1 when both are zero unlike Haskell which throws an exception
    template<typename T>
    T gcd(T a, T b){
        if(b == 0){
            if(a == 0) return -1;
            return a;
        }
        return gcd(b, a % b);
    }
    
    template<typename T>
    T head(std::vector<T> list){
        return list.front();
    }
    
    template<typename T>
    T id(T a){
        return a;
    }
    
    template<typename T>
    std::vector<T> init(std::vector<T> list){
        return std::vector<T>(list.begin(), list.end()-1);
    }
    
    bool isAlpha(char c){
        return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
    }
    
    bool isDigit(char c){
        return (c >= '0' && c <= '9');
    }
    
    bool isLower(char c){
        return (c >= 'a' && c <= 'z');
    }
    
    bool isSpace(char c){
        return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
    }
    
    bool isUpper(char c){
        return (c >= 'A' && c <= 'Z');
    }
    
    //! \todo Implement some vector like type for infinite lists for now
    //! you'll have to specify the number of iterations per call
    template<typename T, typename Callable>
    std::vector<T> iterate(Callable f, T svalue, int N){
        if(N == 0) return std::vector<T>();
        return prepend(svalue, iterate(f, f(svalue), N-1));
    }
    
    template<typename T>
    T last(std::vector<T> list){
        return list.back();
    }
    
    template<typename T>
    T lcm(T a, T b){
        if(a == 0 || b == 0) return 0;
        return ((a/gcd(a,b))*b);
    }
    
    template<typename T>
    size_t length(std::vector<T> list){
        return list.size();
    }
    
    std::vector<std::string> lines(std::string str){
        if(str.empty()) return std::vector<std::string>();
        auto i = std::find(str.begin(), str.end(), '\n');
        if(i != str.end()) return prepend(std::string(str.begin(), i), lines(std::string(i+1, str.end())));
        return std::vector<std::string>(1,str);
    }
    
    template<typename T>
    T maximum(std::vector<T> list){
        return *std::max_element(list.begin(), list.end());
    }
    
    template<typename T>
    T minimum(std::vector<T> list){
        return *std::min_element(list.begin(), list.end());
    }
    
    template<typename T>
    T mod(T a, T b){
        return a % b;
    }
    
    template<typename T>
    bool notElem(T a, std::vector<T> list){
        return !elem(a, list);
    }
    
    template<typename T>
    bool null(std::vector<T> list){
        return list.empty();
    }
    
    template<typename T>
    bool odd(T a){
        return (a & 1);
    }
    
    void putStr(std::string str){
        std::cout << str;
    }
    
    template<typename T>
    T product(std::vector<T> list){
        return foldl(std::multiplies<T>(), 1, list);
    }
    
    template<typename T>
    T quot(T a, T b){
        return a/b;
    }
    
    template<typename T>
    T rem(T a, T b){
        return a % b;
    }
    
    //! \note Currently the same as \a repeat
    template<typename T>
    std::vector<T> replicate(int n, T a){
        if(n <= 0) return std::vector<T>();
        return prepend(a, replicate(n-1, a));
    }
    
    template<typename T, typename U>
    U snd(std::pair<T, U> a){
        return a.second;
    }
    
    template<typename T, typename Callable>
    std::pair<std::vector<T>, std::vector<T> > span(Callable f, std::vector<T> list){
        return split([f](T a){return !f(a);}, list);
    }
    
    template<typename T>
    std::pair<std::vector<T>, std::vector<T> > splitAt(int n, std::vector<T> list){
        if(n > list.size()) return std::make_pair(list, std::vector<T>());
        return std::make_pair(std::vector<T>(list.begin(), list.begin()+n),
                              std::vector<T>(list.begin()+n, list.end()));
    }
    
    template<typename T>
    T subtract(T a, T b){
        return a - b;
    }
    
    template<typename T>
    T sum(std::vector<T> list){
        return foldl(std::plus<T>(), 0, list);
    }
    
    template<typename T>
    std::vector<T> tail(std::vector<T> list){
        return std::vector<T>(list.begin()+1, list.end());
    }
    
    template<typename T>
    std::vector<T> take(int n, std::vector<T> list){
        if(n > list.size()) return list;
        return std::vector<T>(list.begin(), list.begin()+n);
    }
    
    template<typename T, typename Callable>
    std::vector<T> takeWhile(Callable f, std::vector<T> list){
        if(list.empty()) return list;
        if(!f(list.front())) return std::vector<T>();
        return prepend(list.front(), takeWhile(f, std::vector<T>(list.begin()+1, list.end())));
    }
    
    char toLower(char c){
        if(isUpper(c)) return c - 'A' + 'a';
        return c;
    }
    
    char toUpper(char c){
        if(isLower(c)) return c - 'a' + 'A';
        return c;
    }
    
    std::string unlines(std::vector<std::string> list){
        return foldl(std::plus<std::string>(), std::string(),
                     map([](std::string str){return str+'\n';}, list));
    }
    
    template<typename T, typename Callable, typename Predicate>
    T until(Predicate p, Callable f, T svalue){
        if(p(svalue)) return svalue;
        return until(p, f, f(svalue));
    }
    
    std::string unwords(std::vector<std::string> list){
        std::string tStr(foldl(std::plus<std::string>(), std::string(),
                               map([](std::string str){return str+' ';}, list)));
        return std::string(tStr.begin(), tStr.end()-1);
    }
    
    std::vector<std::string> words(std::string str){
        if(str.empty()) return std::vector<std::string>();
        auto i = std::find_if(str.begin(), str.end(), isSpace);
        if(i != str.end()) return prepend(std::string(str.begin(), i), lines(std::string(i+1, str.end())));
        return std::vector<std::string>(1,str);
    }
    
    template<typename T, typename Callable>
    typename std::result_of<Callable(T)>::type operator>>=(std::vector<T> a, Callable f){
        return concat(map(f,a));
    }

    template<typename T, typename U>
    std::vector<U> operator>>(std::vector<T> a, std::vector<U> b){
        return a >>= [&](std::vector<T> m){return b;};
    }

    //! \todo Needs infinite lists, for now parameter for size will have to do
    template<typename T>
    std::vector<T> repeat(T a, int n){
        if(n <= 0) return std::vector<T>();
        return prepend(a, repeat(a, n-1));
    }
}
#endif //_PRELUDE_H_
