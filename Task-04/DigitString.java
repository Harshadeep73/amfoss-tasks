import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int t = sc.nextInt();

        while (t-- > 0) {
            String s = sc.next();

            int oddSeen = 0,deletedTwos = 0,fours = 0;

            for (char c : s.toCharArray()) {
                if (c == '1' || c == '3') {
                    oddSeen++;
                } 
		else if (c == '2') {
                    deletedTwos = Math.min(deletedTwos + 1, oddSeen);
                } 
		else {
                    fours++;
                }
            }

            System.out.println(fours + deletedTwos);
        }

    }
}
