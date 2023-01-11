#include <iostream>
#include <vector>

struct Node {
  int superordinate;
  std::vector<int> subordinates;

  int lower;
  int upper;

  Node()
      : superordinate(0), subordinates(), lower(0), upper(0) {}
};

struct SegTree {
  std::vector<int> values;
  int n;

  SegTree(int n) : values(2 * n, 0), n(n) {}

  void update(int left, int right) { update_internal(left, right, 1, 0, n); }

  std::vector<int> serialize() {
    std::vector<int> result;
    result.reserve(n);
    serialise_internal(result, 1, 0);
    return std::move(result);
  }

private:
  void update_internal(int left, int right, int v, int start, int end) {
    if (left <= start && end <= right) {
      values[v] += 1;
    } else if (end <= left || right <= start) {
      return;
    } else {
      int mid = (start + end) / 2;
      update_internal(left, right, 2 * v, start, mid);
      update_internal(left, right, 2 * v + 1, mid, end);
    }
  }

  void serialise_internal(std::vector<int> &out, int v, int extra) {
    extra += values[v];
    if (v >= n) {
      out.push_back(extra);
    } else {
      serialise_internal(out, 2 * v, extra);
      serialise_internal(out, 2 * v + 1, extra);
    }
  }

  int count_internal(int v, bool invert) {
    bool here = invert ^ values[v];
    if (v >= n) {
      return here;
    } else {
      return count_internal(2 * v, here) +
             count_internal(2 * v + 1, here);
    }
  }
};


void annotate_rank(std::vector<Node> &nodes, int node_idx, int &rank) {
  Node &node = nodes[node_idx];
  node.lower = rank;
  for (auto child : node.subordinates) {
    annotate_rank(nodes, child, rank);
  }
  node.upper = ++rank;
}

int find_ancestor(const std::vector<Node> &nodes, int node_idx, int g) {
  const Node &node = nodes[node_idx];
  int parent_idx = node.superordinate;
  const Node &parent = nodes[parent_idx];

  if (parent.upper - parent.lower > g) {
    return node_idx;
  } else {
    return find_ancestor(nodes, parent_idx, g);
  }
}

int main() {
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  int n, m;
  std::cin >> n >> m;

  std::vector<Node> nodes(n);

  for (int i = 1; i < n; i++) {
    int x;
    std::cin >> x;
    nodes[i].superordinate = x;
    nodes[x].subordinates.push_back(i);
  }

  int rank = 0;
  annotate_rank(nodes, 0, rank);

  // for (int i = 0; i < n; i++) {
  //   const Node &node = nodes[i];
  //   std::cout << i << ":\n";
  //   std::cout << "  superordinate: " << node.superordinate << "\n";
  //   std::cout << "  subordinates:\n";
  //   for (int child : node.subordinates) {
  //     std::cout << "  - " << child << "\n";
  //   }
  //   std::cout << "  lower: " << node.lower << "\n";
  //   std::cout << "  upper: " << node.upper << "\n";
  // }

  /*

    Strategy:

    Where
      N = number of nodes
      K = number of orders
      H = height of tree

    For each order,
      Find the parent of the highest-ranking direct supervisor who will approve the work (O(height of tree))
      Let A = Last supervisor needed, Al and Ar be the endpoints of A's range in the segtree ([Al, Ar)),
          P = supervisor of A, Pl and Pr defined likewise, and
          G = number of approvals needed (O(1))

      We know that G > Ar - Al, so let G' = G - (Ar - Al). IF Al - Pl >= G', we do two updates in the segtree:
      1. update(Al, Ar, +1)       (O(log N))
      2. update(Pl, Pl + G', +1)  (O(log N))
      Otherwise we only need one:
      1. update(Al, Pl + G, +1)   (O(log N))

    In the end we simply read off the segtree and reorder the values.

    Complexity: O(K H log N)
    - Might be fine?

    Special cases:
    - P may not be defined if G = N
      - Just add a special case for this specific case
  */

  int rounded_n = 1;
  while (rounded_n < n) {
    rounded_n *= 2;
  }
  SegTree st(rounded_n);

  for (int i = 0; i < m; i++) {
    int w, g;
    std::cin >> w >> g;

    if (g == n) {
      // Special case where A doesn't have a parent
      st.update(0, rounded_n);
    } else {
      int ancestor_idx = find_ancestor(nodes, w, g);
      // find_ancestor returns a node A st.
      //   A's order <= G
      //   A's parent's order > G
      //   A is a descendant of W

      const Node &ancestor = nodes[ancestor_idx];
      const Node &parent = nodes[ancestor.superordinate];

      int g1 = g - (ancestor.upper - ancestor.lower);
      if (ancestor.lower - parent.lower >= g1) {
        st.update(ancestor.lower, ancestor.upper);
        st.update(parent.lower, parent.lower + g1);
      } else {
        st.update(parent.lower, parent.lower + g);
      }
    }
  }

  std::vector<int> result = st.serialize();

  for (int i = 0; i < n; i++) {
    std::cout << result[nodes[i].upper - 1] << ' ';
  }
}
