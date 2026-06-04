import java.util.concurrent.Semaphore;

class Account2 {
    String name;
    int balance;

    Account2(String name, int balance) {
        this.name = name;
        this.balance = balance;
    }
}

public class DeadlockFixed {
    static Semaphore mutex = new Semaphore(1);

    static void transfer(Account2 from, Account2 to, int amount) {
        try {
            mutex.acquire(); // only ONE transfer can run at a time
            try {
                System.out.println(Thread.currentThread().getName()
                        + " acquired mutex | transferring " + amount
                        + " from " + from.name + " to " + to.name);

                Thread.sleep(150);

                from.balance -= amount;
                to.balance += amount;

                System.out.println(Thread.currentThread().getName()
                        + " transfer of " + amount + " completed: "
                        + from.name + "=" + from.balance
                        + "  " + to.name + "=" + to.balance);
            } finally {
                mutex.release(); // always release even if error occurs
            }
        } catch (InterruptedException e) {
            System.out.println(Thread.currentThread().getName() + " was interrupted.");
        }
    }

    public static void main(String[] args) throws InterruptedException {
        Account2 accountA = new Account2("Account-A", 1000);
        Account2 accountB = new Account2("Account-B", 1000);

        System.out.println("=== Bank Transfer Deadlock Prevention Demo ===");
        System.out.println("Starting balances:");
        System.out.println("  Account-A: " + accountA.balance);
        System.out.println("  Account-B: " + accountB.balance);
        System.out.println("  Total:     " + (accountA.balance + accountB.balance));
        System.out.println();

        Thread t1 = new Thread(() ->
                transfer(accountA, accountB, 100), "Thread-1");
        Thread t2 = new Thread(() ->
                transfer(accountB, accountA, 200), "Thread-2");

        t1.start();
        t2.start();

        t1.join();
        t2.join();

        System.out.println();
        System.out.println("Final balances:");
        System.out.println("  Account-A: " + accountA.balance);
        System.out.println("  Account-B: " + accountB.balance);
        System.out.println("  Total:     " + (accountA.balance + accountB.balance));
        System.out.println();
        System.out.println("No deadlock occurred");
    }
}
