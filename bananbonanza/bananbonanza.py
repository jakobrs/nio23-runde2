import sys

sys.setrecursionlimit(200_000)

n = int(input())
prices = [
    [int(x) for x in input().split()]
    for _ in range(n)
]

cache = {}


def eval(i, city):
    if (i, city) in cache:
        return cache[(i, city)]

    if i == 0:
        return 0

    best = eval(i - 1, city)

    if i >= 3:
        if city == 1:
            best = max(best, eval(i - 3, 0) + prices[i][0])
        elif city == 0:
            best = max(best, eval(i - 3, 1))
    if i >= 2:
        if city == 2:
            best = max(best, eval(i - 2, 0) + prices[i][1])
        elif city == 0:
            best = max(best, eval(i - 2, 2))

    cache[(i, city)] = best
    return best


print(max(
    eval(n - 1, city)
    for city in range(3)
))
