#ifndef ITER_FILTER_FALSE_HPP_
#define ITER_FILTER_FALSE_HPP_

#include "internal/iterbase.hpp"
#include "filter.hpp"

#include <utility>

namespace iter {

  namespace detail {

    // Callable object that reverses the boolean result of another
    // callable, taking the object in a Container's iterator
    template <typename FilterFunc, typename Container>
    class PredicateFlipper {
     private:
      FilterFunc filter_func;

     public:
      PredicateFlipper(FilterFunc in_filter_func)
          : filter_func(in_filter_func) {}

      PredicateFlipper() = delete;
      PredicateFlipper(const PredicateFlipper&) = default;

      // Calls the filter_func
      bool operator()(const iter::impl::iterator_deref<Container> item) const {
        return !bool(filter_func(item));
      }

      // with non-const incase FilterFunc::operator() is non-const
      bool operator()(const iter::impl::iterator_deref<Container> item) {
        return !bool(filter_func(item));
      }
    };

    // Reverses the bool() conversion result of anything that supports a
    // bool conversion
    template <typename Container>
    class BoolFlipper {
     public:
      bool operator()(const iter::impl::iterator_deref<Container> item) const {
        return !bool(item);
      }
    };
  }

  // Creates a PredicateFlipper for the predicate function, which reverses
  // the bool result of the function.  The PredicateFlipper is then passed
  // to the normal filter() function
  template <typename FilterFunc, typename Container>
  auto filterfalse(FilterFunc filter_func, Container&& container) {
    return filter(detail::PredicateFlipper<FilterFunc, Container>(filter_func),
        std::forward<Container>(container));
  }

  // Single argument version, uses a BoolFlipper to reverse the truthiness
  // of an object
  template <typename Container>
  auto filterfalse(Container&& container) {
    return filter(
        detail::BoolFlipper<Container>(), std::forward<Container>(container));
  }
}

#endif
