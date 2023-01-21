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
  for (int i = 0; i + 1 < n; i++) {
    if (name[i] != name[i + 1]) {
      departments += 1;
    }
  }

  std::cout << departments << '\n';
}
