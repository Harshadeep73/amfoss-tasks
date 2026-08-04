import java.util.*;

public class Papyrus {

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int t = sc.nextInt();

        while (t-- > 0) {
            int n = sc.nextInt();
            long c = sc.nextLong();

            long[] a = new long[n];
            long[] b = new long[n];

            for (int i = 0; i < n; i++)
                a[i] = sc.nextLong();

            for (int i = 0; i < n; i++)
                b[i] = sc.nextLong();

            long ans = Long.MAX_VALUE;

            boolean ok = true;
            long cost = 0;

            for (int i = 0; i < n; i++) {
                if (a[i] < b[i]) {
                    ok = false;
                    break;
                }
                cost += a[i] - b[i];
            }

            if (ok)
                ans = Math.min(ans, cost);

            long[] sa = a.clone();
            long[] sb = b.clone();

            Arrays.sort(sa);
            Arrays.sort(sb);

            ok = true;
            cost = c;

            for (int i = 0; i < n; i++) {
                if (sa[i] < sb[i]) {
                    ok = false;
                    break;
                }
                cost += sa[i] - sb[i];
            }

            if (ok)
                ans = Math.min(ans, cost);

            if (ans == Long.MAX_VALUE)
                System.out.println(-1);
            else
                System.out.println(ans);
        }

        sc.close();
    }
}
