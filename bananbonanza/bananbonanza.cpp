#include <iostream>
#include <vector>

#ifdef PRINT_DP_TABLE
#include <iomanip>
#endif

using i64 = int64_t;

int main() {
  std::cin.tie(nullptr);
  std::ios_base::sync_with_stdio(false);

  int n;
  std::cin >> n;

  std::vector<i64> b(n), o(n);
  for (int i = 0; i < n; i++)
    std::cin >> b[i] >> o[i];

  // 0 -> kristiansand
  // 1 -> bergen
  // 2 -> oslo
  i64 dp[n][3];
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < 3; j++) {
      dp[i][j] = 0;
    }
  }

#ifdef PRINT_DP_TABLE
  std::cout << " 0   0   0   0\n";
#endif

  i64 best = 0;
  for (int i = 1; i < n; i++) {
    dp[i][0] = dp[i - 1][0];
    dp[i][1] = dp[i - 1][1];
    dp[i][2] = dp[i - 1][2];

    if (i >= 3) {
      dp[i][0] = std::max(dp[i][0], dp[i - 3][1]);
      dp[i][1] = std::max(dp[i][1], dp[i - 3][0] + b[i]);
    }
    if (i >= 2) {
      dp[i][0] = std::max(dp[i][0], dp[i - 2][2]);
      dp[i][2] = std::max(dp[i][2], dp[i - 2][0] + o[i]);
    }

#ifdef PRINT_DP_TABLE
    std::cout << std::setw(2) << i << ' ' << std::setw(3) << dp[i][0] << ' '
              << std::setw(3) << dp[i][1] << ' ' << std::setw(3) << dp[i][2]
              << '\n';
#endif
  }

  std::cout << std::max(std::max(dp[n - 1][0], dp[n - 1][1]), dp[n - 1][2]);
}
