#include <iostream>
#include <vector>

int main() {
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  int n, k;
  std::cin >> n >> k;

  std::vector<bool> a(n, false);
  a[0] = true;

  for (int i = 0; i < k; i++) {
    int l, r;
    std::cin >> l >> r;
    r += 1;

    a[l] = !a[l];
    if (r < n)
      a[r] = !a[r];
  }

  int total = 0;
  bool cur = false;
  for (bool x : a) {
    if (x) {
      cur = !cur;
    }
    total += cur;
  }

  std::cout << total;
}
