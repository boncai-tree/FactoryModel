"""Генератор стресс-тестов разной размерности."""
import argparse
import random


def generate(M: int, N: int, total: int, out_path: str, seed: int = 42) -> None:
    rng = random.Random(seed)
    lines = [f"{M} {N}"]
    for _ in range(M - 1):
        lines.append(" ".join(str(rng.randint(0, 10000)) for _ in range(N)))

    queues = [0] * N
    for k in range(total):
        queues[rng.randrange(N)] += 1

    for j in range(N):
        types = [rng.randint(0, M - 2) for _ in range(queues[j])]
        lines.append(" ".join([str(queues[j])] + [str(t) for t in types]))

    with open(out_path, "w") as f:
        f.write("\n".join(lines) + "\n")
    print(f"Generated {out_path}: M={M}, N={N}, S={sum(queues)}")


if __name__ == "__main__":
    p = argparse.ArgumentParser()
    p.add_argument("--M", type=int, default=100)
    p.add_argument("--N", type=int, default=100)
    p.add_argument("--S", type=int, default=100_000)
    p.add_argument("--out", default="examples/stress.txt")
    p.add_argument("--seed", type=int, default=42)
    args = p.parse_args()
    generate(args.M, args.N, args.S, args.out, args.seed)
