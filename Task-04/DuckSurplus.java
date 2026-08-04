import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int t = sc.nextInt();
        StringBuilder sb = new StringBuilder();

        while (t-- > 0) {
            int n = sc.nextInt();
            long[] a = new long[n];
            for (int i = 0; i < n; i++) {
                a[i] = sc.nextLong();
            }

            Deque<Long> stack = new ArrayDeque<>();

            for (int i = 0; i < n; i++) {
                long current = a[i];

                while (!stack.isEmpty() && stack.peek() > current) {
                    current += stack.pop();
                }

                stack.push(current);
            }

            long answer = stack.peek();

            sb.append(answer).append("\n");
        }

        System.out.print(sb);
    }
}
