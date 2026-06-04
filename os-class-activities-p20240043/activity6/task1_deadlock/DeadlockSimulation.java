import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicBoolean;

class Account {
    String name;
    int balance;
    Semaphore lock = new Semaphore(1);

    Account(String name, int balance) {
        this.name = name;
        this.balance = balance;
    }
}

class Transfer {
    static AtomicBoolean t1Done = new AtomicBoolean(false);
    static AtomicBoolean t2Done = new AtomicBoolean(false);

    static void transfer(Account from, Account to, int amount, AtomicBoolean done) {
        try {
            System.out.println(Thread.currentThread().getName()
                    + " trying to lock FROM " + from.name);
            from.lock.acquire();
            System.out.println(Thread.currentThread().getName()
                    + " locked FROM " + from.name
                    + " | now waiting for " + to.name);

            Thread.sleep(150);

            System.out.println(Thread.currentThread().getName()
                    + " trying to lock TO " + to.name);
            to.lock.acquire(); // <-- DEADLOCK HAPPENS HERE
            System.out.println(Thread.currentThread().getName()
                    + " locked TO " + to.name);

            from.balance -= amount;
            to.balance += amount;

            System.out.println(Thread.currentThread().getName()
                    + " transfer of " + amount + " completed");

            to.lock.release();
            from.lock.release();
            done.set(true);
        } catch (InterruptedException e) {
            System.out.println(Thread.currentThread().getName() + " was interrupted.");
        }
    }
}

public class DeadlockSimulation {
    public static void main(String[] args) throws InterruptedException {
        Account account1 = new Account("Account-A", 1000);
        Account account2 = new Account("Account-B", 1000);

        System.out.println("=== Bank Transfer Deadlock Demo ===");
        System.out.println("Starting balances:");
        System.out.println("  Account-A: " + account1.balance);
        System.out.println("  Account-B: " + account2.balance);
        System.out.println("  Total:     " + (account1.balance + account2.balance));
        System.out.println();

        Thread t1 = new Thread(() ->
                Transfer.transfer(account1, account2, 100, Transfer.t1Done),
                "Thread-1"
        );
        Thread t2 = new Thread(() ->
                Transfer.transfer(account2, account1, 200, Transfer.t2Done),
                "Thread-2"
        );

        t1.setDaemon(true);
        t2.setDaemon(true);

        t1.start();
        t2.start();

        // Watchdog: wait 3 seconds then check
        long deadline = System.currentTimeMillis() + 3000;
        while (System.currentTimeMillis() < deadline) {
            if (Transfer.t1Done.get() && Transfer.t2Done.get()) break;
            Thread.sleep(200);
        }

        System.out.println();
        if (!Transfer.t1Done.get() || !Transfer.t2Done.get()) {
            System.out.println("Deadlock detected: transactions are stuck");
            System.out.println();
            System.out.println("Lock state:");
            System.out.println("  Thread-1 is waiting for Account-B (held by Thread-2)");
            System.out.println("  Thread-2 is waiting for Account-A (held by Thread-1)");
            System.out.println();
            System.out.println("Deadlock conditions met:");
            System.out.println("  [1] Mutual exclusion : each account locked by one thread at a time");
            System.out.println("  [2] Hold and wait    : each thread holds one lock and waits for the other");
            System.out.println("  [3] No preemption    : locks are not forcibly taken away");
            System.out.println("  [4] Circular wait    : Thread-1->Account-B->Thread-2->Account-A->Thread-1");
        } else {
            System.out.println("Both transfers completed (no deadlock this run).");
        }
    }
}
