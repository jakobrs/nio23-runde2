n, m = map(int, input().split())

name = input()

forbidden = [0 for _ in range(26)]

ord_a = ord('A')
for _ in range(m):
    a, b = input().split()

    forbidden[ord(b) - ord_a] |= 1 << (ord(a) - ord_a)

dp = [100_000_000] * (n + 1)
dp[0] = 0

for i in range(n):
    x = dp[i]

    here = 0
    incompatible = 0

    for j in range(i + 1, n + 1):
        ch = ord(name[j - 1]) - ord_a
        here |= 1 << ch
        incompatible |= forbidden[ch]

        if here & ~incompatible:
            dp[j] = min(dp[j], x + 1)

print(dp[n])
