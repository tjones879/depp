#ifndef DEPP_UTILS_HPP
#define DEPP_UTILS_HPP

#include <vector>

namespace depp {
template <class It>
struct iter_pair {
    It b, e;

    It begin() const { return b; }
    It end() const { return e; }
};

template <class C>
using iterator_t = decltype(std::begin(std::declval<C>()));

template <class C>
iter_pair<iterator_t<C&>> offset(C& container, size_t skip) {
    return {std::next(container.begin(), skip), container.end()};
}

}

#endif
