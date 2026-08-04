import java.util.*;

public class GTGT {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int t = sc.nextInt();
        StringBuilder sb = new StringBuilder();

        while (t-- > 0) {
            String x = sc.next();
            long y = 1;
            for (int i = 0; i < x.length(); i++) y *= 10;
            y += 1;
            sb.append(y).append("\n");
        }

        System.out.print(sb);
    }
}
