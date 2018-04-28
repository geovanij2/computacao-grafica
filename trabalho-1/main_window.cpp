#include <iostream>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>  
#include "Viewport.hpp"
#include "objects.hpp"
#include "Transformation.hpp"

//Objetos da main window
Viewport* viewport;
Coordinates polygon_coords;
Coordinates curve_coords;
bool isObject3D = false;
std::vector<Transformation> accumulator;
std::vector<Polygon> faces_object3D;

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
GtkButton* rotate_left;
GtkButton* rotate_right;
GtkEntry* step_entry;
GtkEntry* angle_entry;
GtkToggleButton *LB_Clipping,*CS_Clipping;
GtkToggleButton *x_check,*y_check,*z_check;

//Enum para TreeView
enum {	 	  	 	     	  		  	  	    	      	 	
  COL_ID = 0,
  COL_NAME,
  COL_TYPE,
  NUM_COLS
};

int count_obj = 0;

//Objetos da janela de adicionar forma geometrica
GObject* add_geometric_shape_w;
GtkButton* add_point1;
GtkButton* add_line1;
GtkButton* add_poly1;
GtkButton* add_curve1;
GtkButton* add_object3D1;

//Objetos da janela de adicionar ponto, linha e poligono
GObject* add_point_w;
GtkEntry* name_point_entry;
GtkEntry* x1_point_entry;
GtkEntry* y1_point_entry;
GtkEntry* z1_point_entry;
GtkButton* add_point;

GObject* add_line_w;
GtkEntry* name_line_entry;
GtkEntry* x1_line_entry;
GtkEntry* y1_line_entry;
GtkEntry* z1_line_entry;
GtkEntry* x2_line_entry;
GtkEntry* y2_line_entry;
GtkEntry* z2_line_entry;
GtkButton* add_line;

GObject* add_poly_w;
GtkEntry* name_poly_entry;
GtkEntry* x_poly_entry;
GtkEntry* y_poly_entry;
GtkEntry* z_poly_entry;
GtkButton* add_point_poly;
GtkButton* add_poly;
GtkToggleButton* filled;

GObject* add_curve_w;
GtkEntry* name_curve_entry;
GtkEntry* x_curve_entry;
GtkEntry* y_curve_entry;
GtkEntry* z_curve_entry;
GtkButton* add_point_curve;
GtkButton* add_curve;
GtkLabel* label_number_points;
GtkToggleButton* bspline_check;

GObject* add_object3D_w;
GtkEntry* name_object3D_entry;
GtkButton* add_face;
GtkButton* add_object3D;

//Objects from change_obj_w
GObject* change_obj_w;
GtkEntry* angle_world_entry;
GtkEntry* angle_obj_entry;
GtkEntry* trans_y_entry;
GtkEntry* trans_x_entry;
GtkEntry* trans_z_entry;
GtkEntry* angle_point_entry;
GtkEntry* angle_pointx_entry;
GtkEntry* angle_pointy_entry;
GtkEntry* angle_pointz_entry;
GtkEntry* angle_vectorx_entry;
GtkEntry* angle_vectory_entry;
GtkEntry* angle_vectorz_entry;
GtkEntry* sx_entry;
GtkEntry* sy_entry;
GtkEntry* sz_entry;
GtkButton* angle_world_button;
GtkButton* angle_obj_button;
GtkButton* translate_button;
GtkButton* rotate_point_button;
GtkButton* schedule_button;
GtkButton* change_obj_button;
GtkToggleButton *x_checkr,*y_checkr,*z_checkr;

void fill_treeview(const char* name,const char* type);
int get_index_selected();

/* CALLBACKS */

/* MAIN_W */
void on_zoom_in_button_clicked (GtkWidget *widget, gpointer data) {	 	  	 	     	  		  	  	    	      	 	
	double step = atof(gtk_entry_get_text(step_entry));
	viewport->zoom(step);
}

void on_zoom_out_button_clicked (GtkWidget *widget, gpointer data) {
	double step = atof(gtk_entry_get_text(step_entry));
	viewport->zoom(-step);
}

void on_up_button_clicked (GtkWidget *widget, gpointer data) {
	double step = atof(gtk_entry_get_text(step_entry));
	viewport->moveY(step);
}

