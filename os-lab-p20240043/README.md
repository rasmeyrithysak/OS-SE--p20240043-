# OS Lab 5 Submission — Threads, Kernel Workers & Process Signals

- **Student Name:** Rasmey Rithysak
- **Student ID:** p20240043

---

## Task Output Source Files

Make sure all of the following files are present in your `lab5/thread_lab/` folder:

- [x] `process_test.c`
- [x] `thread_test.c`
- [x] `multi_thread.c`
- [x] `sleeper_threads.c`
- [x] `signal_handler.c`
- [x] `challenge.c`

---

## Screenshots

Insert your screenshots below.

### Screenshot 1 — Task 1: Process vs Thread (Process Test)
Show the output of `process_test.c`.
![process vs thread 1](images/process_vs_thread_1.png)

---

### Screenshot 2 — Task 1: Process vs Thread (Thread Test)
Show the output of `thread_test.c`.
![process vs thread 2](images/process_vs_thread_2.png)

---

### Screenshot 3 — Task 2: Thread Interaction
Show the output of `multi_thread.c`.
![thread interaction](images/thread_interaction.png)

---

### Screenshot 4 — Task 3: Visualizing 1:1 Thread Mapping
Show the `ps -eLf` output or `/proc/[pid]/task/` directory visualizing the LWP mapping for user threads.
![user to kernel mapping](images/user_kernel_mapping.png)

---

### Screenshot 5 — Task 3: `htop` Kernel Threads
Show `htop` visualizing kernel threads (usually bracketed names like `[kworker]`).
![htop kernel threads](images/htop_kernel_threads.png)

---

### Screenshot 6 — Task 4: Catching `SIGINT`
Show the output of your `signal_handler` program gracefully catching `Ctrl+C`.
![signal handling](images/signal_sigint.png)

---

### Screenshot 7 — Challenge: Graceful Multithreaded Shutdown
Show the output of your `challenge.c` program joining its threads and exiting gracefully after receiving `Ctrl+C`.
![challenge shutdown](images/challenge_shutdown.png)

---

## Answers to Lab Questions

1. **Why do threads share memory while processes do not (by default)?**
   > Threads exist within the same process and share the same virtual address space, so any changes to global variables are visible to all threads. Processes, on the other hand, each get their own separate copy of memory through a mechanism called Copy-on-Write after `fork()`, so modifications in one process do not affect another.

2. **Based on the 1:1 mapping, what is the role of an LWP (Lightweight Process) in Linux?**
   > An LWP (Lightweight Process) is the kernel-level representation of a user thread. In Linux's 1:1 threading model, every user thread created with `pthread_create()` is mapped directly to one LWP, which the kernel scheduler sees and schedules independently on the CPU. This allows true parallel execution on multi-core systems.

3. **Why is it restricted to send signals to kernel threads (e.g., `kthreadd` or `kworker`)?**
   > Kernel threads run entirely in kernel space and perform critical system tasks such as memory management, I/O handling, and scheduling. Allowing user processes to send signals to them could destabilize or crash the entire system. The kernel protects these threads from user-space interference for system stability and security.

4. **Why can't `SIGKILL` (kill -9) be caught by a signal handler?**
   > `SIGKILL` is handled directly by the kernel and never delivered to the process's signal handler. This is by design — it guarantees that a process can always be forcefully terminated regardless of its state, even if it is stuck in an infinite loop or has a buggy signal handler. It serves as an absolute last resort.

---

## Reflection

> The most challenging part of this lab was combining threads and signal handling together in the challenge task. Signal handlers run asynchronously, so using a `volatile` flag to safely communicate between the signal handler and worker threads was an important pattern to understand. These concepts are directly applicable to large-scale applications like web servers (e.g., Nginx uses worker threads to handle multiple requests concurrently) and databases (e.g., PostgreSQL uses signals for graceful shutdown and reload). Without proper thread management and signal handling, these systems would either crash ungracefully or fail to clean up resources properly.
