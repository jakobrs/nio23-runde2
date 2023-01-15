n, m = map(int, input().split())

name = input()

forbidden = dict()
for ch in range(ord('A'), ord('Z') + 1):
    forbidden[chr(ch)] = set()

for _ in range(m):
    a, b = input().split()

    forbidden[b].add(a)

dp = [100_000_000] * (n + 1)
dp[0] = 0

for i in range(n):
    x = dp[i]

    here = set()
    incompatible = set()

    for j in range(i + 1, n + 1):
        here.add(name[j - 1])
        for ch in forbidden[name[j - 1]]:
            incompatible.add(ch)

        for ch in here:
            if ch not in incompatible:
                dp[j] = min(dp[j], x + 1)
                break

print(dp[n])
