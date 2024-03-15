#include <stdio.h>
#include <gtk/gtk.h>

static int sec_elapsed = 0;
static gboolean is_timer_running = FALSE;

void update_label(GtkLabel *label) {
    gchar *text = g_strdup_printf("Time elapsed: %d secs", sec_elapsed);
    gtk_label_set_text(label, text);
    g_free(text);
}

gboolean timer_callback(GtkLabel *label) {
    if (is_timer_running) {
        sec_elapsed++;
        update_label(label);
    }
    return G_SOURCE_CONTINUE;
}

void start_stop_timer(GtkButton *button, GtkLabel *label) {
    if (is_timer_running) {
        is_timer_running = FALSE;
        gtk_button_set_label(button, "Start");
    } else {
        is_timer_running = TRUE;
        gtk_button_set_label(button, "Stop");
    }
}

void reset_timer(GtkButton *button, GtkLabel *label) {
    sec_elapsed = 0;
    update_label(label);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *label = gtk_label_new("Time elapsed: 0 secs");
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);

    GtkWidget *start_stop_button = gtk_button_new_with_label("Start");
    g_signal_connect(start_stop_button, "clicked", G_CALLBACK(start_stop_timer), label);
    gtk_box_pack_start(GTK_BOX(vbox), start_stop_button, FALSE, FALSE, 0);

    GtkWidget *reset_button = gtk_button_new_with_label("Reset");
    g_signal_connect(reset_button, "clicked", G_CALLBACK(reset_timer), label);
    gtk_box_pack_start(GTK_BOX(vbox), reset_button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);

    g_timeout_add_seconds(1, (GSourceFunc) timer_callback, label);

    gtk_main();
    return 0;
}