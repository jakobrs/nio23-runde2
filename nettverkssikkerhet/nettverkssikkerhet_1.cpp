#include <iostream>
#include <queue>
#include <vector>

int main() {
  int n, m, k, p;
  std::cin >> n >> m >> k >> p;

  std::vector<std::vector<int>> adj(n);

  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x].push_back(y);
    adj[y].push_back(x);
  }

  for (int i = 0; i < p; i++) {
    // no
  }

  // Construct BFS tree
  std::vector<int> parent(n, -1);
  {
    std::vector<bool> visited(n, false);
    for (int i = 0; i < n; i++) {
      if (!visited[i]) {
        // `i` will be the root of this subtree
        std::queue<int> stack;
        stack.push(i);

        visited[i] = true;

        while (!stack.empty()) {
          int here = stack.front();
          stack.pop();

          for (auto neighbour : adj[here]) {
            if (!visited[neighbour]) {
              visited[neighbour] = true;
              parent[neighbour] = here;
              stack.push(neighbour);
            }
          }
        }
      }
    }
  }

  int total = 0;

  std::vector<bool> visited(n, false);
  for (int i = 0; i < n; i++) {
    if (!visited[i]) {
      int size = 0;
      bool cyclic = false;

      std::vector<int> stack;
      stack.push_back(i);

      while (!stack.empty()) {
        int top = stack.back();
        stack.pop_back();

        if (visited[top]) {
          continue;
        }
        visited[top] = true;
        size += 1;

        for (int neighbour : adj[top]) {
          if (neighbour != parent[top]) {
            if (visited[neighbour]) {
              // we've found this node some other way
              cyclic = true;
            } else {
              stack.push_back(neighbour);
            }
          }
        }
      }

      total += (size + cyclic) / 2;
    }
  }

  std::cout << total << '\n';
}
