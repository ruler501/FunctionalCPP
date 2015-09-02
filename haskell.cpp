
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
 
//! \todo Not quite functional, but implementing a concat iterator is hard
template<typename T>
std::vector<T> concat(T first,
                      std::vector<T> list){
    std::vector<T> v(1, first);
    v.insert(v.end(), list.begin(), list.end());
    return v;
}

//! \todo make functional. Could possibly break loop into a recursion
template<typename T>
std::vector<T> flatten(std::vector<std::vector<T> > a){
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
    return concat(f(list.front()),
                  map(std::vector<T>(list.begin()+1, list.end()), f));
}

template<typename T, typename U, typename R, typename Callable>
std::vector<typename std::result_of<Callable(T,U)>::type> zipWith(std::vector<T> list1,
                       std::vector<U> list2,
                       Callable f){
    if(list1.empty() || list2.empty())
        return std::vector<R>();
    return concat(f(list1.front(), list2.front()),
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
typename std::result_of<Callable(T)>::type operator>>=(std::vector<T> a, Callable f){
    return flatten(map(a,f));
}

template<typename T, typename U>
std::vector<U> operator>>(std::vector<T> a, std::vector<U> b){
    return b;
}

template<typename T>
std::vector<T> repeat(int n, T a){
    if(n <= 0) return std::vector<T>();
    return concat(a, repeat(n-1, a));
}