void on_down_button_clicked (GtkWidget *widget, gpointer data) {
	double step = atof(gtk_entry_get_text(step_entry));
	viewport->moveY(-step);
}

void on_left_button_clicked (GtkWidget *widget, gpointer data) {
	double step = atof(gtk_entry_get_text(step_entry));
	viewport->moveX(-step);
}

void on_right_button_clicked (GtkWidget *widget, gpointer data) {
	double step = atof(gtk_entry_get_text(step_entry));
	viewport->moveX(step);
}

void on_rotate_right_clicked (GtkWidget *widget, gpointer data) {
	double angle = atof(gtk_entry_get_text(angle_entry));
	if (gtk_toggle_button_get_active(x_check)) {
	    viewport->rotate_window_on_x(-angle);
	} else if (gtk_toggle_button_get_active(y_check)) {
	    viewport->rotate_window_on_y(-angle);
	} else if (gtk_toggle_button_get_active(z_check)) {
	    viewport->rotate_window_on_z(-angle);
	}
}

void on_rotate_left_clicked (GtkWidget *widget, gpointer data) {
	double angle = atof(gtk_entry_get_text(angle_entry));
	if (gtk_toggle_button_get_active(x_check)) {
	    viewport->rotate_window_on_x(angle);
	} else if (gtk_toggle_button_get_active(y_check)) {
	    viewport->rotate_window_on_y(angle);
	} else if (gtk_toggle_button_get_active(z_check)){
	    viewport->rotate_window_on_z(angle);
	}
}	 	  	 	     	  		  	  	    	      	 	

void on_add_object_button_clicked (GtkWidget *widget, gpointer data) {
	gtk_widget_show (GTK_WIDGET(add_geometric_shape_w));
}

void on_change_object_clicked (GtkWidget *widget, gpointer data) {
	//esse metodo deve instanciar uma matriz em que será acumulada as transformações
	//cada botão adicionará a sua matriz na matriz atual acumulando as transformações
	GtkTreeIter iter;
	GtkTreeModel *model;
	if(gtk_tree_selection_get_selected (objects_select, &model, &iter)) {
		gtk_widget_show (GTK_WIDGET(change_obj_w));
	}
}

/* ADD_GEOMETRIC_W */
void on_add_point1_clicked (GtkWidget *widget, gpointer data) {
	gtk_widget_show (GTK_WIDGET(add_point_w));
}

void on_add_line1_clicked (GtkWidget *widget, gpointer data) {
	gtk_widget_show (GTK_WIDGET(add_line_w));
}

void on_add_poly1_clicked (GtkWidget *widget, gpointer data) {
	isObject3D = false;
	gtk_widget_show (GTK_WIDGET(add_poly_w));
}

void on_add_curve1_clicked (GtkWidget *widget, gpointer data) {
    gtk_widget_show (GTK_WIDGET(add_curve_w));
}

void on_add_object3D1_clicked (GtkWidget *widget, gpointer data) {
    gtk_widget_show (GTK_WIDGET(add_object3D_w));
}

/* ADD_POINT_W */
void on_add_point_clicked (GtkWidget *widget, gpointer data) {
	const gchar* name = gtk_entry_get_text(name_point_entry);
	double x1 = atof(gtk_entry_get_text(x1_point_entry));
	double y1 = atof(gtk_entry_get_text(y1_point_entry));
	double z1 = atof(gtk_entry_get_text(z1_point_entry));

	fill_treeview(name,"Point");
	Point* point = new Point(name, x1, y1, z1);
	viewport->addObject(point);

    gtk_entry_set_text(name_point_entry,"");
    gtk_entry_set_text(x1_point_entry,"");
    gtk_entry_set_text(y1_point_entry,"");
    gtk_entry_set_text(z1_point_entry,"");
}	 	  	 	     	  		  	  	    	      	 	

