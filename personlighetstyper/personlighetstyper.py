from typing import List

infinity = 3_000_000_000


class SegTree:
    down: List[int]
    up: List[int]
    offset: int

    def __init__(self, size: int) -> None:
        self.down = [infinity] * (2 * size)
        self.up = [infinity] * (2 * size)
        self.offset = size

    def push(self, index: int):
        cap = self.down[index]
        self.down[index] = infinity

        self.up[2 * index] = min(self.up[2 * index], cap)
        self.up[2 * index + 1] = min(self.up[2 * index + 1], cap)
        self.down[2 * index] = min(self.down[2 * index], cap)
        self.down[2 * index + 1] = min(self.down[2 * index + 1], cap)

    def pull(self, index: int) -> None:
        self.up[index] = min(
            self.up[2 * index],
            self.up[2 * index + 1],
        )

    def update(self, left: int, right: int, cap: int):
        def update_internal(v: int, start: int, end: int):
            if left <= start and end <= right:
                self.down[v] = min(self.down[v], cap)
                self.up[v] = min(self.up[v], cap)
            elif end <= left or right <= start:
                pass
            else:
                self.push(v)
                mid = (start + end) // 2
                update_internal(2 * v, start, mid)
                update_internal(2 * v + 1, mid, end)
                self.pull(v)

        update_internal(1, 0, self.offset)

    def query(self, left: int, right: int) -> int:
        def query_internal(v: int, start: int, end: int):
            if left <= start and end <= right:
                return self.up[v]
            elif end <= left or right <= start:
                return infinity
            else:
                self.push(v)
                mid = (start + end) // 2
                return min(
                    query_internal(2 * v, start, mid),
                    query_internal(2 * v + 1, mid, end),
                )

        return query_internal(1, 0, self.offset)


n, m = map(int, input().strip().split(' '))

name = input().strip()

cantlead = [0] * 26

a_ord = ord('A')
for _ in range(m):
    a, b = input().strip().split(' ')
    cantlead[ord(a) - a_ord] |= 1 << (ord(b) - a_ord)

last = [[0] * (n + 1) for _ in range(26)]
for ch in range(26):
    last[ch][0] = 0
    for i in range(1, n + 1):
        if ord(name[i - 1]) - a_ord == ch:
            last[ch][i] = i
        else:
            last[ch][i] = last[ch][i - 1]

first = [[0] * (n + 1) for _ in range(26)]
for ch in range(26):
    first[ch][n] = n
    for i in range(n - 1, -1, -1):
        if ord(name[i]) - a_ord == ch:
            first[ch][i] = i
        else:
            first[ch][i] = first[ch][i + 1]

rounded_n = 1
while rounded_n < n + 1:
    rounded_n *= 2

dp = SegTree(rounded_n)
dp.update(0, 1, 0)

for i in range(n):
    here = ord(name[i]) - a_ord

    left_bound = 0
    for ch in range(26):
        if cantlead[here] & (1 << ch):
            left_bound = max(left_bound, last[ch][i])

    right_bound = n
    for ch in range(26):
        if cantlead[here] & (1 << ch):
            right_bound = min(right_bound, first[ch][i])

    min_to_this_segment = dp.query(left_bound, i + 1)
    dp.update(i + 1, right_bound + 1, min_to_this_segment + 1)

print(dp.query(n, n + 1))
