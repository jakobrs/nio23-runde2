#include <iostream>
#include <vector>

int main() {
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  int n, m;
  std::cin >> n >> m;

  std::string name;
  std::cin >> name;

  for (int i = 0; i < m; i++) {
    char a, b;
    std::cin >> a >> b;
  }

  int departments = 1;
  char prev = name[0];
  for (int i = 1; i < n; i++) {
    if (name[i] != prev) {
      departments += 1;
      prev = name[i];
    }
  }

  std::cout << departments << '\n';
}
