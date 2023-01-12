#include <iostream>
#include <queue>
#include <utility>
#include <vector>

int main() {
  int n, m, k, p;
  std::cin >> n >> m >> k >> p;

  std::vector<std::vector<int>> adj(n);
  // Online adjacencies are stored separately
  std::vector<std::pair<int, int>> padj;

  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x].push_back(y);
    adj[y].push_back(x);

    if (x < k && y < k) {
      padj.push_back(std::make_pair(x, y));
    }
  }

  for (int j = 0; j < p; j++) {
    int x, y;
    std::cin >> x >> y;
    padj.push_back(std::make_pair(x, y));
  }

  struct Extra {
    // to = -1 in case (2.1).
    int from, to;
    // Number of nodes not including nodes in G_K
    int extra_count;
  };

  std::vector<Extra> extras;

  std::vector<bool> visited(n, false);
  for (int i = 0; i < k; i++) {
    visited[i] = true;

    for (int j : adj[i]) {
      if (visited[j])
        continue;

      int prev = i;
      int cur = j;

      for (int t = 0;; t++) {
        if (cur < k) {
          extras.push_back(Extra{.from = i, .to = cur, .extra_count = t});
          if (cur == i) {
            goto skip;
          } else {
            goto next;
          }
        }

        const auto &neighbours = adj[cur];
        visited[cur] = true;
        if (neighbours.size() == 1) {
          extras.push_back(Extra{.from = i, .to = -1, .extra_count = t + 1});
          goto next;
        } else if (neighbours[0] == prev) {
          prev = cur;
          cur = neighbours[1];
        } else {
          prev = cur;
          cur = neighbours[0];
        }
      }

    next:;
    }

  skip:;
  }

  int best = 0;
  for (int set = 0; set < 1 << k; set++) {
#define TEST(i) (set & (1 << (i)))
    // Verify that this is indeed an independent set
    for (auto pair : padj) {
      int x = pair.first;
      int y = pair.second;

      if (TEST(x) && TEST(y)) {
        // not an independent set, `pair` connects `x` and `y`
        goto next_set;
      }
    }

    // fun fact you can't goto past variable initialisation
    int total;
    total = __builtin_popcount(set);

    for (const auto &extra : extras) {
      if (extra.to == -1) {
        total += (extra.extra_count + 1 - (bool)TEST(extra.from)) / 2;
      } else {
        total += (extra.extra_count + 1 - (bool)TEST(extra.from) - (bool)TEST(extra.to)) / 2;
      }
    }

    best = std::max(best, total);

  next_set:;
  }

  // We also need to count nodes not connected to a network-connected node
  int even_more = 0;
  int networked = 0;
  {
    std::vector<bool> new_visited = visited;
    for (bool x : new_visited) {
      if (x) {
        networked += 1;
      }
    }

    // This is straight up copied from my p1 solution
    // Construct BFS tree
    std::vector<int> parent(n, -1);
    {
      for (int i = 0; i < k; i++) {
        visited[i] = true;
      }

      for (int i = k; i < n; i++) {
        if (!visited[i]) {
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

    for (int i = k; i < n; i++) {
      if (!new_visited[i]) {
        int size = 0;
        bool cyclic = false;

        std::vector<int> stack;
        stack.push_back(i);

        while (!stack.empty()) {
          int top = stack.back();
          stack.pop_back();

          if (new_visited[top]) {
            continue;
          }
          new_visited[top] = true;
          size += 1;

          for (int neighbour : adj[top]) {
            if (neighbour != parent[top]) {
              if (new_visited[neighbour]) {
                cyclic = true;
              } else {
                stack.push_back(neighbour);
              }
            }
          }
        }

        even_more += (size + cyclic) / 2;
      }
    }
  }

  std::cout << networked - best + even_more << '\n';
}
