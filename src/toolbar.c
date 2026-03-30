#include "toolbar.h"
#include "devtools.h"

static void 
hide_menuitem_visibility(GtkApplication *app, char *action_name)
{   
    // Look-up the action
    GAction *action = 
        g_action_map_lookup_action(
            G_ACTION_MAP(app), 
            action_name
        );
    
    // Hide the action 
    if(action)
        g_simple_action_set_enabled(
            G_SIMPLE_ACTION(action),
            FALSE
        );
}


/* -------- Menu creation -------- */
GMenuModel *create_menu(AppCtx *ctx) {
    GMenu *menubar =g_menu_new();

    /* File Menu*/
    GMenu *file = g_menu_new();
    g_menu_append(file, "New", "app.New");

    // g_menu_append(file, "Solve", "app.Solve");
    GMenuItem *solve_item = 
        g_menu_item_new("Solve", "app.Solve");

    g_menu_item_set_attribute(solve_item,
                            "hidden-when",
                            "s",
                            "action-disabled"); //Start hidden
    
    hide_menuitem_visibility(ctx->app,"Solve");

    g_menu_append_item(file, solve_item);

    g_menu_append(file, "Quit", "app.Quit");
    g_menu_append_submenu(menubar, "File", G_MENU_MODEL(file));

    /* Options Menu*/
    GMenu *options = g_menu_new();
    
    #if COMPILE_WITH_DEV_TOOLS
        g_menu_append(options,
                        "Developer Tools",
                        "app.developer-tools");
    #endif
    
    // Difficulty submenu
	GMenuItem *item;

    GMenu *difficulty_menu = g_menu_new();
	item = g_menu_item_new("Easy", "app.difficulty");
	g_menu_item_set_attribute_value(item, "target",g_variant_new_int32(EASY));
	g_menu_append_item(difficulty_menu, item);
 
		
	item = g_menu_item_new("Medium", "app.difficulty");
	g_menu_item_set_attribute_value(item, "target",g_variant_new_int32(MEDIUM));
	g_menu_append_item(difficulty_menu, item);

	item = g_menu_item_new("Hard", "app.difficulty");
	g_menu_item_set_attribute_value(item, "target",g_variant_new_int32(HARD));
	g_menu_append_item(difficulty_menu, item);
//   g_menu_append(difficulty_menu, "Easy", "app.Difficulty::0");
 //   g_menu_append(difficulty_menu, "Medium", "app.Difficulty::1");
 //   g_menu_append(difficulty_menu, "Hard", "app.Difficulty::2");
   g_menu_append_submenu(options, "Difficulty", G_MENU_MODEL(difficulty_menu));

    g_menu_append_submenu(menubar, "Options", G_MENU_MODEL(options));



    /* Help submenu*/
    GMenu *help = g_menu_new();
    g_menu_append(help, "About", "app.About");
    g_menu_append_submenu(menubar, "Help", G_MENU_MODEL(help));

    return G_MENU_MODEL(menubar);
}


