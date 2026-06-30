#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t keep_running = 1;

void handle_signal(int sig) {
    if (sig == SIGINT) {
        printf("\n[signal_demo] Caught SIGINT (Ctrl+C) - cleaning up...\n");
    } else if (sig == SIGTERM) {
        printf("\n[signal_demo] Caught SIGTERM - cleaning up...\n");
    }
    keep_running = 0;
}

int main() {
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    printf("[signal_demo] PID=%d running. Send SIGINT (Ctrl+C) or SIGTERM to stop.\n", getpid());

    while (keep_running) {
        printf("[signal_demo] working...\n");
        sleep(1);
    }

    printf("[signal_demo] Cleanup complete. Exiting cleanly.\n");
    return 0;
}
