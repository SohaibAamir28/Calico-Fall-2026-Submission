import java.io.*;

class Solution {
	/**
	 * Return the maximum possible value of X, Big Ben's matcha enjoyment.
	 *
	 * N: the days in Big Ben's life
	 * L: the lower bound of enjoyment from each store
	 * R: the upper bound of enjoyment from each store
	 */ 
    static int solve(int N, int[] L, int[] R) {
        // YOUR CODE HERE
        return -1;
    }

    static BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
    static PrintWriter out = new PrintWriter(System.out);

    public static void main(String[] args) throws IOException {
        int T = Integer.parseInt(in.readLine());

        for (int i = 0; i < T; i++) {
            int N = Integer.parseInt(in.readLine());

            int[] L = new int[N];
            String[] temp = in.readLine().split(" ");
            for (int j = 0; j < N; j++) {
                L[j] = Integer.parseInt(temp[j]);
            }

            int[] R = new int[N];
            String[] req = in.readLine().split(" ");
            for (int j = 0; j < N; j++) {
                R[j] = Integer.parseInt(req[j]);
            }

            out.println(solve(N, L, R));
        }
        out.flush();
    }
}
