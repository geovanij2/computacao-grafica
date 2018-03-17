#include <iostream>
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
GtkTreeSelection* objects_select;

GtkWidget* draw_viewport;
GtkButton* zoom_in;
GtkButton* zoom_out;
GtkButton* move_down;
GtkButton* move_up;
GtkButton* move_right;
GtkButton* move_left;
GtkButton* add_geometric_shape;
GtkButton* change_object;
GtkEntry* step_entry;

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

//Objects from change_obj_w
GObject* change_obj_w;
GtkEntry* angle_world_entry;
GtkEntry* angle_obj_entry;
GtkEntry* trans_y_entry;
GtkEntry* trans_x_entry;
GtkEntry* angle_point_entry;
GtkEntry* angle_pointx_entry;
GtkEntry* angle_pointy_entry;
GtkEntry* sx_entry;
GtkEntry* sy_entry;
GtkButton* angle_world_button;
GtkButton* angle_obj_button;
GtkButton* translate_button;
GtkButton* rotate_point_button;
GtkButton* schedule_button;
GtkButton* change_obj_button;

void fill_treeview(const char* name,const char* type);
std::string get_name_selected();

/* CALLBACKS */

/* MAIN_W */
void on_zoom_in_button_clicked (GtkWidget *widget, gpointer   data)
{
  double step = atof(gtk_entry_get_text(step_entry));
  viewport->zoom(step);
}
void on_zoom_out_button_clicked (GtkWidget *widget, gpointer   data)
{
  double step = atof(gtk_entry_get_text(step_entry));
  viewport->zoom(-step);
}
void on_up_button_clicked (GtkWidget *widget, gpointer   data)
{
  double step = atof(gtk_entry_get_text(step_entry));
  viewport->moveY(step);
}
void on_down_button_clicked (GtkWidget *widget, gpointer   data)
{
  double step = atof(gtk_entry_get_text(step_entry));
  viewport->moveY(-step);
}
void on_left_button_clicked (GtkWidget *widget, gpointer   data)
{
  double step = atof(gtk_entry_get_text(step_entry));
  viewport->moveX(-step);
}
void on_right_button_clicked (GtkWidget *widget, gpointer   data)
{
  double step = atof(gtk_entry_get_text(step_entry));
  viewport->moveX(step); 
}
void on_add_object_button_clicked (GtkWidget *widget, gpointer   data)
{
  gtk_widget_show (GTK_WIDGET(add_geometric_shape_w));
}
void on_change_object_clicked (GtkWidget *widget, gpointer   data)
{
  //esse metodo deve instanciar uma matriz em que será acumulada as transformações
  //cada botão adicionará a sua matriz na matriz atual acumulando as transformações
  GtkTreeIter iter;
  GtkTreeModel *model;
  if(gtk_tree_selection_get_selected (objects_select, &model, &iter)) {
    gtk_widget_show (GTK_WIDGET(change_obj_w));
  }
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

/* Buttons Change object */
void on_angle_world_button_clicked (GtkWidget *widget, gpointer   data)
{
  double angle = atof(gtk_entry_get_text(angle_world_entry));
  //acumula a matriz de rotação no centro do mundo na matriz atual
}
void on_angle_obj_button_clicked(GtkWidget *widget, gpointer   data)
{
  std::string name = get_name_selected();
  double angle = atof(gtk_entry_get_text(angle_obj_entry));
  //acumula a matriz de rotação no objeto na matriz atual
}
void on_translate_button_clicked (GtkWidget *widget, gpointer   data)
{
  double dx = atof(gtk_entry_get_text(trans_x_entry));
  double dy = atof(gtk_entry_get_text(trans_y_entry));
  //acumula a matriz de translação na matriz atual
}
void on_rotate_point_button_clicked (GtkWidget *widget, gpointer   data)
{
  double angle = atof(gtk_entry_get_text(angle_point_entry));
  double x = atof(gtk_entry_get_text(angle_pointx_entry));
  double y = atof(gtk_entry_get_text(angle_pointy_entry));
  //acumula a matriz de rotação na matriz atual
}
void on_schedule_button_clicked (GtkWidget *widget, gpointer   data)
{
  double sx = atof(gtk_entry_get_text(sx_entry));
  double sy = atof(gtk_entry_get_text(sy_entry));
  //acumula a matriz de escalonamento na matriz atual
}
void on_change_obj_button_clicked (GtkWidget *widget, gpointer   data)
{
  std::string name = get_name_selected();
  //aqui será onde será feita a multiplicação da matriz final no objeto com nome name
}

gboolean draw_objects(GtkWidget* widget, cairo_t* cr, gpointer data) 
{
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);
  cairo_set_source_rgb (cr, 1, 0, 0);
  cairo_set_line_width(cr, 5.0);

  int width = gtk_widget_get_allocated_width (widget);
  int height = gtk_widget_get_allocated_height (widget);
   //530x535
  cairo_move_to(cr, 0, 0);
  cairo_line_to(cr, width, 0);
  cairo_line_to(cr, width, height);
  cairo_line_to(cr, 0, height);
  cairo_line_to(cr, 0, 0);
  cairo_stroke(cr);
  
  cairo_set_source_rgb(cr, 0, 0, 0);   
  cairo_set_line_width(cr, 1.0);
  viewport->drawDisplayFile(cr);

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_set_line_width(cr, 1);

  gtk_widget_queue_draw(draw_viewport);
  return FALSE;
}

