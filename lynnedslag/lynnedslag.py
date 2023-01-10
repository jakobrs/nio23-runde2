from typing import List


class SegmentTree:
    offset: int
    values: List[bool]

    def __init__(self, size: int) -> None:
        self.values = [False] * (2 * size)
        self.offset = size

    def update(self, left: int, right: int) -> None:
        def update_internal(v: int, start: int, end: int) -> None:
            if left <= start and end <= right:
                self.values[v] = not self.values[v]
            elif end <= left or right <= start:
                pass
            else:
                mid = (start + end) // 2
                update_internal(2 * v, start, mid)
                update_internal(2 * v + 1, mid, end)

        update_internal(1, 0, self.offset)

    def count(self) -> int:
        def count_internal(v: int, invert: bool) -> int:
            here = invert ^ self.values[v]
            if v >= self.offset:
                return here
            else:
                return count_internal(2 * v, here) + count_internal(2 * v + 1, here)

        return count_internal(1, False)


n, m = map(int, input().split())

rounded_n = 1
while rounded_n < n:
    rounded_n *= 2

st = SegmentTree(rounded_n)

for _ in range(m):
    l, r = map(int, input().split())
    st.update(l, r + 1)

print(n - st.count())
