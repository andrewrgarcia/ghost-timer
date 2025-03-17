#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include "timer.h"

#define MAX_CMD_LENGTH 512

void display_time(int hours, int minutes, int seconds, char *color) {
    char cmd[MAX_CMD_LENGTH];
    pid_t pid;
    
    // Format hh:mm:ss
    char time_str[9];
    sprintf(time_str, "%02d:%02d:%02d", hours, minutes, seconds);
    
    // Kill any previous osd_cat processes
    system("pkill -f \"osd_cat\" 2>/dev/null");
    
    // osd_cat
    snprintf(cmd, MAX_CMD_LENGTH, 
             "echo \"%s\" | osd_cat --pos=top --align=right --offset=50 "
             "--font=\"-misc-fixed-bold-r-normal--34-*-*-*-*-*-iso8859-1\" "
             "--color=%s --shadow=2 --outline=2 --delay=9999 &",
             time_str, color);
    
    // Execute command
    pid = fork();
    if (pid == 0) {
        // Child process
        system(cmd);
        exit(0);
    }
}

void display_message(const char *message, char *color) {
    char cmd[MAX_CMD_LENGTH];

    // Kill any previous osd_cat processes
    system("pkill -f \"osd_cat\" 2>/dev/null");

    // osd_cat
    snprintf(cmd, MAX_CMD_LENGTH, 
             "echo \"%s\" | osd_cat --pos=top --align=right --offset=50 "
             "--font=\"-misc-fixed-bold-r-normal--34-*-*-*-*-*-iso8859-1\" "
             "--color=%s --shadow=2 --outline=2 --delay=5 &",
             message, color);

    // Execute command
    system(cmd);
}

void start_countdown(int total_seconds) {
    int hours, minutes, seconds;
    int total_time = total_seconds;  
    int threshold_25 = total_time / 4; 
    int threshold_10 = total_time / 10; 

    while (total_seconds >= 0) {
        hours = total_seconds / 3600;
        minutes = (total_seconds % 3600) / 60;
        seconds = total_seconds % 60;

        // Change color based on remaining time
        char *color = "white";  
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

    display_message("Time's Up!", "red");
    sleep(2);

    // Clear the OSD completely
    system("pkill -f \"osd_cat\" 2>/dev/null");
}

int get_user_input(int *hours, int *minutes, int *seconds) {
    printf("Enter hours: ");
    if (scanf("%d", hours) != 1) {
        return 0;
    }
    
    printf("Enter minutes: ");
    if (scanf("%d", minutes) != 1) {
        return 0;
    }
    
    printf("Enter seconds: ");
    if (scanf("%d", seconds) != 1) {
        return 0;
    }
    
    return 1;
}
