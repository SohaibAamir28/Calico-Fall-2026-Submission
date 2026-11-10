# CALICO Fall '26 - Problem 3: verity (INTERACTIVE)
#
# Guess a hidden string of length N over K allowed letters. After a wrong
# guess the judge shows the hidden string with every wrong position as "_".
#
# Idea: positions are independent. Every unknown position simply tries the
# allowed letters in order (A[0], A[1], ...). Each wrong guess eliminates one
# letter at every unknown position, so we need at most K <= 26 guesses
# (the limit is 100).
import sys


def read_string():
    return sys.stdin.readline().strip()


def print_guess(guess):
    # flush=True is required in interactive problems, otherwise the judge
    # never receives our guess and both programs wait forever.
    print(guess, flush=True)


def solve(N, K, A):
    known = [None] * N   # known[i] = confirmed letter at position i (or None)
    ptr = [0] * N        # ptr[i]   = index in A of the letter to try next at i
    while True:
        # Build the guess: confirmed letters stay, others try their next letter.
        guess = []
        for i in range(N):
            if known[i] is not None:
                guess.append(known[i])
            else:
                guess.append(A[min(ptr[i], K - 1)])
        g = "".join(guess)
        print_guess(g)

        resp = read_string()
        if resp == "CORRECT":
            return
        # resp[i] != "_" means our letter at position i was right.
        for i in range(N):
            if known[i] is None:
                if resp[i] != "_":
                    known[i] = resp[i]
                else:
                    ptr[i] += 1      # wrong -> try the next letter next time


def main():
    T = int(read_string())
    for _ in range(T):
        line = read_string()
        while not line:              # skip possible blank lines
            line = read_string()
        N, K = map(int, line.split())
        A = read_string().split()    # the K allowed letters
        solve(N, K, A)


if __name__ == "__main__":
    main()
