#ifndef TIMER_H
#define TIMER_H

void display_time(int hours, int minutes, int seconds, char *color);
void display_message(const char *message, char *color);
void start_countdown(int total_seconds);
int get_user_input(int *hours, int *minutes, int *seconds);

#endif /* TIMER_H */
