#include <type_traits>
#include <vector>

/********************************************************************************
 * Non Functional Section                                             *
 * Needs improvments                                                  *
 ********************************************************************************/
 
//! \todo make functional. Maybe recursion with flags?
template<typename T>
std::ostream& operator<<(std::ostream& stream, std::vector<T> list);

//! \todo Not quite functional, but implementing a concat iterator is hard
template<typename T>
std::vector<T> concat(T first,
                      std::vector<T> list);

//! \todo make functional. Could possibly break loop into a recursion
template<typename T>
std::vector<T> flatten(std::vector<std::vector<T> > a);

/********************************************************************************
 * End Non Functional Section                                         *
 ********************************************************************************/

template<typename T, typename Callable>
std::vector<typename std::result_of<Callable(T)>::type> map(std::vector<T> list,
                   Callable f);

template<typename T, typename U, typename R, typename Callable>
std::vector<typename std::result_of<Callable(T,U)>::type> zipWith(std::vector<T> list1,
                                                                  std::vector<U> list2,
                                                                  Callable f);

template<typename T, typename U>
std::vector<std::pair<T, U> > zip(std::vector<T> list1,
                                  std::vector<T> list2);

template<typename T, typename U, typename Callable>
U foldl(std::vector<T> list, Callable f, U svalue);

template<typename T>
std::vector<T> reverse(std::vector<T> list);

template<typename T>
std::vector<T> pure(T a);

template<typename T, typename Callable>
typename std::result_of<Callable(T)>::type operator>>=(std::vector<T> a, Callable f);

template<typename T, typename U>
std::vector<U> operator>>(std::vector<T> a, std::vector<U> b);

template<typename T>
std::vector<T> repeat(int n, T a);
