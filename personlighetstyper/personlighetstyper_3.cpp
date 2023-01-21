#include <iostream>
#include <vector>

#define INFINITY 1000000

struct SegTree {
  std::vector<int64_t> down;
  std::vector<int64_t> up;
  int64_t offset;

  SegTree(int64_t size)
      : offset(size), down(2 * size, INFINITY), up(2 * size + 1, INFINITY) {}

  void push(int64_t index) {
    int64_t cap = down[index];
    down[index] = INFINITY;

    up[2 * index] = std::min(up[2 * index], cap);
    up[2 * index + 1] = std::min(up[2 * index + 1], cap);
    down[2 * index] = std::min(down[2 * index], cap);
    down[2 * index + 1] = std::min(down[2 * index + 1], cap);
  }

  void pull(int64_t index) {
    up[index] = std::min(up[2 * index], up[2 * index + 1]);
  }

  void update(int64_t left, int64_t right, int64_t cap) {
    update_internal(left, right, cap, 1, 0, offset);
  }

  int query(int64_t left, int64_t right) {
    return query_internal(left, right, 1, 0, offset);
  }

 private:
  void update_internal(int64_t left, int64_t right, int64_t cap, int64_t v,
                       int64_t from, int64_t to) {
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

  int query_internal(int64_t left, int64_t right, int64_t v, int64_t from,
                     int64_t to) {
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

    cantlead[a - 'A'] |= 1 << (int32_t)(b - 'A');
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

  /*
    Strategy:
      dp[i] = minste nødvendige avdelinger for å dekke de første `i` personene
      dp støtter range query minimum og range set maximum i log n tid

      for hver posisjon i in 0..=n:
        La l og r være henholdsvis laveste og høyeste indeks slik at [l, r) kan ledes av person i (O(1))

        La x være den minste mengden avdelinger som trenges for å "nå" [l, r) (dvs. min(dp[k] for k in l..i)) (O(log n))
        Utfør setmax over [i + 1, r] med verdien x + 1 (O(log n))

      Skriv ut dp[n]

      Fullstendig tidskomplksitet: O(n log n)
  */

  int64_t rounded_n = 1;
  while (rounded_n < n + 1) {
    rounded_n *= 2;
  }
  SegTree dp(rounded_n);
  dp.update(0, 1, 0);

  for (int64_t i = 0; i < n; i++) {
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

    int64_t min_to_this_segment = dp.query(left_bound, i + 1);
    dp.update(i + 1, right_bound + 1, min_to_this_segment + 1);
  }

  std::cout << dp.query(n, n + 1);
}
