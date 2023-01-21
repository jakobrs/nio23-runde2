#include <iostream>
#include <vector>

#define INFINITY 100000000ll

int main() {
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  int64_t n, m;
  std::cin >> n >> m;

  std::string name;
  std::cin >> name;

  int32_t cantlead[26];
  for (int i = 0; i < 26; i++) {
    cantlead[i] = 0;
  }

  for (int64_t i = 0; i < m; i++) {
    char a, b;
    std::cin >> a >> b;

    cantlead[a - 'A'] |= 1 << (b - 'A');
  }

  // last[ch][i] = index of last `ch` in range 0..i, + 1
  std::vector<int64_t> last[26];
  for (int ch = 0; ch < 26; ch++) {
    last[ch] = std::vector<int64_t>(n + 1, 0);
    last[ch][0] = 0;

    for (int64_t i = 1; i <= n; i++) {
      if (name[i - 1] - 'A' == ch) {
        last[ch][i] = i;
      } else {
        last[ch][i] = last[ch][i - 1];
      }
    }
  }

  // first[ch][i] = index of first `ch` in range i..n
  std::vector<int64_t> first[26];
  for (int ch = 0; ch < 26; ch++) {
    first[ch] = std::vector<int64_t>(n + 1, 0);
    first[ch][n] = n;

    for (int64_t i = n - 1; i >= 0; i--) {
      if (name[i] - 'A' == ch) {
        first[ch][i] = i;
      } else {
        first[ch][i] = first[ch][i + 1];
      }
    }
  }

  struct Interval {
    int64_t from;
    int64_t to;
  };
  std::vector<Interval> intervals;
  intervals.reserve(n);

  for (int64_t i = 0; i < n; i++) {
    if (!('A' <= name[i] && name[i] <= 'Z')) {
      return -1;
    }

    int here = name[i] - 'A';

    int64_t left_bound = 0;
    for (int ch = 0; ch < 26; ch++) {
      if (cantlead[here] & (1 << ch)) {
        left_bound = std::max(left_bound, last[ch][i]);
      }
    }
    int64_t right_bound = n;
    for (int ch = 0; ch < 26; ch++) {
      if (cantlead[here] & (1 << ch)) {
        right_bound = std::min(right_bound, first[ch][i]);
      }
    }

    intervals.push_back(Interval{.from = left_bound, .to = right_bound});
  }

  // Constructs a "directed interval graph"
  std::vector<std::vector<int64_t>> adj(n);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < i; j++) {
      if (intervals[j].to >= intervals[i].from) {
        adj[j].push_back(i);
      }
    }
  }

  // The interval graph is conveniently topologically sorted
  std::vector<int64_t> dists(n, INFINITY);
  for (int64_t i = 0; i < n; i++) {
    if (intervals[i].from == 0) {
      dists[i] = 1;
    }
  }
  for (int64_t i = 0; i < n; i++) {
    int64_t dist = dists[i];
    for (int64_t j : adj[i]) {
      dists[j] = std::min(dists[j], dist + 1);
    }
  }
  int64_t best = INFINITY;
  for (int64_t i = 0; i < n; i++) {
    if (intervals[i].to == n) {
      best = std::min(best, dists[i]);
    }
  }

  std::cout << best;
}
