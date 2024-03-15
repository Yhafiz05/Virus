#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

// Fonction pour convertir Celsius en Fahrenheit
double celsius_to_fahrenheit(double celsius) {
    return (celsius * 9 / 5) + 32;
}

// Fonction pour convertir Fahrenheit en Celsius
double fahrenheit_to_celsius(double fahrenheit) {
    return (fahrenheit - 32) * 5 / 9;
}

// Callback pour gérer la conversion de Celsius en Fahrenheit
void on_celsius_to_fahrenheit(GtkWidget *button, gpointer user_data) {
    GtkWidget *entry = (GtkWidget *)user_data;
    const gchar *temp_str = gtk_entry_get_text(GTK_ENTRY(entry));
    double celsius = atof(temp_str);
    double fahrenheit = celsius_to_fahrenheit(celsius);

    gchar *result_str = g_strdup_printf("%.2f Celsius = %.2f Fahrenheit", celsius, fahrenheit);

    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", result_str);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    g_free(result_str);
}

// Callback pour gérer la conversion de Fahrenheit en Celsius
void on_fahrenheit_to_celsius(GtkWidget *button, gpointer user_data) {
    GtkWidget *entry = (GtkWidget *)user_data;
    const gchar *temp_str = gtk_entry_get_text(GTK_ENTRY(entry));
    double fahrenheit = atof(temp_str);
    double celsius = fahrenheit_to_celsius(fahrenheit);

    gchar *result_str = g_strdup_printf("%.2f Fahrenheit = %.2f Celsius", fahrenheit, celsius);

    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", result_str);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    g_free(result_str);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Convertisseur de température");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget *temp_label = gtk_label_new("Température :");
    gtk_grid_attach(GTK_GRID(grid), temp_label, 0, 0, 1, 1);

    GtkWidget *temp_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), temp_entry, 1, 0, 1, 1);

    GtkWidget *celsius_button = gtk_button_new_with_label("Celsius to Fahrenheit");
    g_signal_connect(celsius_button, "clicked", G_CALLBACK(on_celsius_to_fahrenheit), temp_entry);
    gtk_grid_attach(GTK_GRID(grid), celsius_button, 0, 1, 1, 1);

    GtkWidget *fahrenheit_button = gtk_button_new_with_label("Fahrenheit to Celsius");
    g_signal_connect(fahrenheit_button, "clicked", G_CALLBACK(on_fahrenheit_to_celsius), temp_entry);
    gtk_grid_attach(GTK_GRID(grid), fahrenheit_button, 1, 1, 1, 1);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

