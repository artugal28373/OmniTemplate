#pragma once
#include "range.hpp"
struct id {};
template <typename Group> Group operator/(Group const &a, Group const &b) {
	return a * (id{} / b);
}
template <typename T1, typename T2>
auto constexpr operator-(const T1 &a, const T2 &b) {
	return a + -b;
}
template <typename T1, typename T2>
auto constexpr operator!=(const T1 &a, const T2 &b) {
	return !(a == b);
}
template <typename T1, typename T2> auto operator<=(const T1 &a, const T2 &b) {
	return !(b < a);
}
template <typename T1, typename T2> auto operator>=(const T1 &a, const T2 &b) {
	return b <= a;
}
template <typename T1, typename T2> auto operator>(const T1 &a, const T2 &b) {
	return b < a;
}
tm() using uset = unordered_set<T>;
um operator+(um a, um const &b) {
	for (const auto &p : b) {
		a[p.first] += p.second;
	}
	return a;
}
tm() auto operator|(unordered_set<T> const &a, unordered_set<T> const &b) {
	const auto &[sm, bg] = minmax(a, b);
	auto ret = bg;
	ret.insert(al(sm));
	return ret;
};
tm() auto operator&(unordered_set<T> const &a, unordered_set<T> const &b) {
	const auto &[sm, bg] = minmax(a, b);
	unordered_set<T> ret;
	for (const auto &x : sm) {
		if (bg.count(x)) {
			ret.insert(x);
		}
	}
	return ret;
}
tm() auto sub_set(unordered_set<T> const &a, unordered_set<T> const &b,
		  unordered_set<T> &ret) {
	for (const auto &x : a) {
		if (!b.count(x)) {
			ret.insert(x);
		}
	}
	return ret;
}
tm() auto operator-(unordered_set<T> const &a, unordered_set<T> const &b) {
	unordered_set<T> ret;
	return sub_set(a, b, ret);
}
tm() auto operator^(unordered_set<T> const &a, unordered_set<T> const &b) {
	unordered_set<T> ret = a - b;
	sub_set(b, a, ret);
	return ret;
}
tm() auto operator*(vector<T> a, vector<T> b) {
	assert(a.size() == b.size());
	vector<T> c(a.size());
	fo(i, a.size()) { c[i] = a[i] * b[i]; }
	return c;
}
template <typename Iterator>
auto operator+(
    Iterator it,
    enable_if_t<
	!is_same<typename iterator_traits<decltype(it)>::iterator_category,
		 random_access_iterator_tag>::value,
	size_t>
	n) {
	advance(it, n);
	return it;
}
tm() using bin_op = T(*)(T, T);
#define ret(x, id)                                                             \
	if (f == static_cast<decltype(f)>(x)) {                                \
		return id;                                                     \
	}
constexpr ll identity(bin_op<ll const &> const &f) {
	ret(max<ll>, -inf);
	ret(min<ll>, inf);
	abort();
}
constexpr ll identity(bin_op<ll> const &f) {
	ret(gcd<ll>, 0);
	abort();
}
