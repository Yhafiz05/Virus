#include <gtk/gtk.h>

// Fonction de callback pour le calcul
void on_calculate(GtkWidget *widget, gpointer data) {
    // Récupérer les valeurs des champs de saisie
    GtkEntry *entry1 = GTK_ENTRY(gtk_grid_get_child_at(GTK_GRID(data), 0, 0));
    const gchar *text1 = gtk_entry_get_text(entry1);

    GtkEntry *entry2 = GTK_ENTRY(gtk_grid_get_child_at(GTK_GRID(data), 0, 1));
    const gchar *text2 = gtk_entry_get_text(entry2);

    // Convertir les valeurs en nombres
    double num1 = atof(text1);
    double num2 = atof(text2);

    // Effectuer le calcul (ici, une addition)
    double result = num1 + num2;

    // Afficher le résultat dans une boîte de dialogue
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(widget))),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "Le résultat est : %.2f", result);

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Créer la fenêtre principale
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Calculatrice");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Créer deux champs de saisie
    GtkWidget *entry1 = gtk_entry_new();
    GtkWidget *entry2 = gtk_entry_new();

    // Créer un bouton de calcul
    GtkWidget *calculate_button = gtk_button_new_with_label("Calculer");

    // Créer une mise en page pour organiser les éléments
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), entry1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry2, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), calculate_button, 0, 2, 1, 1);

    // Connecter la fonction de callback au bouton de calcul
    g_signal_connect(G_OBJECT(calculate_button), "clicked", G_CALLBACK(on_calculate), grid);

    // Ajouter la mise en page à la fenêtre
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Afficher tous les éléments
    gtk_widget_show_all(window);

    // Lancer la boucle principale
    gtk_main();

    return 0;
}

