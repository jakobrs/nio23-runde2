#include <iostream>
#include <vector>

struct Node {
  int superordinate;
  std::vector<int> subordinates;
  int approvals;

  Node()
      : superordinate(0), subordinates(), approvals(0) {}
};

void go_down(std::vector<Node> &nodes, int node_idx, int &needed_approvals) {
  Node &node = nodes[node_idx];
  for (int i = 0; i < node.subordinates.size() && needed_approvals > 0; i++) {
    go_down(nodes, node.subordinates[i], needed_approvals);
  }
  if (needed_approvals > 0) {
    node.approvals += 1;
    needed_approvals -= 1;
  }
}
void go_up(std::vector<Node> &nodes, int node_idx, int needed_approvals) {
  if (needed_approvals == 0) {
    return;
  }
  Node &node = nodes[node_idx];
  node.approvals += 1;
  needed_approvals -= 1;
  if (needed_approvals == 0) {
    return;
  }
  int parent_idx = node.superordinate;
  Node &parent = nodes[parent_idx];

  for (int i = 0; i < parent.subordinates.size() && needed_approvals > 0; i++) {
    int child_idx = parent.subordinates[i];
    if (child_idx != node_idx) {
      go_down(nodes, child_idx, needed_approvals);
    }
  }

  if (needed_approvals > 0) {
    go_up(nodes, parent_idx, needed_approvals);
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

  for (int i = 0; i < m; i++) {
    int w, g;
    std::cin >> w >> g;

    go_up(nodes, w, g);
  }

  for (const auto &node : nodes) {
    std::cout << node.approvals << ' ';
  }
}
