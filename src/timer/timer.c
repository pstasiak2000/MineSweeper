#include "timer.h"
#include <stdio.h>

static gboolean timer_tick(gpointer user_data) {
    Timer *t = user_data;

    // Increment total seconds
    t->total_seconds++;

    // Compute minutes and seconds for display
    unsigned int minutes = t->total_seconds / 60;
    unsigned int seconds = t->total_seconds % 60;

    char buf[32];
    snprintf(buf, sizeof(buf), "%02u:%02u", minutes, seconds);
    gtk_label_set_text(t->label, buf);

    return G_SOURCE_CONTINUE;
}

void timer_init(Timer *timer, GtkLabel *label) {
    timer->total_seconds = 0;
    timer->label = label;
    timer->timeout_id = 0;
    gtk_label_set_text(label, "00:00");

}

void timer_start(Timer *timer) {
    if(timer->timeout_id == 0)
        timer->timeout_id = g_timeout_add(1000, timer_tick, timer);
}

void timer_pause(Timer *timer) {
    if(timer->timeout_id != 0) {
        g_source_remove(timer->timeout_id);
        timer->timeout_id = 0;
    }
}

void timer_reset(Timer *timer) {
    timer->total_seconds = 0;
    gtk_label_set_text(timer->label, "00:00");
}

void timer_stop(Timer *timer) {
    timer_pause(timer);
    timer_reset(timer);
}