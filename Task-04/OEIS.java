import java.util.*;

public class Main {

    static final int LIMIT = 110000;

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        boolean[] isPrime = new boolean[LIMIT + 1];
        Arrays.fill(isPrime, true);
        isPrime[0] = false;
        isPrime[1] = false;

        for (int i = 2; i * i <= LIMIT; i++) {
            if (isPrime[i]) {
                for (int j = i * i; j <= LIMIT; j += i) {
                    isPrime[j] = false;
                }
            }
        }

        ArrayList<Integer> primes = new ArrayList<>();
        for (int i = 2; i <= LIMIT; i++) {
            if (isPrime[i]) {
                primes.add(i);
            }
        }

        int t = sc.nextInt();

        while (t-- > 0) {
            int n = sc.nextInt();

            if (n == 2) {
                System.out.println("1 2");
                continue;
            }

            StringBuilder ans = new StringBuilder();

            ans.append("1 ");
            ans.append("2 ");

            for (int i = 0; i < n - 2; i++) {
                long val = 1L * primes.get(i) * primes.get(i + 1);
                ans.append(val);

                if (i != n - 3)
                    ans.append(" ");
            }

            System.out.println(ans);
        }

        sc.close();
    }
}
