#include <gtk/gtk.h>
#include <stdlib.h>  
#include "Viewport.hpp"
#include "objects.hpp"

//Objetos da main window
Viewport* viewport;
Coordinates polygon_coords;

GObject *main_w;
GtkListStore  *store;
GtkTreeIter iter;
GtkTreeView* objects_tree;
GtkWidget *view;

GtkWidget* draw_viewport;
GtkButton* zoom_in;
GtkButton* zoom_out;
GtkButton* move_down;
GtkButton* move_up;
GtkButton* move_right;
GtkButton* move_left;
GtkButton* add_geometric_shape;

//Enum para TreeView
enum
{
  COL_NAME = 0,
  COL_TYPE,
  NUM_COLS
} ;

//Objetos da janela de adicionar forma geometrica
GObject* add_geometric_shape_w;
GtkButton* add_point1;
GtkButton* add_line1;
GtkButton* add_poly1;

//Objetos da janela de adicionar ponto, linha e poligono
GObject* add_point_w;
GtkEntry* name_point_entry;
GtkEntry* x1_point_entry;
GtkEntry* y1_point_entry;
GtkButton* add_point;

GObject* add_line_w;
GtkEntry* name_line_entry;
GtkEntry* x1_line_entry;
GtkEntry* y1_line_entry;
GtkEntry* x2_line_entry;
GtkEntry* y2_line_entry;
GtkButton* add_line;

GObject* add_poly_w;
GtkEntry* name_poly_entry;
GtkEntry* x_poly_entry;
GtkEntry* y_poly_entry;
GtkButton* add_point_poly;
GtkButton* add_poly;
GtkToggleButton *filled;

void fill_treeview(const char* name,const char* type);

/* CALLBACKS */

/* MAIN_W */
void on_zoom_in_button_clicked (GtkWidget *widget, gpointer   data)
{
  viewport->zoom(10);
}
void on_zoom_out_button_clicked (GtkWidget *widget, gpointer   data)
{
  viewport->zoom(-10);
}
void on_up_button_clicked (GtkWidget *widget, gpointer   data)
{
  viewport->moveY(10);
}
void on_down_button_clicked (GtkWidget *widget, gpointer   data)
{
  viewport->moveY(-10);
}
void on_left_button_clicked (GtkWidget *widget, gpointer   data)
{
  viewport->moveX(-10);
}
void on_right_button_clicked (GtkWidget *widget, gpointer   data)
{
  viewport->moveX(10); 
}
void on_add_object_button_clicked (GtkWidget *widget, gpointer   data)
{
  gtk_widget_show (GTK_WIDGET(add_geometric_shape_w));
}
/* ADD_GEOMETRIC_W */
void on_add_point1_clicked (GtkWidget *widget, gpointer   data)
{
  gtk_widget_show (GTK_WIDGET(add_point_w));
}
void on_add_line1_clicked (GtkWidget *widget, gpointer   data)
{
  gtk_widget_show (GTK_WIDGET(add_line_w));
}
void on_add_poly1_clicked (GtkWidget *widget, gpointer   data)
{
  gtk_widget_show (GTK_WIDGET(add_poly_w));
}
/* ADD_POINT_W */
void on_add_point_clicked (GtkWidget *widget, gpointer   data)
{
  const gchar* name = gtk_entry_get_text(name_point_entry);
  double x1 = atof(gtk_entry_get_text(x1_point_entry));
  double y1 = atof(gtk_entry_get_text(y1_point_entry));
 
  fill_treeview(name,"Point");

  Point* point = new Point(name, x1, y1);
  viewport->addObject(point);
}
/* ADD_LINE_W */
void on_add_line_clicked (GtkWidget *widget, gpointer   data)
{
  const gchar* name = gtk_entry_get_text(name_line_entry);
  double x1 = atof(gtk_entry_get_text(x1_line_entry));
  double y1 = atof(gtk_entry_get_text(y1_line_entry));
  double x2 = atof(gtk_entry_get_text(x2_line_entry));
  double y2 = atof(gtk_entry_get_text(y2_line_entry));

  fill_treeview(name,"Line");
  Line* line = new Line(name, x1, y1, x2, y2);
  viewport->addObject(line);

}
/* ADD_POLY */
void on_add_point_poly_clicked (GtkWidget *widget, gpointer   data)
{
  const gchar* name = gtk_entry_get_text(name_poly_entry);
  double x1 = atof(gtk_entry_get_text(x_poly_entry));
  double y1 = atof(gtk_entry_get_text(y_poly_entry));
  polygon_coords.push_back(Coordinate(x1,y1));
}

void on_add_poly_clicked (GtkWidget *widget, gpointer   data)
{
  const gchar* name = gtk_entry_get_text(name_poly_entry);
  fill_treeview(name,"Polygon");
  Polygon* polygon = new Polygon(name, polygon_coords);
  viewport->addObject(polygon);
  polygon_coords.clear();
}

