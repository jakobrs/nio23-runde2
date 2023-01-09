n, m = map(int, input().split())

a = [True] * n

for _ in range(m):
    l, r = map(int, input().split())
    a[l:r+1] = [not x for x in a[l:r+1]]
    # Note: This is much slower for subtask 2
    # a[:] = a[:l] + [not x for x in a[l:r+1]] + a[r+1:]

print(sum(a))
