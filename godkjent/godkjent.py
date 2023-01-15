from typing import List, Optional
import sys

sys.setrecursionlimit(300_000)


class Node:
    superordinate: Optional["Node"]
    subordinates: List["Node"]
    jump: Optional["Node"]
    dist: int
    lower: int
    upper: int

    def __init__(self):
        self.superordinate = None
        self.subordinates = []
        self.jump = None
        self.dist = -1
        self.lower = 0
        self.upper = 0


n, m = map(int, input().split())

nodes = [Node() for _ in range(n)]

lst = [int(x) for x in input().split()]
for i in range(1, n):
    x = lst[i - 1]
    nodes[i].superordinate = nodes[x]
    nodes[x].subordinates.append(nodes[i])


def annotate_rank():
    rank = 0

    def go(node: Node):
        nonlocal rank
        node.lower = rank

        for child in node.subordinates:
            go(child)

        rank += 1
        node.upper = rank

    go(nodes[0])


def add_jump_links(node: Node):
    node.jump = node.superordinate
    node.dist = 1

    if node.superordinate is not None:
        if node.superordinate.jump is not None:
            if node.superordinate.dist == node.superordinate.jump.dist:
                node.jump = node.superordinate.jump.jump
                node.dist = node.superordinate.dist * 2 + 1

    for child in node.subordinates:
        add_jump_links(child)


annotate_rank()
add_jump_links(nodes[0])


def find_ancestor(w: Node, g: int):
    parent = w.superordinate

    if parent.upper - parent.lower > g:
        return w

    if w.jump is not None:
        if w.jump.upper - w.jump.lower <= g:
            return find_ancestor(w.jump, g)
    return find_ancestor(parent, g)


diffs = [0] * (n + 1)

for _ in range(m):
    w, g = map(int, input().split())

    if g == n:
        diffs[0] += 1
    else:
        ancestor = find_ancestor(nodes[w], g)
        parent = ancestor.superordinate

        g1 = g - (ancestor.upper - ancestor.lower)

        if ancestor.lower - parent.lower >= g1:
            diffs[ancestor.lower] += 1
            diffs[ancestor.upper] -= 1
            diffs[parent.lower] += 1
            diffs[parent.lower + g1] -= 1
        else:
            diffs[parent.lower] += 1
            diffs[parent.lower + g] -= 1

result = []
prev = 0
for x in diffs:
    prev += x
    result.append(prev)

print(" ".join(str(result[nodes[i].upper - 1]) for i in range(n)))
