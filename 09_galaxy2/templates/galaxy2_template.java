import java.io.*;

class Solution {
    /**
     * Return an array of Q strings, the i-th of which is "YES"
     * if Big Ben can travel from start[i] to end[i] within K[i]
     * seconds, and "NO" otherwise.
     *
     * N: the number of rows in the grid
     * M: the number of columns in the grid
     * Q: the number of queries
     * G: the grid, where each tile is 'R', 'B', or '.'
     * start: the {row, column} of the starting tile of each query
     * end: the {row, column} of the ending tile of each query
     * K: the maximum number of seconds allowed for each query
     */
    static String[] solve(int N, int M, int Q, String[] G, int[][] start, int[][] end, int[] K) {
        // YOUR CODE HERE
        return new String[0];
    }

    static BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
    static PrintWriter out = new PrintWriter(System.out);

    public static void main(String[] args) throws IOException {
        int T = Integer.parseInt(in.readLine().trim());
        for (int t = 0; t < T; t++) {
            String[] temp = in.readLine().trim().split(" ");
            int N = Integer.parseInt(temp[0]);
            int M = Integer.parseInt(temp[1]);
            int Q = Integer.parseInt(temp[2]);
            String[] G = new String[N];
            for (int i = 0; i < N; i++) {
                G[i] = in.readLine().trim();
            }
            int[][] start = new int[Q][2];
            int[][] end = new int[Q][2];
            int[] K = new int[Q];
            for (int i = 0; i < Q; i++) {
                temp = in.readLine().trim().split(" ");
                start[i][0] = Integer.parseInt(temp[0]);
                start[i][1] = Integer.parseInt(temp[1]);
                end[i][0] = Integer.parseInt(temp[2]);
                end[i][1] = Integer.parseInt(temp[3]);
                K[i] = Integer.parseInt(temp[4]);
            }

            for (String s : solve(N, M, Q, G, start, end, K)) {
                out.println(s);
            }
        }
        out.flush();
    }
}
