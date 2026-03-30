#include "ldbd.h"

gchar *LDBD_save_data_path = NULL;

LDBD_Entry LDBD_create_entry(const char *name, int level, int total_time, int points) {
	LDBD_Entry entry;
	
	snprintf(entry.name, LDBD_MAX_ENTRY_NAME_BUFFER_SIZE, "%s", name);
	entry.level = level;
	entry.total_time = total_time;
	entry.points = points;

	return entry;
}

:w`

void LDBD_save_entry(LDBD_Entry *entry){
	
	FILE *fptr = fopen(LDBD_save_data_path, "ab+");
	if(!fptr) {
		perror("Leaderboard: No file found!");
		return;
	}

//	LDBD_save_entry_to_file(fptr,entry);
	fwrite(entry, sizeof *entry, 1, fptr);
	fclose(fptr);
}



gchar *LDBD_create_save_data_path(gchar *filename){
	gchar *data_dir = g_build_filename(
		g_get_user_data_dir(), // platform-correct base dir
		"minesweeper",
		NULL
	);

	g_mkdir_with_parents(data_dir,0700);

	gchar *file_path = g_build_filename(
		data_dir,
		filename,
		NULL
	);
	
	return file_path;
}
