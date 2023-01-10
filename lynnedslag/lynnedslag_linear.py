n, m = map(int, input().split())

a = [False] * n
a[0] = True

for _ in range(m):
    l, r = map(int, input().split())
    r += 1
    a[l] = not a[l]
    if r < n:
        a[r] = not a[r]

total = 0
cur = False
for x in a:
    if x:
        cur = not cur
    total += cur

print(total)