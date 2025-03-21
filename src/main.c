#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "timer.h"

void cleanup(int signum) {
    (void)signum; // Suppress warning
    system("pkill -f \"osd_cat\" 2>/dev/null");
    exit(0);
}

int main(int argc, char *argv[]) {
    int hours, minutes, seconds, total_seconds;
    
    signal(SIGINT, handle_sigint);
    
    if (!get_user_input(argc, argv, &hours, &minutes, &seconds)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    total_seconds = hours * 3600 + minutes * 60 + seconds;
    
    if (total_seconds <= 0) {
        fprintf(stderr, "Invalid time duration!\n");
        return 1;
    }
    
    start_countdown(total_seconds);
    
    system("pkill -f \"osd_cat\" 2>/dev/null");
    
    return 0;
}
