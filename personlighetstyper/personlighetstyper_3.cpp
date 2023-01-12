#include <iostream>
#include <vector>

#define INFINITY 1000000

struct SegTree {
  std::vector<int> down;
  std::vector<int> up;
  int offset;

  SegTree(int size)
      : offset(size), down(2 * size, INFINITY), up(2 * size + 1, INFINITY) {}

  void push(int index) {
    int cap = down[index];
    down[index] = INFINITY;

    up[2 * index] = std::min(up[2 * index], cap);
    up[2 * index + 1] = std::min(up[2 * index + 1], cap);
    down[2 * index] = std::min(down[2 * index], cap);
    down[2 * index + 1] = std::min(down[2 * index + 1], cap);
  }

  void pull(int index) {
    up[index] = std::min(up[2 * index], up[2 * index + 1]);
  }

  void update(int left, int right, int cap) {
    update_internal(left, right, cap, 1, 0, offset);
  }

  int query(int left, int right) {
    return query_internal(left, right, 1, 0, offset);
  }

 private:
  void update_internal(int left, int right, int cap, int v, int from, int to) {
    if (left <= from && to <= right) {
      down[v] = std::min(down[v], cap);
      up[v] = std::min(up[v], cap);
    } else if (to <= left || right <= from) {
      return;
    } else {
      push(v);
      int mid = (from + to) / 2;
      update_internal(left, right, cap, 2 * v, from, mid);
      update_internal(left, right, cap, 2 * v + 1, mid, to);
      pull(v);
    }
  }

  int query_internal(int left, int right, int v, int from, int to) {
    if (left <= from && to <= right) {
      return up[v];
    } else if (to <= left || right <= from) {
      return INFINITY;
    } else {
      push(v);
      int mid = (from + to) / 2;
      return std::min(query_internal(left, right, 2 * v, from, mid),
                      query_internal(left, right, 2 * v + 1, mid, to));
    }
  }
};

int main() {
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  int n, m;
  std::cin >> n >> m;

  std::string name;
  std::cin >> name;

  if (name.size() != n) {
    return -1;
  }

  int32_t cantlead[26];
  for (int i = 0; i < 26; i++) {
    cantlead[i] = 0;
  }

  for (int i = 0; i < m; i++) {
    char a, b;
    std::cin >> a >> b;
    cantlead[a - 'A'] |= 1 << (b - 'A');
  }

  // last[ch][i] = index of last `ch` in range 0..i, + 1
  std::vector<int> last[26];
  for (int ch = 0; ch < 26; ch++) {
    last[ch] = std::vector<int>(n + 1, 0);
    last[ch][0] = 0;
    for (int i = 1; i <= n; i++) {
      if (name[i - 1] - 'A' == ch) {
        last[ch][i] = i;
      } else {
        last[ch][i] = last[ch][i - 1];
      }
    }
  }

  // first[ch][i] = index of first `ch` in range i..n
  std::vector<int> first[26];
  for (int ch = 0; ch < 26; ch++) {
    first[ch] = std::vector<int>(n + 1, 0);
    first[ch][n] = n;
    for (int i = n - 1; i >= 0; i--) {
      if (name[i] - 'A' == ch) {
        first[ch][i] = i;
      } else {
        first[ch][i] = first[ch][i + 1];
      }
    }
  }

  /*

    Strategy:
      dp[i] = minste nødvendige avdelinger for å dekke de første `i` personene
      dp støtter range query minimum og range set maximum i log n tid

      for hver posisjon i in 0..=n:
        La l og r være henholdsvis laveste og høyeste indeks slik at [l, r) kan ledes av person i (O(1))

        La x være den minste mengden avdelinger som trenges for å "nå" [l, r) (dvs. min(dp[k] for k in l..i)) (O(log n))
        Utfør setmax over [i + 1, r) med verdien x + 1 (O(log n))

      Skriv ut dp[n]

      Fullstendig tidskomplksitet: O(n log n)
  */

  int rounded_n = 1;
  while (rounded_n < n + 1) {
    rounded_n *= 2;
  }
  SegTree dp(rounded_n);
  dp.update(0, 1, 0);

  for (int i = 0; i < n; i++) {
    int here = name[i] - 'A';

    int left_bound = 0;
    for (int ch = 0; ch < 26; ch++) {
      if (cantlead[here] & (1 << ch)) {
        left_bound = std::max(left_bound, last[ch][i]);
      }
    }
    int right_bound = n;
    for (int ch = 0; ch < 26; ch++) {
      if (cantlead[here] & (1 << ch)) {
        right_bound = std::min(right_bound, first[ch][i]);
      }
    }
    // std::cout << left_bound << ".." << right_bound << '\n';

    int min_to_this_segment = dp.query(left_bound, i + 1);
    // std::cout << "min to here: " << min_to_this_segment << "\n";
    dp.update(i + 1, right_bound + 1, min_to_this_segment + 1);
  }

  std::cout << dp.query(n, n + 1);
}
