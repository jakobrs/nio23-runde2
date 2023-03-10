#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

struct Node {
  int superordinate;
  std::vector<int> subordinates;

  int jump;
  int dist;

  int lower;
  int upper;

  Node()
      : superordinate(-1),
        subordinates(),
        jump(-1),
        dist(-1),
        lower(0),
        upper(0) {}
};

void annotate_rank(std::vector<Node> &nodes) {
  int rank = 0;

  std::vector<
      std::tuple<int, std::vector<int>::iterator, std::vector<int>::iterator>>
      stack;
  nodes[0].lower = rank;
  stack.push_back(std::make_tuple(0, nodes[0].subordinates.begin(),
                                  nodes[0].subordinates.end()));

  while (!stack.empty()) {
    auto &top = stack.back();
    int n = std::get<0>(top);
    auto &here = std::get<1>(top);
    auto &end = std::get<2>(top);

    if (here == end) {
      stack.pop_back();
      nodes[n].upper = ++rank;
    } else {
      int next = *(here++);
      Node &node = nodes[next];
      node.lower = rank;

      stack.push_back(std::make_tuple(next, node.subordinates.begin(),
                                      node.subordinates.end()));
    }
  }
}

int find_ancestor(const std::vector<Node> &nodes, int node_idx, int g) {
  const Node &node = nodes[node_idx];
  int parent_idx = node.superordinate;
  const Node &parent = nodes[parent_idx];

  if (parent.upper - parent.lower > g) {
    return node_idx;
  }

  if (node.dist >= 0) {
    const Node &jumped_to = nodes[node.jump];

    if (jumped_to.upper - jumped_to.lower <= g) {
      return find_ancestor(nodes, node.jump, g);
    }
  }
  return find_ancestor(nodes, parent_idx, g);
}

void add_jump_links(std::vector<Node> &nodes) {
  std::vector<int> stack;
  stack.push_back(0);

  int max = 0;

  while (!stack.empty()) {
    int node_idx = stack.back();
    stack.pop_back();

    Node &node = nodes[node_idx];

    // add the link
    if (node.superordinate != -1) {
      Node &superordinate = nodes[node.superordinate];

      if (superordinate.jump != -1) {
        Node &middle = nodes[superordinate.jump];

        if (superordinate.dist == middle.dist) {
          node.jump = middle.jump;
          node.dist = middle.dist * 2 + 1;

          max = std::max(max, node.dist);

          goto a;
        }
      }

      node.jump = node.superordinate;
      node.dist = 1;
    a:;
    }

    for (auto child : node.subordinates) {
      stack.push_back(child);
    }
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
  annotate_rank(nodes);
  add_jump_links(nodes);

  // for (int i = 0; i < n; i++) {
  //   const Node &node = nodes[i];
  //   std::cout << i << ":\n";
  //   std::cout << "  superordinate: " << node.superordinate << "\n";
  //   std::cout << "  jump:\n";
  //   std::cout << "    to: " << node.jump << "\n";
  //   std::cout << "    length: " << node.dist << "\n";
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
      Find the parent of the highest-ranking direct supervisor who will approve the work (O(log H))
      Let A = Last supervisor needed, Al and Ar be the endpoints of A's range ([Al, Ar)),
          P = supervisor of A, Pl and Pr defined likewise, and
          G = number of approvals needed (O(1))

      We know that G > Ar - Al, so let G' = G - (Ar - Al). IF Al - Pl >= G', we do two updates in the segtree:
      1. update(Al, Ar, +1)       (O(1))
      2. update(Pl, Pl + G', +1)  (O(1))
      Otherwise we only need one:
      1. update(Al, Pl + G, +1)   (O(1))

    In the end we simply read off the segtree and reorder the values.

    Complexity: O(K log H + N)
    - Might be fine?

    Special cases:
    - P may not be defined if G = N
      - Just add a special case for this specific case
  */

  std::vector<int> diffs(n + 1);

  for (int i = 0; i < m; i++) {
    int w, g;
    std::cin >> w >> g;

    if (g == n) {
      // Special case where A doesn't have a parent
      diffs[0] += 1;
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
        diffs[ancestor.lower] += 1;
        diffs[ancestor.upper] -= 1;
        diffs[parent.lower] += 1;
        diffs[parent.lower + g1] -= 1;
      } else {
        diffs[parent.lower] += 1;
        diffs[parent.lower + g] -= 1;
      }
    }
  }

  std::vector<int> result;
  result.reserve(n);
  int prev = 0;
  for (int i = 0; i < n; i++) {
    result.push_back(prev += diffs[i]);
  }

  for (int i = 0; i < n; i++) {
    std::cout << result[nodes[i].upper - 1] << ' ';
  }
}
