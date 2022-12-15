#pragma once
#include <iterator>

template <typename IteratorT> class EnumerateIt
    : std::iterator<std::forward_iterator_tag,
                    typename std::iterator_traits<IteratorT>::value_type> {
private:
public:
  size_t mCurIdx;
  IteratorT mItr;
  using ValueT = typename std::iterator_traits<IteratorT>::value_type;

  explicit EnumerateIt(IteratorT iterator)
      : mCurIdx{0}, mItr{std::move(iterator)} {}

  EnumerateIt(IteratorT iterator, size_t startingCount)
      : mCurIdx{startingCount}, mItr{std::move(iterator)} {}

  EnumerateIt& operator++() {
    ++mItr;
    ++mCurIdx;
    return *this;
  }

  bool operator==(const EnumerateIt& enumItr) const {
    return (mCurIdx == enumItr.mCurIdx) && (mItr == enumItr.mItr);
  }

  bool operator!=(const EnumerateIt& enumItr) const { return !(*this == enumItr); }

  decltype(auto) operator*() {
    return std::make_pair(mCurIdx, std::forward<decltype(*mItr)>(*mItr));
  }

  decltype(auto) operator*() const {
    return std::make_pair(mCurIdx, std::forward<decltype(*mItr)>(*mItr));
  }
};

template <typename T> struct EnumerateWrapper {
  T& range;
};

template <typename T> EnumerateWrapper<T> Enumerate(T&& range) { return {range}; }

template <typename T> auto begin(EnumerateWrapper<T> wrapper) {
  return EnumerateIt<decltype(std::begin(wrapper.range))>(std::begin(wrapper.range));
}

template <typename T> auto end(EnumerateWrapper<T> wrapper) {
  return EnumerateIt<decltype(std::end(wrapper.range))>(
      std::end(wrapper.range), std::distance(std::begin(wrapper.range), std::end(wrapper.range)));
}
