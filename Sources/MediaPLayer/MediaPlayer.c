#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <gtk/gtk.h>

#define MAX_FILES 100
#define MIN_FILES 6

GtkWidget *image;
char *file_paths[MAX_FILES];
int current_index = 0;
int num_files = 0;

void load_image(const char *file_path) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(file_path, NULL);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
}

void next_image(GtkWidget *widget, gpointer data) {
    if (current_index < num_files - 1) {
        current_index++;
        load_image(file_paths[current_index]);
    }
}

void prev_image(GtkWidget *widget, gpointer data) {
    if (current_index > 0) {
        current_index--;
        load_image(file_paths[current_index]);
    }
}

int is_image_file(const char *filename) {
    const char *ext = strrchr(filename, '.');
    if (ext != NULL) {
        if (strcasecmp(ext, ".jpg") == 0 || strcasecmp(ext, ".jpeg") == 0 || strcasecmp(ext, ".svg") == 0) {
            return 1;
        }
    }
    return 0;
}

void list_image_files() {
    DIR *dir;
    struct dirent *entry;
    dir = opendir("../../images");
    if (dir != NULL) {
        while ((entry = readdir(dir)) != NULL && num_files < MAX_FILES) {
            if (is_image_file(entry->d_name)) {
                file_paths[num_files] = strdup(entry->d_name);
                num_files++;
            }
        }
        closedir(dir);
    }
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *hbox;
    GtkWidget *prev_button;
    GtkWidget *next_button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Image Viewer");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 800); // Taille fixe de 800x800
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_add(GTK_CONTAINER(window), hbox);

    prev_button = gtk_button_new_with_label("Previous");
    g_signal_connect(prev_button, "clicked", G_CALLBACK(prev_image), NULL);
    gtk_box_pack_start(GTK_BOX(hbox), prev_button, TRUE, TRUE, 5);

    image = gtk_image_new();
    gtk_box_pack_start(GTK_BOX(hbox), image, TRUE, TRUE, 5);

    next_button = gtk_button_new_with_label("Next");
    g_signal_connect(next_button, "clicked", G_CALLBACK(next_image), NULL);
    gtk_box_pack_start(GTK_BOX(hbox), next_button, TRUE, TRUE, 5);

    list_image_files();
    load_image(file_paths[current_index]);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
    