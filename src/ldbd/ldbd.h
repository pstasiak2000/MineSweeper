#ifndef LDBD_H
#define LDBD_H

#include <stdio.h>
#include <gtk/gtk.h>

#define LDBD_MAX_SIZE 1000
#define LDBD_MAX_ENTRY_NAME_BUFFER_SIZE 64

extern gchar *LDBD_save_data_path;

/* Leaderboard entry structure for time elapsed and points*/
typedef struct {
	const char name[LDBD_MAX_ENTRY_NAME_BUFFER_SIZE];
	int level;
	int total_time;
	int points;
} LDBD_Entry ;

/* Leaderboard structure with entries as an array  */
typedef struct {
	int size;
	LDBD_Entry *data;
} LDBD_LeaderBoard;

// Create a leaderboard 
LDBD_Entry LDBD_create_entry(const char *name, int total_time, int points);

// Save leaderboard entry to file
void LDBD_save_entry(LDBD_Entry *entry);

// Create_save data path for leaderboard entries
gchar *LDBD_create_save_data_path(gchar *filename);

#endif