/* ADD_LINE_W */
void on_add_line_clicked (GtkWidget *widget, gpointer data) {
	const gchar* name = gtk_entry_get_text(name_line_entry);
	double x1 = atof(gtk_entry_get_text(x1_line_entry));
	double y1 = atof(gtk_entry_get_text(y1_line_entry));
	double z1 = atof(gtk_entry_get_text(z1_line_entry));
	double x2 = atof(gtk_entry_get_text(x2_line_entry));
	double y2 = atof(gtk_entry_get_text(y2_line_entry));
	double z2 = atof(gtk_entry_get_text(z2_line_entry));
	
	fill_treeview(name,"Line");
	Line* line = new Line(name, x1, y1, z1, x2, y2, z2);
	viewport->addObject(line);
    gtk_entry_set_text(name_line_entry,"");
    gtk_entry_set_text(x1_line_entry,"");
    gtk_entry_set_text(x2_line_entry,"");
    gtk_entry_set_text(y1_line_entry,"");
    gtk_entry_set_text(y2_line_entry,"");  
    gtk_entry_set_text(z1_line_entry,"");
    gtk_entry_set_text(z2_line_entry,"");  

}

/* ADD_POLY */
void on_add_point_poly_clicked (GtkWidget *widget, gpointer data) {
	double x1 = atof(gtk_entry_get_text(x_poly_entry));
	double y1 = atof(gtk_entry_get_text(y_poly_entry));
	double z1 = atof(gtk_entry_get_text(z_poly_entry));
	polygon_coords.push_back(Coordinate(x1,y1,z1));
    gtk_entry_set_text(x_poly_entry,"");
    gtk_entry_set_text(y_poly_entry,"");
    gtk_entry_set_text(z_poly_entry,"");
}

void on_add_poly_clicked (GtkWidget *widget, gpointer data) {
  const gchar* name = gtk_entry_get_text(name_poly_entry);
	Polygon* polygon = new Polygon(name, polygon_coords, gtk_toggle_button_get_active(filled));
	if (!isObject3D) {
	    fill_treeview(name,"Polygon");
	    viewport->addObject(polygon);  
	} else {
	    faces_object3D.push_back(*polygon);
	    gtk_widget_hide (GTK_WIDGET(add_poly_w));
	}
	polygon_coords.clear();
    gtk_entry_set_text(name_poly_entry, "");
}	 	  	 	     	  		  	  	    	      	 	
/* ADD CURVE */
void on_add_point_curve_clicked (GtkWidget *widget, gpointer data) {
    double x1 = atof(gtk_entry_get_text(x_curve_entry));
    double y1 = atof(gtk_entry_get_text(y_curve_entry));
    double z1 = atof(gtk_entry_get_text(z_curve_entry));
    curve_coords.push_back(Coordinate(x1,y1,z1));
    int n = atoi(gtk_label_get_text(label_number_points));
    //char* s;
    //itoa(++n, s, 10);
    char buf[10];
    sprintf(buf, "  %d  ", ++n);

    gtk_label_set_text(label_number_points, buf);
    gtk_entry_set_text(x_curve_entry, "");
    gtk_entry_set_text(y_curve_entry, "");
    gtk_entry_set_text(z_curve_entry, "");
}

void on_add_curve_clicked (GtkWidget *widget, gpointer data) {
  const gchar* name = gtk_entry_get_text(name_curve_entry);
  if (gtk_toggle_button_get_active(bspline_check)){
        fill_treeview(name,"B-Spline Curve");
        BsplineCurve* curve = new BsplineCurve(name, curve_coords);
        viewport->addObject(curve);
        curve_coords.clear();
  } else {
        fill_treeview(name,"Bezier Curve");
        BezierCurve* curve = new BezierCurve(name, curve_coords);
        viewport->addObject(curve);
        curve_coords.clear();
  }
 
  
    gtk_label_set_text(label_number_points, "  0  ");
    gtk_entry_set_text(x_curve_entry, "");
    gtk_entry_set_text(y_curve_entry, "");
    gtk_entry_set_text(z_curve_entry, "");
    gtk_entry_set_text(name_curve_entry, "");
}	

void on_add_face_clicked (GtkWidget *widget, gpointer data) {
    isObject3D = true;
    gtk_widget_show (GTK_WIDGET(add_poly_w));
}

void on_add_object3D_clicked (GtkWidget *widget, gpointer data) {
    const gchar* name = gtk_entry_get_text(name_object3D_entry);
    Object3D* object = new Object3D(name, faces_object3D);
    fill_treeview(name,"Object3D");
    faces_object3D.clear();
    viewport->addObject(object);
}

