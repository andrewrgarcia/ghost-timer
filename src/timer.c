#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <termios.h>
#include <sys/select.h>
#include "timer.h"
#include <unistd.h>  

extern char *optarg;

#define MAX_CMD_LENGTH 512

static int paused = 0;
static int running = 1;

void toggle_pause() {
    paused = !paused;
}

void handle_sigint(int signum) {
    (void)signum;
    running = 0;
    system("pkill -f \"osd_cat\" 2>/dev/null");
    exit(0);
}

char check_keypress() {
    struct termios oldt, newt;
    fd_set readfds;
    struct timeval timeout;
    char ch = 0;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000;
    
    if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout) > 0) {
        read(STDIN_FILENO, &ch, 1);
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void display_time(int hours, int minutes, int seconds, char *color) {
    char cmd[MAX_CMD_LENGTH];
    pid_t pid;
    
    char time_str[9];
    sprintf(time_str, "%02d:%02d:%02d", hours, minutes, seconds);
    
    system("pkill -f \"osd_cat\" 2>/dev/null");
    
    snprintf(cmd, MAX_CMD_LENGTH, 
             "echo \"%s\" | osd_cat --pos=top --align=right --offset=50 "
             "--font=\"-misc-fixed-bold-r-normal--34-*-*-*-*-*-iso8859-1\" "
             "--color=%s --shadow=2 --outline=2 --delay=9999 &",
             time_str, color);
    
    pid = fork();
    if (pid == 0) {
        system(cmd);
        exit(0);
    }
}

void display_message(const char *message, char *color) {
    char cmd[MAX_CMD_LENGTH];
    system("pkill -f \"osd_cat\" 2>/dev/null");
    snprintf(cmd, MAX_CMD_LENGTH, 
             "echo \"%s\" | osd_cat --pos=top --align=right --offset=50 "
             "--font=\"-misc-fixed-bold-r-normal--34-*-*-*-*-*-iso8859-1\" "
             "--color=%s --shadow=2 --outline=2 --delay=5 &",
             message, color);
    system(cmd);
}

void start_countdown(int total_seconds) {
    int hours, minutes, seconds;
    int total_time = total_seconds;  
    int threshold_50 = total_time / 2;
    int threshold_25 = total_time / 4; 
    int threshold_10 = total_time / 10;

    signal(SIGINT, handle_sigint);

    // Display controls before starting
    printf("\n=======================================\n");
    printf("  ghost-timer started! ⏳👻\n");
    printf("  Press 'P' to pause/resume | Press Ctrl+C to exit\n");
    printf("=======================================\n\n");

    while (total_seconds >= 0 && running) {
        char ch = check_keypress();
        if (ch == 'p' || ch == 'P') {
            toggle_pause();
        }

        while (paused && running) {
            sleep(1);
            ch = check_keypress();
            if (ch == 'p' || ch == 'P') {
                toggle_pause();
            }
        }
        
        hours = total_seconds / 3600;
        minutes = (total_seconds % 3600) / 60;
        seconds = total_seconds % 60;
        
        char *color = "cyan";
        if (total_seconds <= threshold_50) {
            color = "white";
        }
        if (total_seconds <= threshold_25) {
            color = "orange";
        }
        if (total_seconds <= threshold_10) {
            color = "red";
        }
        
        display_time(hours, minutes, seconds, color);
        
        sleep(1);
        total_seconds--;
    }
    
    if (running) {
        display_message("Time's Up!", "red");
        sleep(2);
    }
    
    system("pkill -f \"osd_cat\" 2>/dev/null");
    exit(0);
}


int get_user_input(int argc, char *argv[], int *hours, int *minutes, int *seconds) {
    *hours = 0;
    *minutes = 0;
    *seconds = 0;
    
    int opt;
    int flag_used = 0;  // Flag to check if any -h, -m, -s is used

    while ((opt = getopt(argc, argv, "h:m:s:")) != -1) {
        switch (opt) {
            case 'h':
                *hours = atoi(optarg);
                flag_used = 1;
                break;
            case 'm':
                *minutes = atoi(optarg);
                flag_used = 1;
                break;
            case 's':
                *seconds = atoi(optarg);
                flag_used = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-h hours] [-m minutes] [-s seconds]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Always print the header
    printf("\n===============================================\n");
    printf("  ghost-timer by Andrew Garcia, 2025\n");
    printf("  https://github.com/andrewrgarcia/ghost-timer\n");
    printf("===============================================\n\n");

    // If no flags were used, prompt the user for input
    if (!flag_used) {
        printf("Enter hours: ");
        scanf("%d", hours);
        printf("Enter minutes: ");
        scanf("%d", minutes);
        printf("Enter seconds: ");
        scanf("%d", seconds);
    }

    return 1;
}



