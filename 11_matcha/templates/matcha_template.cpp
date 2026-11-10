#include <iostream>
#include <vector>
using namespace std;

/**
 * Return the maximum possible value of X, Big Ben's matcha enjoyment.
 *
 * N: the days in Big Ben's life
 * L: the lower bound of enjoyment from each store
 * R: the upper bound of enjoyment from each store
 */ 
int solve(int N, vector<int> L, vector<int> R) {
	// YOUR CODE HERE
	return -1;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		vector<int> L(N), R(N);
		for (int& i : L) cin >> i;
		for (int& i : R) cin >> i;
		cout << solve(N, L, R) << '\n';
	}
	return 0;
}