/* Buttons Change object */
void on_angle_world_button_clicked (GtkWidget *widget, gpointer data) {
	double angle = atof(gtk_entry_get_text(angle_world_entry));
	if (gtk_toggle_button_get_active(x_checkr)) {
	    accumulator.push_back(Transformation::generate_rx(angle,false));
	} else if (gtk_toggle_button_get_active(y_checkr)) {
	    accumulator.push_back(Transformation::generate_ry(angle,false));
	} else if (gtk_toggle_button_get_active(z_checkr)) {
	    accumulator.push_back(Transformation::generate_rz(angle,false));
	}
}

void on_angle_obj_button_clicked(GtkWidget *widget, gpointer data) {
	int index = get_index_selected();
	Coordinate center = (viewport->getObject(index))->get_center_coord();
	double angle = atof(gtk_entry_get_text(angle_obj_entry));
	
	//accumulator.push_back(Transformation::generate_full_rotation(0,0,angle,0,center));
	accumulator.push_back(Transformation::generate_translation_matrix(-center[0], -center[1], -center[2]));
	if (gtk_toggle_button_get_active(x_checkr)) {
	    accumulator.push_back(Transformation::generate_rx(angle,false));
	} else if (gtk_toggle_button_get_active(y_checkr)) {
	    accumulator.push_back(Transformation::generate_ry(angle,false));
	} else if (gtk_toggle_button_get_active(z_checkr)) {
	    accumulator.push_back(Transformation::generate_rz(angle,false));
	}
	accumulator.push_back(Transformation::generate_translation_matrix(center[0], center[1], center[2]));

}

void on_translate_button_clicked (GtkWidget *widget, gpointer data) {
	double dx = atof(gtk_entry_get_text(trans_x_entry));
	double dy = atof(gtk_entry_get_text(trans_y_entry));
	double dz = atof(gtk_entry_get_text(trans_z_entry));
	accumulator.push_back(Transformation::generate_translation_matrix(dx, dy, dz));
}

void on_rotate_point_button_clicked (GtkWidget *widget, gpointer data) {
	double angle = atof(gtk_entry_get_text(angle_point_entry));
	double x = atof(gtk_entry_get_text(angle_pointx_entry));
	double y = atof(gtk_entry_get_text(angle_pointy_entry));
	double z = atof(gtk_entry_get_text(angle_pointz_entry));
	double xv = atof(gtk_entry_get_text(angle_vectorx_entry));
	double yv = atof(gtk_entry_get_text(angle_vectory_entry));
	double zv = atof(gtk_entry_get_text(angle_vectorz_entry));
	
	Coordinate p(x, y, z);
	Coordinate v(xv, yv, zv);
	accumulator.push_back(Transformation::generate_ra(angle, p, v));

}
//ok
void on_schedule_button_clicked (GtkWidget *widget, gpointer data) {
	double sx = atof(gtk_entry_get_text(sx_entry));
	double sy = atof(gtk_entry_get_text(sy_entry));
	double sz = atof(gtk_entry_get_text(sz_entry));

	int index = get_index_selected();
	Coordinate center = (viewport->getObject(index))->get_center_coord();
	accumulator.push_back(Transformation::generate_scaling_around_obj_center_matrix(sx, sy, sz, center));
}	 	  	 	     	  		  	  	    	      	 	
void on_change_obj_button_clicked (GtkWidget *widget, gpointer data) {
	//std::string name = get_name_selected();
	//aqui será onde será feita a multiplicação da matriz final no objeto com nome name
	Matrix identity = { {1, 0, 0, 0},
	         			{0, 1, 0, 0},
	         			{0, 0, 1, 0},
	         			{0, 0, 0, 1} };
	Transformation id = Transformation(identity);
	for(int i=0; i < accumulator.size(); i++){
		id *= accumulator.at(i);
	}
	Object* obj = viewport->getObject(get_index_selected());
	obj->transform_coords(id);
	viewport->normalize_and_clip_obj(obj);
	accumulator.clear();
}

