#ifndef TIMER_H
#define TIMER_H

#include <gtk/gtk.h>
#include <glib.h>

typedef struct {
    guint total_seconds; // Elapsed seconds
    GtkLabel *label; // label to update
    guint timeout_id; // ID returned by g_timeout_add
} Timer ;

/* Initialize the timer */
void timer_init(Timer *timer, GtkLabel *label);

/* Start or resume the timer */
void timer_start(Timer *timer);

/* Pause the timer */
void timer_pause(Timer *timer);

/* Reset the timer*/
void timer_reset(Timer *timer);

/* Stop the timer completely */
void timer_stop(Timer *timer);

/* Get minutes from elapsed time */
uint get_seconds(Timer *timer);

/* Get seconds from elapsed time */
uint get_minutes(Timer *timer);

#endif