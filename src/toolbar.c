#include "toolbar.h"


/* -------- Menu creation -------- */
GMenuModel *create_menu(void) {
    GMenu *menubar =g_menu_new();

    /* File Menu*/
    GMenu *file = g_menu_new();
    g_menu_append(file, "New", "app.New");
    g_menu_append(file, "Solve", "app.Solve");
    g_menu_append(file, "Quit", "app.Quit");
    g_menu_append_submenu(menubar, "File", G_MENU_MODEL(file));

    /* Options Menu*/
    GMenu *options = g_menu_new();

    // Difficulty submenu
    GMenu *difficulty_menu = g_menu_new();
    g_menu_append(difficulty_menu, "Easy", "app.Difficulty::easy");
    g_menu_append(difficulty_menu, "Medium", "app.Difficulty::medium");
    g_menu_append(difficulty_menu, "Hard", "app.Difficulty::hard");
    g_menu_append_submenu(options, "Difficulty", G_MENU_MODEL(difficulty_menu));

    g_menu_append_submenu(menubar, "Options", G_MENU_MODEL(options));



    /* Help submenu*/
    GMenu *help = g_menu_new();
    g_menu_append(help, "About", "app.About");
    g_menu_append_submenu(menubar, "Help", G_MENU_MODEL(help));

    return G_MENU_MODEL(menubar);
}