gboolean draw_objects(GtkWidget* widget, cairo_t* cr, gpointer data) {
	cairo_set_source_rgb (cr, 1, 0, 0);
	cairo_set_source_rgb(cr, 1, 1, 1);
	cairo_paint(cr);

    //int width = gtk_widget_get_allocated_width (widget);
    //int height = gtk_widget_get_allocated_height (widget);
    //530x535
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 1.0);

    viewport->drawDisplayFile(cr);
  
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width(cr, 2.0);

    cairo_move_to(cr, 10, 10);
    cairo_line_to(cr, 520, 10);
    cairo_line_to(cr, 520, 525);
    cairo_line_to(cr, 10, 525);
    cairo_line_to(cr, 10, 10);
    cairo_stroke(cr);


	gtk_widget_queue_draw(draw_viewport);
	return FALSE;
}	 	  	 	     	  		  	  	    	      	 	

int get_index_selected() {
	GtkTreeIter iter;
	GtkTreeModel *model;
	int index;

	if(gtk_tree_selection_get_selected (objects_select, &model, &iter)) {
		gtk_tree_model_get (model, &iter, COL_ID, &index, -1);
	}
	return index;
}

void fill_treeview (const char* name, const char* type) {
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter, COL_ID, count_obj, COL_NAME, name, COL_TYPE, type,-1);
	count_obj++;
}

void check() {
    if (gtk_toggle_button_get_active(CS_Clipping)){
        gtk_toggle_button_set_active(LB_Clipping, false);
        viewport->changeLineClipAlg(Line_clip_algs::CS);
    } else if(gtk_toggle_button_get_active(LB_Clipping)) {
        gtk_toggle_button_set_active(CS_Clipping, false);
        viewport->changeLineClipAlg(Line_clip_algs::LB);
    } else 
        viewport->changeLineClipAlg(Line_clip_algs::CS);
}

void check_x() {
    if (gtk_toggle_button_get_active(x_check)) {
        gtk_toggle_button_set_active(y_check, false);
        gtk_toggle_button_set_active(z_check, false);
    }
}

void check_y() {
    if (gtk_toggle_button_get_active(y_check)) {
        gtk_toggle_button_set_active(x_check, false);
        gtk_toggle_button_set_active(z_check, false);  
    }
}

void check_z() {
    if (gtk_toggle_button_get_active(z_check)) {
        gtk_toggle_button_set_active(x_check, false);
        gtk_toggle_button_set_active(y_check, false);
    }
}

void check_xr() {
    if (gtk_toggle_button_get_active(x_checkr)) {
        gtk_toggle_button_set_active(y_checkr, false);
        gtk_toggle_button_set_active(z_checkr, false);
    }
}

void check_yr() {
    if (gtk_toggle_button_get_active(y_checkr)) {
        gtk_toggle_button_set_active(x_checkr, false);
        gtk_toggle_button_set_active(z_checkr, false);  
    }
}

void check_zr() {
    if (gtk_toggle_button_get_active(z_checkr)) {
        gtk_toggle_button_set_active(x_checkr, false);
        gtk_toggle_button_set_active(y_checkr, false);
    }
}
        
void create_treeview (void) {
	GtkCellRenderer *renderer;
	GtkTreeModel *model;

	/* Column 1 */
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (objects_tree), -1, "ID", renderer, "text", COL_ID, NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (objects_tree), -1, "Name", renderer, "text", COL_NAME, NULL);
	/* Column 2 */
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (objects_tree), -1, "Type", renderer, "text", COL_TYPE, NULL);


	model = GTK_TREE_MODEL (store = gtk_list_store_new (NUM_COLS, G_TYPE_UINT, G_TYPE_STRING, G_TYPE_STRING));
	gtk_tree_view_set_model (GTK_TREE_VIEW (objects_tree), model);

	objects_select = gtk_tree_view_get_selection(objects_tree);
	gtk_tree_selection_set_mode(objects_select, GTK_SELECTION_SINGLE);

	g_object_unref (model);
}	 	  	 	     	  		  	  	    	      	 	

