import java.io.*;

class Solution {

    /**
    * Output the result of adding a certain string to the end of a prompt.
    *
    * @param S Original prompt
    * @return Result of adding the certain string to the end of the prompt
    */
	public static String solve(String S){
        return "";
    }

	static BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
    static PrintWriter out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(System.out)));

    public static void main(String[] args) throws IOException{ 
        int T = Integer.parseInt(in.readLine());


        for (int t = 0; t < T; t++) {
            String prompt = in.readLine();

            String result = solve(prompt);
            out.println(result);
        }
        out.flush();
    } 
}