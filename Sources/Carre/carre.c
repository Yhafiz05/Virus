#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gtk/gtk.h>

// Fonction pour calculer le carré d'un nombre
double carre(double nombre) {
    return nombre * nombre;
}

// Fonction pour calculer la racine carrée d'un nombre
double racine_carree(double nombre) {
    return sqrt(nombre);
}

// Callback pour gérer le calcul lorsque l'utilisateur appuie sur un bouton
void on_calculer(GtkWidget *button, gpointer user_data) {
    // Récupère la chaîne de caractères de l'entrée utilisateur
    GtkWidget *entry = (GtkWidget *)user_data;
    const gchar *nombre_str = gtk_entry_get_text(GTK_ENTRY(entry));
    double nombre = atof(nombre_str);

    // Récupère le nom du bouton qui a été cliqué
    const gchar *bouton_label = gtk_button_get_label(GTK_BUTTON(button));

    // Variable pour stocker le résultat du calcul
    double resultat;

    // Effectue le calcul approprié en fonction du bouton cliqué
    if (strcmp(bouton_label, "Carré") == 0) {
        resultat = carre(nombre);
    } else if (strcmp(bouton_label, "Racine") == 0) {
        resultat = racine_carree(nombre);
    } else {
        resultat = 0.0; // En cas d'erreur, retourne 0
    }

    // Affiche le résultat dans une boîte de dialogue
    gchar *resultat_str = g_strdup_printf("Résultat : %.2f", resultat);
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", resultat_str);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    g_free(resultat_str);
}

int main(int argc, char *argv[]) {
    // Initialisation de l'environnement gtk avec les argument passé à la fonction main
    gtk_init(&argc, &argv);

     // Création de la fenêtre principale

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   
    gtk_window_set_title(GTK_WINDOW(window), "Calculateur de carré et de racine au carrée");
    // Signal de destruction de la fenêtre
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Création de la grille pour organiser les widgets
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Ajouts des différents éléments et callback sur la fonction on_calculer lors de l'appui sur les boutons
    GtkWidget *nombre_label = gtk_label_new("Nombre :");
    gtk_grid_attach(GTK_GRID(grid), nombre_label, 0, 0, 1, 1);

    GtkWidget *nombre_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), nombre_entry, 1, 0, 1, 1);

    GtkWidget *carre_button = gtk_button_new_with_label("Carré");
    g_signal_connect(carre_button, "clicked", G_CALLBACK(on_calculer), nombre_entry);
    gtk_grid_attach(GTK_GRID(grid), carre_button, 0, 1, 1, 1);

    GtkWidget *racine_button = gtk_button_new_with_label("Racine");
    g_signal_connect(racine_button, "clicked", G_CALLBACK(on_calculer), nombre_entry);
    gtk_grid_attach(GTK_GRID(grid), racine_button, 1, 1, 1, 1);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