int main (int argc, char *argv[]) {
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
	viewport = new Viewport(510,515);
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

	rotate_right = GTK_BUTTON(gtk_builder_get_object(builder, "rotate_right"));
	g_signal_connect (rotate_right, "clicked", G_CALLBACK (on_rotate_right_clicked), NULL);

	rotate_left = GTK_BUTTON(gtk_builder_get_object(builder, "rotate_left"));
	g_signal_connect (rotate_left, "clicked", G_CALLBACK (on_rotate_left_clicked), NULL);

	/* Buttons add_geometric_shape_w, just open a new window*/
	add_geometric_shape_w = gtk_builder_get_object (builder, "add_geometric_shape_w");
	g_signal_connect (add_geometric_shape_w, "delete_event", G_CALLBACK (gtk_widget_hide_on_delete), NULL);

	add_point1 = GTK_BUTTON(gtk_builder_get_object(builder, "add_point1"));
	g_signal_connect (add_point1, "clicked", G_CALLBACK (on_add_point1_clicked), NULL);

	add_line1 = GTK_BUTTON(gtk_builder_get_object(builder, "add_line1"));
	g_signal_connect (add_line1, "clicked", G_CALLBACK (on_add_line1_clicked), NULL);

	add_poly1 = GTK_BUTTON(gtk_builder_get_object(builder, "add_poly1"));
	g_signal_connect (add_poly1, "clicked", G_CALLBACK (on_add_poly1_clicked), NULL);

    add_curve1 = GTK_BUTTON(gtk_builder_get_object(builder, "add_curve1"));
    g_signal_connect (add_curve1, "clicked", G_CALLBACK (on_add_curve1_clicked), NULL);
    
    add_object3D1 = GTK_BUTTON(gtk_builder_get_object(builder, "add_object3D1"));
    g_signal_connect (add_object3D1, "clicked", G_CALLBACK (on_add_object3D1_clicked), NULL);


	/* Delete action windows*/
	add_point_w = gtk_builder_get_object (builder, "add_point_w");
	g_signal_connect (add_point_w, "delete_event", G_CALLBACK (gtk_widget_hide_on_delete), NULL);

	add_line_w = gtk_builder_get_object (builder, "add_line_w");
	g_signal_connect (add_line_w, "delete_event", G_CALLBACK (gtk_widget_hide_on_delete), NULL);

	add_poly_w = gtk_builder_get_object (builder, "add_poly_w");
	g_signal_connect (add_poly_w, "delete_event", G_CALLBACK (gtk_widget_hide_on_delete), NULL);

    add_curve_w = gtk_builder_get_object (builder, "add_curve_w");
    g_signal_connect (add_curve_w, "delete_event", G_CALLBACK (gtk_widget_hide_on_delete), NULL);
    
    add_object3D_w = gtk_builder_get_object (builder, "add_object3D_w");
    g_signal_connect (add_object3D_w, "delete_event", G_CALLBACK (gtk_widget_hide_on_delete), NULL);

	/* Buttons point, line, poly, curve and object3D*/
	add_point = GTK_BUTTON(gtk_builder_get_object(builder, "add_point"));
	g_signal_connect (add_point, "clicked", G_CALLBACK (on_add_point_clicked), NULL);

	add_line = GTK_BUTTON(gtk_builder_get_object(builder, "add_line"));
	g_signal_connect (add_line, "clicked", G_CALLBACK (on_add_line_clicked), NULL);

	add_point_poly = GTK_BUTTON(gtk_builder_get_object(builder, "add_point_poly"));
	g_signal_connect (add_point_poly, "clicked", G_CALLBACK (on_add_point_poly_clicked), NULL);

	add_poly = GTK_BUTTON(gtk_builder_get_object(builder, "add_poly"));
	g_signal_connect (add_poly, "clicked", G_CALLBACK (on_add_poly_clicked), NULL);

    add_point_curve = GTK_BUTTON(gtk_builder_get_object(builder, "add_point_curve"));
    g_signal_connect (add_point_curve, "clicked", G_CALLBACK (on_add_point_curve_clicked), NULL);

    add_curve = GTK_BUTTON(gtk_builder_get_object(builder, "add_curve"));
    g_signal_connect (add_curve, "clicked", G_CALLBACK (on_add_curve_clicked), NULL);
    
    add_face = GTK_BUTTON(gtk_builder_get_object(builder, "add_face"));
    g_signal_connect (add_face, "clicked", G_CALLBACK (on_add_face_clicked), NULL);
    
    add_object3D = GTK_BUTTON(gtk_builder_get_object(builder, "add_object3D"));
    g_signal_connect (add_object3D, "clicked", G_CALLBACK (on_add_object3D_clicked), NULL);
    
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
	angle_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_entry"));
	name_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "name_point_entry"));
	x1_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "x1_point_entry"));
	y1_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "y1_point_entry"));
	z1_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "z1_point_entry"));
	name_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "name_line_entry"));
	x1_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "x1_line_entry"));
	y1_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "y1_line_entry"));
	z1_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "z1_line_entry"));
	x2_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "x2_line_entry"));
	y2_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "y2_line_entry"));
	z2_line_entry = GTK_ENTRY(gtk_builder_get_object(builder, "z2_line_entry"));
	name_poly_entry = GTK_ENTRY(gtk_builder_get_object(builder, "name_poly_entry"));
	x_poly_entry = GTK_ENTRY(gtk_builder_get_object(builder, "x_poly_entry"));
	y_poly_entry = GTK_ENTRY(gtk_builder_get_object(builder, "y_poly_entry"));
	z_poly_entry = GTK_ENTRY(gtk_builder_get_object(builder, "z_poly_entry"));
	angle_world_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_world_entry"));
	angle_obj_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_obj_entry"));
	trans_x_entry = GTK_ENTRY(gtk_builder_get_object(builder, "trans_x_entry"));
	trans_y_entry = GTK_ENTRY(gtk_builder_get_object(builder, "trans_y_entry"));
	trans_z_entry = GTK_ENTRY(gtk_builder_get_object(builder, "trans_z_entry"));
	angle_point_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_point_entry"));
	angle_pointx_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_pointx_entry"));
	angle_pointy_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_pointy_entry"));
	angle_pointz_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_pointz_entry"));
	angle_vectorx_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_vectorx_entry"));
	angle_vectory_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_vectory_entry"));
	angle_vectorz_entry = GTK_ENTRY(gtk_builder_get_object(builder, "angle_vectorz_entry"));
	sx_entry = GTK_ENTRY(gtk_builder_get_object(builder, "sx_entry"));
	sy_entry = GTK_ENTRY(gtk_builder_get_object(builder, "sy_entry"));
	sz_entry = GTK_ENTRY(gtk_builder_get_object(builder, "sz_entry"));
    filled = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"fill_poly_checkButton"));
    CS_Clipping = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"CS_Clipping"));
    LB_Clipping = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"LB_Clipping"));
    g_signal_connect(CS_Clipping, "toggled", G_CALLBACK(check), NULL);
    g_signal_connect(LB_Clipping, "toggled", G_CALLBACK(check), NULL);
    name_curve_entry = GTK_ENTRY(gtk_builder_get_object(builder, "name_curve_entry"));
    x_curve_entry = GTK_ENTRY(gtk_builder_get_object(builder, "x_curve_entry"));
    y_curve_entry = GTK_ENTRY(gtk_builder_get_object(builder, "y_curve_entry"));
    z_curve_entry = GTK_ENTRY(gtk_builder_get_object(builder, "z_curve_entry"));
    label_number_points = GTK_LABEL(gtk_builder_get_object(builder, "label_number_points"));
    bspline_check = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"bspline_check"));
    x_check = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"x_check"));
    y_check = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"y_check"));
    z_check = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"z_check"));
    x_checkr = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"x_checkr"));
    y_checkr = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"y_checkr"));
    z_checkr = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder,"z_checkr"));
    g_signal_connect(x_check, "toggled", G_CALLBACK(check_x), NULL);
    g_signal_connect(y_check, "toggled", G_CALLBACK(check_y), NULL);
    g_signal_connect(z_check, "toggled", G_CALLBACK(check_z), NULL);
    g_signal_connect(x_checkr, "toggled", G_CALLBACK(check_xr), NULL);
    g_signal_connect(y_checkr, "toggled", G_CALLBACK(check_yr), NULL);
    g_signal_connect(z_checkr, "toggled", G_CALLBACK(check_zr), NULL);
    name_object3D_entry = GTK_ENTRY(gtk_builder_get_object(builder, "name_object3D_entry"));
    
	gtk_widget_show(GTK_WIDGET(main_w));

	gtk_main ();

	return 0;
}	 	  	 	     	  		  	  	    	      	 	