std::string get_name_selected()
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar* object_name;
  std::string name;

  if(gtk_tree_selection_get_selected (objects_select, &model, &iter)) {
    gtk_tree_model_get (model, &iter, COL_NAME, &object_name, -1);
    name = (std::string)object_name;
  }
  return name;
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

  objects_select = gtk_tree_view_get_selection(objects_tree);
  gtk_tree_selection_set_mode(objects_select, GTK_SELECTION_SINGLE);

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
  viewport = new Viewport(530,535); 
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

  change_object = GTK_BUTTON(gtk_builder_get_object(builder, "change_object"));
  g_signal_connect (change_object, "clicked", G_CALLBACK (on_change_object_clicked), NULL);


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

/* Buttons Change object */
  change_obj_w = gtk_builder_get_object (builder, "change_obj_w");
  g_signal_connect (change_obj_w, "delete_event", G_CALLBACK (gtk_widget_hide_on_delete), NULL);

  angle_world_button = GTK_BUTTON(gtk_builder_get_object(builder, "angle_world_button"));
  g_signal_connect (angle_world_button  , "clicked", G_CALLBACK (on_angle_world_button_clicked), NULL);

  angle_obj_button = GTK_BUTTON(gtk_builder_get_object(builder, "angle_obj_button"));
  g_signal_connect (angle_obj_button, "clicked", G_CALLBACK (on_angle_obj_button_clicked), NULL);

  translate_button = GTK_BUTTON(gtk_builder_get_object(builder, "translate_button"));
  g_signal_connect (translate_button, "clicked", G_CALLBACK (on_translate_button_clicked), NULL);

  rotate_point_button = GTK_BUTTON(gtk_builder_get_object(builder, "rotate_point_button"));
  g_signal_connect (rotate_point_button, "clicked", G_CALLBACK (on_rotate_point_button_clicked), NULL);

  schedule_button = GTK_BUTTON(gtk_builder_get_object(builder, "schedule_button"));
  g_signal_connect (schedule_button, "clicked", G_CALLBACK (on_schedule_button_clicked), NULL);

  change_obj_button = GTK_BUTTON(gtk_builder_get_object(builder, "change_obj_button"));
  g_signal_connect (change_obj_button, "clicked", G_CALLBACK (on_change_obj_button_clicked), NULL);

/* Connecting Entry*/
  step_entry = GTK_ENTRY(gtk_builder_get_object(builder, "step_entry"));
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
  angle_world_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_world_entry"));
  angle_obj_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_obj_entry"));
  trans_x_entry = GTK_ENTRY(gtk_builder_get_object(builder, "trans_x_entry"));
  trans_y_entry = GTK_ENTRY(gtk_builder_get_object(builder, "trans_y_entry"));
  angle_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_point_entry"));
  angle_pointx_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_pointx_entry"));
  angle_pointy_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_pointy_entry"));
  sx_entry =  GTK_ENTRY(gtk_builder_get_object(builder, "sx_entry"));
  sy_entry =  GTK_ENTRY(gtk_builder_get_object(builder, "sy_entry"));


  gtk_widget_show(GTK_WIDGET(main_w));

  gtk_main ();

  return 0;
}