gboolean draw_objects(GtkWidget* widget, cairo_t* cr, gpointer data) 
{

  cairo_set_source_rgb(cr, 1, 1, 1);   
  cairo_paint(cr);

  cairo_set_source_rgb (cr, 1, 0, 0);
  cairo_set_line_width(cr, 0.5);

  int width = gtk_widget_get_allocated_width (widget);
  int height = gtk_widget_get_allocated_height (widget);
   //550x535
   //25+500+25
   //18+500+17
  cairo_move_to(cr, 20, 20);
  cairo_line_to(cr, width - 20, 20);
  cairo_line_to(cr, width - 20, height - 20);
  cairo_line_to(cr, 20, height - 20);
  cairo_line_to(cr, 20, 20);
  cairo_stroke(cr);
  
  viewport->drawDisplayFile(cr);

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 1);


    /*for(list<Element*>::iterator it = displayFile.begin(); it != displayFile.end(); it++) {
        (*it)->draw(cr, adaptX, adaptY, window, type);
    }*/
  gtk_widget_queue_draw(draw_viewport);
  return FALSE;
}


void fill_treeview (const char* name, const char* type)
{
  gtk_list_store_append (store, &iter);
  gtk_list_store_set (store, &iter, COL_NAME, name, COL_TYPE, type,-1);
}

void create_treeview (void)
{
  GtkCellRenderer     *renderer;
  GtkTreeModel        *model;

  /* Column 1 */
  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (objects_tree), -1, "Name", renderer, "text", COL_NAME, NULL);
  /* Column 2 */
  renderer = gtk_cell_renderer_text_new ();
  gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (objects_tree), -1, "Type", renderer, "text", COL_TYPE, NULL);

  model = GTK_TREE_MODEL (store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_STRING));
  gtk_tree_view_set_model (GTK_TREE_VIEW (objects_tree), model);

  g_object_unref (model);
}

int main (int   argc, char *argv[])
{
  GtkBuilder *builder;
  gtk_init (&argc, &argv);

  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "interface.glade", NULL);

  /* Connect signal handlers to the constructed widgets. */
  main_w = gtk_builder_get_object (builder, "main_w");
  g_signal_connect (main_w, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  gtk_window_set_resizable (GTK_WINDOW(main_w),  false);

  /* Tamanho da viewport */
  viewport = new Viewport(550,535); 
  /* Connect Treeview*/
  objects_tree = GTK_TREE_VIEW(gtk_builder_get_object(builder, "object_tree"));
  create_treeview();

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


  /* Buttons add_geometric_shape_w, just open a new window*/
  add_geometric_shape_w = gtk_builder_get_object (builder, "add_geometric_shape_w");
  g_signal_connect (add_geometric_shape_w, "delete_event", G_CALLBACK (gtk_widget_hide_on_delete), NULL);

  add_point1 = GTK_BUTTON(gtk_builder_get_object(builder, "add_point1"));
  g_signal_connect (add_point1, "clicked", G_CALLBACK (on_add_point1_clicked), NULL);

  add_line1 = GTK_BUTTON(gtk_builder_get_object(builder, "add_line1"));
  g_signal_connect (add_line1, "clicked", G_CALLBACK (on_add_line1_clicked), NULL);

  add_poly1 = GTK_BUTTON(gtk_builder_get_object(builder, "add_poly1"));
  g_signal_connect (add_poly1, "clicked", G_CALLBACK (on_add_poly1_clicked), NULL);


  /* Delete action windows*/
  add_point_w = gtk_builder_get_object (builder, "add_point_w");
  g_signal_connect (add_point_w, "delete_event", G_CALLBACK (gtk_widget_hide_on_delete), NULL);

  add_line_w = gtk_builder_get_object (builder, "add_line_w");
  g_signal_connect (add_line_w, "delete_event", G_CALLBACK (gtk_widget_hide_on_delete), NULL);

  add_poly_w = gtk_builder_get_object (builder, "add_poly_w");
  g_signal_connect (add_poly_w, "delete_event", G_CALLBACK (gtk_widget_hide_on_delete), NULL);

  /* Buttons point, line and poly*/
  add_point = GTK_BUTTON(gtk_builder_get_object(builder, "add_point"));
  g_signal_connect (add_point, "clicked", G_CALLBACK (on_add_point_clicked), NULL);

  add_line = GTK_BUTTON(gtk_builder_get_object(builder, "add_line"));
  g_signal_connect (add_line, "clicked", G_CALLBACK (on_add_line_clicked), NULL);

  add_point_poly = GTK_BUTTON(gtk_builder_get_object(builder, "add_point_poly"));
  g_signal_connect (add_point_poly, "clicked", G_CALLBACK (on_add_point_poly_clicked), NULL);

  add_poly = GTK_BUTTON(gtk_builder_get_object(builder, "add_poly"));
  g_signal_connect (add_poly, "clicked", G_CALLBACK (on_add_poly_clicked), NULL);

/* Connecting Entry*/
  name_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "name_point_entry"));
  x1_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "x1_point_entry"));
  y1_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "y1_point_entry"));
  name_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "name_line_entry"));
  x1_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "x1_line_entry"));
  y1_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "y1_line_entry"));
  x2_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "x2_line_entry"));
  y2_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "y2_line_entry"));
  name_poly_entry = GTK_ENTRY(gtk_builder_get_object(builder, "name_poly_entry"));
  x_poly_entry = GTK_ENTRY(gtk_builder_get_object(builder, "x_poly_entry"));
  y_poly_entry = GTK_ENTRY(gtk_builder_get_object(builder, "y_poly_entry"));

  gtk_widget_show(GTK_WIDGET(main_w));

  gtk_main ();

  return 0;
}
