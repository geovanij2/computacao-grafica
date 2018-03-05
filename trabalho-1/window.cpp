#include <gtk/gtk.h>

GObject *main_window;
GtkWidget* draw_viewport;
GtkButton* zoom_out;
GtkButton* zoom_in;
GtkButton* move_up;
GtkButton* move_down;
GtkButton* move_left;
GtkButton* move_right;
GtkButton* add_geometric_shape;


void on_zoom_in_button_clicked (GtkWidget *widget, gpointer   data)
{
  printf ("Zoom in\n");
}
void on_zoom_out_button_clicked (GtkWidget *widget, gpointer   data)
{
  printf ("Zoom out\n");
}
void on_up_button_clicked (GtkWidget *widget, gpointer   data)
{
  printf ("Up\n");
}
void on_down_button_clicked (GtkWidget *widget, gpointer   data)
{
  printf ("Down\n");
}
void on_left_button_clicked (GtkWidget *widget, gpointer   data)
{
  printf ("Left\n");
}
void on_right_button_clicked (GtkWidget *widget, gpointer   data)
{
  printf ("Right\n");
}
void on_add_object_button_clicked (GtkWidget *widget, gpointer   data)
{
  printf ("New Object\n");
}

gboolean draw_objects(GtkWidget* widget, cairo_t* cr, gpointer data) {

    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    cairo_set_source_rgb (cr, 1, 0, 0);
    cairo_set_line_width(cr, 0.5);

    double width = gtk_widget_get_allocated_width (widget);
    double height = gtk_widget_get_allocated_height (widget);
    
    cairo_move_to(cr, 20, 20);
    cairo_line_to(cr, width-20, 20);
    cairo_line_to(cr, width-20, height-20);
    cairo_line_to(cr, 20,height-20);
    cairo_line_to(cr, 20, 20);
    cairo_stroke(cr);
    
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 1);


    /*for(list<Element*>::iterator it = displayFile.begin(); it != displayFile.end(); it++) {
        (*it)->draw(cr, adaptX, adaptY, window, type);
    }*/
    gtk_widget_queue_draw(draw_viewport);
    return FALSE;
}

int
main (int   argc,
      char *argv[])
{
  GtkBuilder *builder;
  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "interface.glade", NULL);

  /* Connect signal handlers to the constructed widgets. */
  main_window = gtk_builder_get_object (builder, "main_window");
  g_signal_connect (main_window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  draw_viewport = GTK_WIDGET(gtk_builder_get_object(builder, "draw_viewport"));
  g_signal_connect(draw_viewport, "draw", G_CALLBACK(draw_objects), NULL);
  /* Buttons */
  zoom_out = GTK_BUTTON(gtk_builder_get_object(builder, "zoom_out"));
  g_signal_connect (zoom_out, "clicked", G_CALLBACK (on_zoom_out_button_clicked), NULL);

  zoom_in = GTK_BUTTON(gtk_builder_get_object(builder, "zoom_in"));
  g_signal_connect (zoom_in, "clicked", G_CALLBACK (on_zoom_in_button_clicked), NULL);

  move_up = GTK_BUTTON(gtk_builder_get_object(builder, "move_up"));
  g_signal_connect (move_up, "clicked", G_CALLBACK (on_up_button_clicked), NULL);

  move_down = GTK_BUTTON(gtk_builder_get_object(builder, "move_down"));
  g_signal_connect (move_down, "clicked", G_CALLBACK (on_down_button_clicked), NULL);

  move_left = GTK_BUTTON(gtk_builder_get_object(builder, "move_left"));
  g_signal_connect (move_left, "clicked", G_CALLBACK (on_left_button_clicked), NULL);

  move_right = GTK_BUTTON(gtk_builder_get_object(builder, "move_right"));
  g_signal_connect (move_right, "clicked", G_CALLBACK (on_right_button_clicked), NULL);

  add_geometric_shape = GTK_BUTTON(gtk_builder_get_object(builder, "add_geometric_shape"));
  g_signal_connect (add_geometric_shape, "clicked", G_CALLBACK (on_add_object_button_clicked), NULL);

  gtk_widget_show(GTK_WIDGET(main_window));

  gtk_main ();

  return 0;
}
