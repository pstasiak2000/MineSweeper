#ifndef ACTIONS_H
#define ACTIONS_H

#include <gtk/gtk.h>
#include "base.h"

/* --- Game state pointer example --- */
static const char *current_difficulty = "easy";

/* --- Static array of difficulty levels --- */
static const char *difficulty_levels[] = { "easy", "medium", "hard" };

/* Prints the victory message when the game is won */
void victory_message(GtkWindow *window, Timer *timer);

/* Register all of the application actions here */ 
void register_app_actions(GtkApplication *app, AppCtx *ctx);

#endif
