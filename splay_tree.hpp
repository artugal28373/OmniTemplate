#pragma once
#include "core.hpp"
namespace splay_tree {
/*! Implements a splay tree*/
template <typename T> struct SplayTree {
      public:
	/*! Splay tree node */
	struct Node {
	      public:
		// TODO Constructor?
		T value;
		array<Node *, 2> child{};
		Node *parent{};
	};
	/*! Splay tree iterator */
	struct iterator : public it_base<T> {
		using iterator_category = bidirectional_iterator_tag;

	      public:
		void operator--() { advance<false>(); }
		void operator++() { advance<true>(); }
		T &operator*() { return node->value; }
		Node *node;
		iterator(Node *node_arg) : node(node_arg) {}
		bool operator==(const iterator oth) const {
			return this.node == oth.node;
		}

	      private:
		template <bool dir> void advance() {
			if (node->child[1]) {
				node = extremum<!dir>(node->child[1]);
				return;
			}
			for (; node->parent && side(node) == dir;
			     node = node->parent)
				;
			node = node->parent;
		}
	};
	Node *root{}; /*!< Root node */
	size_t size_{}; /*!< Size of the splay tree*/
	SplayTree() {}
	~SplayTree() { destroy(root); }
	static void destroy(Node *const node) {
		/*! Destroy the subtree node */
		if (!node) {
			return;
		}
		for (Node *const child : node->child) {
			destroy(child);
		}
		delete node;
	}
	static void attach(Node *const par, bool side, Node *const new_) {
		if (new_) {
			new_->parent = par;
		}
		par->child[side] = new_;
	}
	static bool side(Node *const child) {
		/*! Returns true if child is on the right, and false otherwise*/
		// TODO use an enum for this
		return child->parent->child[1] == child;
	}
	static void rotate(Node *const x) {
		/*! Rotate node x around its parent */
		const auto p = x->parent;

		const bool i = side(x);

		if (p->parent) {
			attach(p->parent, side(p), x);
		} else {
			x->parent = nullptr;
		}
		attach(p, i, x->child[!i]);
		attach(x, !i, p);
	}
	static void splay(Node *const x) {
		/*! Splay node x. x will become the root of the tree*/
		while (x->parent) {
			if (!x->parent->parent) {
				// Zig step
				rotate(x);
				return;
			}
			if (side(x) == side(x->parent)) {
				// Zig-zig step
				rotate(x->parent);
				rotate(x);
			} else {
				// Zig-zag step
				rotate(x);
				rotate(x);
			}
		}
	}
	void insert(Node *const x) {
		/*! Insert node x into the splay tree*/
		++size_;
		if (!root) {
			root = x;
			return;
		}
		auto y = root;
		while (true) {
			auto &nw = y->child[x->value > y->value];
			if (!nw) {
				nw = x;
				nw->parent = y;
				root = nw;
				splay(nw);
				return;
			}
			y = nw;
		}
		return;
	}
	void insert(const T &key) {
		/*! Insert key key into the splay tree*/
		insert(new Node{key});
	}
	void erase(Node *const x) {
		/*! Erase node x from the splay tree*/
		assert(x);
		splay(x);
		root = join(x->child[0], x->child[1]);
		delete x;
		--size_;
	}
	void erase(const T &key) { erase(find(key)); }
	template <bool i> static Node *extremum(Node *const x) {
		/*! Return the extremum of the subtree x. Minimum if i is false,
		 * maximum if i is true.*/
		assert(x);
		return x->child[i] ? extremum<i>(x->child[i]) : x;
	}
	static Node *join(Node *const a, Node *const b) {
		if (!a) {
			b->parent = nullptr;
			return b;
		}
		Node *const mx = extremum<true>(a);
		splay(mx);
		assert(mx->child[1] == nullptr);
		mx->child[1] = b;
		mx->parent = nullptr;
		return mx;
	}
	array<Node *, 2> split(Node *const x) {
		splay(x);
		// TODO use detach function
		if (x->child[1]) {
			x->child[1]->parent = nullptr;
		}
		return {x, x->child[1]};
	}
	Node *find(const T &key) {
		/*! Returns node with key key*/
		auto x = root;
		for (; x && key != x->value; x = x->child[key > x->value])
			;
		return x;
	}
	size_t size() { return size_; }
	bool empty() { return size() == 0; }
	iterator begin() { return iterator{extremum<false>(root)}; }
	iterator end() { return iterator{nullptr}; }
};
void test_splay_tree() {
	SplayTree<int> sp;
	sp.insert(4);
	sp.insert(3);
	sp.insert(5);
	assert(sp.size() == 3);
	assert(!sp.empty());
	assert(sp.find(4)->value == 4);
	assert(sp.find(3)->value == 3);
	assert(sp.find(5)->value == 5);
	assert(sp.find(2) == nullptr);
	assert(sp.find(6) == nullptr);
	sp.erase(3);
	assert(sp.size() == 2);
	assert(sp.find(3) == nullptr);
	assert(sp.find(5)->value == 5);
	assert(sp.find(4)->value == 4);
	sp.insert(20);
	sp.insert(-2);
	sp.insert(6);
	vl expected{-2, 4, 5, 6, 20};
	assert(sp.size() == expected.size());
	for (auto x : expected) {
		assert(sp.find(x)->value == x);
	}
	vl vec;
	copy(sp.begin(), sp.end(), back_inserter(vec));
	assert(vec == expected);
}
} // namespace splay_tree
using namespace splay_tree;
