#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

static int nombreADeviner;
static int essais = 0;

void guess_number(GtkWidget *widget, gpointer entry) {
    const gchar *str = gtk_entry_get_text(GTK_ENTRY(entry));
    int nombreUtilisateur = atoi(str);
    GtkWidget *dialog;
    gchar *message;

    if (nombreUtilisateur < nombreADeviner) {
        message = g_strdup_printf("Le nombre à deviner est plus grand.");
    } else if (nombreUtilisateur > nombreADeviner) {
        message = g_strdup_printf("Le nombre à deviner est plus petit.");
    } else {
        message = g_strdup_printf("Félicitations ! Vous avez deviné le nombre en %d essais.", essais);
        gtk_widget_set_sensitive(widget, FALSE);
    }

    dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    g_free(message);

    essais++;
}

void new_game(GtkWidget *widget, gpointer entry) {
    GtkWidget *dialog;
    gchar *message;

    nombreADeviner = rand() % 100 + 1;
    essais = 0;

    message = g_strdup_printf("Un nouveau jeu a commencé. Devinez un nombre entre 1 et 100.");
    dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    g_free(message);

    gtk_entry_set_text(GTK_ENTRY(entry), "");
    gtk_widget_grab_focus(entry);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Jeu de devinette");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 300, 100);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), entry, TRUE, TRUE, 0);

    GtkWidget *guess_button = gtk_button_new_with_label("Devinez");
    g_signal_connect(guess_button, "clicked", G_CALLBACK(guess_number), entry);
    gtk_box_pack_start(GTK_BOX(vbox), guess_button, TRUE, TRUE, 0);

    GtkWidget *new_game_button = gtk_button_new_with_label("Nouveau jeu");
    g_signal_connect(new_game_button, "clicked", G_CALLBACK(new_game), entry);
    gtk_box_pack_start(GTK_BOX(vbox), new_game_button, TRUE, TRUE, 0);

    gtk_widget_show_all(window);

    srand(time(NULL));
    new_game(window, entry);

    gtk_main();

    return 0;
}

