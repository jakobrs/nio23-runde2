#include <iostream>
#include <vector>

struct SegTree {
  std::vector<bool> values;
  int n;

  SegTree(int n) : values(2 * n, false), n(n) {}

  void update(int left, int right) { update_internal(left, right, 1, 0, n); }

  int count() { return count_internal(1, false); }

private:
  void update_internal(int left, int right, int v, int start, int end) {
    if (left <= start && end <= right) {
      values[v] = !values[v];
    } else if (end <= left || right <= start) {
      return;
    } else {
      int mid = (start + end) / 2;
      update_internal(left, right, 2 * v, start, mid);
      update_internal(left, right, 2 * v + 1, mid, end);
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

int main() {
  int n, k;
  std::cin >> n >> k;

  int rounded_n = 1;
  while (rounded_n < n) {
    rounded_n *= 2;
  }

  SegTree st(rounded_n);

  for (int i = 0; i < k; i++) {
    int l, r;
    std::cin >> l >> r;
    r += 1;

    st.update(l, r);
  }

  std::cout << n - st.count();
}
