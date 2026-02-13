#include "gui.h"

GtkCssProvider *provider;

gboolean developer_tools = FALSE;

/*****************************************/
/*   UI Generation                      */
/*****************************************/

GtkWidget *create_header_box(void)
{
    GtkWidget *header_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

    gtk_widget_set_hexpand(header_box, TRUE); // horizontal fill
    gtk_widget_set_size_request(header_box, -1, 40);
    gtk_box_set_homogeneous(GTK_BOX(header_box), TRUE); // children equal sizes

    return header_box;
}

GtkWidget *create_empty_gui_grid(void)
{
    GtkWidget *grid = gtk_grid_new();

    gtk_grid_set_row_spacing(GTK_GRID(grid), 1);    // vertical spacing
    gtk_grid_set_column_spacing(GTK_GRID(grid), 1); // horizontal spacing
    gtk_widget_set_hexpand(grid, TRUE);             // fill horizontally
    gtk_widget_set_vexpand(grid, TRUE);             // fill vertically

    gtk_widget_set_halign(grid, GTK_ALIGN_FILL);
    gtk_widget_set_valign(grid, GTK_ALIGN_FILL);

    return grid;
}

Panel create_header_panel(const char *text, const char *css_class_name)
{

    /* Panel box */
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_valign(box, GTK_ALIGN_FILL);

    /* Panel label */
    GtkWidget *label = gtk_label_new(text);
    gtk_box_append(GTK_BOX(box), label);
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(label, GTK_ALIGN_CENTER);

    if (css_class_name)
        gtk_widget_add_css_class(label, css_class_name);

    Panel panel = {.box = box, .label = label};

    return panel;
}

void update_label_text(GtkWidget *label, const char *new_text)
{
    gtk_label_set_text(GTK_LABEL(label), new_text);
}

// Global or static function
static void reset_response_cb(GtkDialog *dlg, gint response, gpointer user_data)
{
    gboolean *result = (gboolean *)user_data;

    if (response == GTK_RESPONSE_ACCEPT)
    {
        *result = TRUE;
    }
    else
    {
        *result = FALSE;
    }

    gtk_window_destroy(GTK_WINDOW(dlg));

    // Quit temporary loop if you used one
    GMainLoop *loop = g_object_get_data(G_OBJECT(dlg), "loop");
    if (loop)
        g_main_loop_quit(loop);
}

gboolean show_reset_confirm_dialog(GtkWindow *parent, const char *text)
{
    gboolean result = FALSE;
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);

    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Confirm Reset",
        parent,
        GTK_DIALOG_MODAL,
        "_No", GTK_RESPONSE_REJECT,
        "_Yes", GTK_RESPONSE_ACCEPT,
        NULL);

    GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    // GtkWidget *label = gtk_label_new("Are you sure you want to reset the board?");
    GtkWidget *label = gtk_label_new(text);
    gtk_box_append(GTK_BOX(content), label);

    // Store loop in dialog so callback can quit it
    g_object_set_data(G_OBJECT(dialog), "loop", loop);

    // Connect callback (normal C function)
    g_signal_connect(dialog, "response", G_CALLBACK(reset_response_cb), &result);

    gtk_window_present(GTK_WINDOW(dialog));

    g_main_loop_run(loop); // blocks until callback calls g_main_loop_quit
    g_main_loop_unref(loop);

    return result;
}

void quick_message(GtkWindow *parent, char *title, char *message)
{

    GtkWidget *dialog, *label, *content_area;
    GtkDialogFlags flags;

    // Create the widgets
    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons(title,
                                         parent,
                                         flags,
                                         ("_OK"),
                                         GTK_RESPONSE_NONE,
                                         NULL);

    gtk_widget_set_size_request(dialog, 500, 100);

    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    label = gtk_label_new(message);

    // Assuming provider is already loaded globally
    GtkStyleContext *ctx = gtk_widget_get_style_context(label);
    gtk_style_context_add_provider(ctx,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER);


    gtk_label_set_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);

    // Make the label expand vertically
    gtk_widget_set_vexpand(label, TRUE);

    // Keep the text centered inside the expanded space
    gtk_widget_set_valign(label, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);

    // Ensure the box is destroyed when the user responds
    g_signal_connect_swapped(dialog,
                             "response",
                             G_CALLBACK(gtk_window_destroy),
                             dialog);

    // Add the label, and show everything we've added
    gtk_widget_add_css_class(label, "popup_message");
    gtk_box_append(GTK_BOX(content_area), label);
    gtk_widget_show(dialog);
}
