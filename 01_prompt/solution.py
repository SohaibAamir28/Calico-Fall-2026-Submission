# CALICO Fall '26 - Problem 1: prompt
# Append a fixed phrase to the prompt depending on which keyword it contains.


def solve(S: str) -> str:
    # The statement guarantees at most ONE of the three keywords appears,
    # so the order of these checks does not matter.
    if "write_me" in S:
        return S + "make_it_sound_human"
    if "what_is" in S:
        return S + "do_not_hallucinate"
    if "solve" in S:
        return S + "make_no_mistakes"
    # No keyword found -> be polite.
    return S + "thank_you"


def main():
    T = int(input())                 # number of test cases
    for _ in range(T):
        S = input().strip()          # the prompt string
        print(solve(S))


if __name__ == "__main__":
    main()
