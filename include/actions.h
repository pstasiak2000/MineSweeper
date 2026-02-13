#ifndef ACTIONS_H
#define ACTIONS_H

#include <gtk/gtk.h>
#include "base.h"

const GActionEntry *get_actions(size_t *n);

/* --- Game state pointer example --- */
static const char *current_difficulty = "easy";

/* --- Static array of difficulty levels --- */
static const char *difficulty_levels[] = { "easy", "medium", "hard" };


// void action_new(GSimpleAction *action, GVariant *p, gpointer user_data);
// void action_quit(GSimpleAction *action, GVariant *p, gpointer user_data);
// void action_about(GSimpleAction *action, GVariant *p, gpointer user_data);

// void action_difficulty(GSimpleAction *action, GVariant *p, gpointer user_data);

#endif