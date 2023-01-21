#include <iostream>
#include <vector>

int main() {
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  int n, m;
  std::cin >> n >> m;

  std::string name;
  std::cin >> name;

  int32_t forbidden[26];
  for (int i = 0; i < 26; i++) {
    forbidden[i] = 0;
  }

  for (int i = 0; i < m; i++) {
    char a, b;
    std::cin >> a >> b;
    
    forbidden[b - 'A'] |= 1 << (a - 'A');
  }

  std::vector<int64_t> dp(n + 1, 1000000000ll);
  dp[0] = 0;

  for (int i = 0; i < n; i++) {
    int64_t x = dp[i];

    int32_t here = 0;
    int32_t incompatible = 0;
    for (int j = i + 1; j <= n; j++) {
      char ch = name[j - 1];

      here |= 1 << (int32_t)(name[j - 1] - 'A');
      incompatible |= forbidden[name[j - 1] - 'A'];

      if (~incompatible & here) {
        dp[j] = std::min(dp[j], x + 1);
      }
    }
  }

  std::cout << dp[n];
}
