#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>

#include "headers.h"

// Window constants
#define W_MAIN 			0
#define W_MOVES			1
#define W_AESTHETICS	2
#define W_STATS			3
#define W_OTHERS		4
#define W_EVOLUTIONS	5
#define W_EXP			6

GtkWidget *windows[5];
int currEditWindow = 0;

// Widgets
GtkWidget *tv_monsters;

void toggleEditable(gboolean editable)
{

}

void openEditWindow(int newWindow) {
	//gtk_widget_hide(windows[currWindow]);
	gtk_widget_show(windows[newWindow]);
}

// --- MAIN WINDOW BUTTONS CLICKED ---
void b_editstats_clicked(GtkWidget *obj, gpointer user_data)
{
	openEditWindow(W_STATS);
}

void b_editaesthetics_clicked(GtkWidget *obj, gpointer user_data)
{
	openEditWindow(W_AESTHETICS);
}

void b_editmovelist_clicked(GtkWidget *obj, gpointer user_data)
{
	openEditWindow(W_MOVES);
}

void b_editothers_clicked(GtkWidget *obj, gpointer user_data)
{
	openEditWindow(W_OTHERS);
}

void b_editevolutions_clicked(GtkWidget *obj, gpointer user_data)
{
	openEditWindow(W_EVOLUTIONS);
}

void b_editexp_clicked(GtkWidget *obj, gpointer user_data)
{
	openEditWindow(W_EXP);
}

// --- OTHER STUFF
void b_new_clicked(GtkWidget *obj, gpointer user_data)
{
	GtkTreeIter iter;
	GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(tv_monsters));
	gtk_list_store_append(GTK_LIST_STORE(model), &iter);
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, "NEW", -1);
}

void loadTypes(char *filename)
{
	printf("Loading types..\n");
	FILE * file;
	file = fopen(filename, "r");

	if(file == NULL) {
		printf("[ERROR] Missing types.pke file!\n");
		return;
	}

	typeHeader_f header;
	fread(&header.version, sizeof(int), 1, file);
	if(header.version != TYPE_VERSION) {
		printf("[ERROR] types.pke is V%i, but V%i is expected!\n",
			header.version, TYPE_VERSION);
		return;
	}
	fread(&header.count, sizeof(int), 1, file);
	
	for(int i=0; i<header.count; i++) {
		char tmpChar[8];
		int resCnt, weakCnt;
		fread(&tmpChar[0], sizeof(char[8]), 1, file);
		fread(&resCnt, sizeof(int), 1, file);
		fread(&weakCnt, sizeof(int), 1, file);

		printf("   %s\n", tmpChar);

		fseek(file, resCnt*(sizeof(int)+sizeof(char))+weakCnt*sizeof(int),
			SEEK_CUR);
	}

	fclose(file);

	printf("done!\n");
}

void on_window_destroy (GtkWidget *object, gpointer user_data)
{
	gtk_main_quit();
}

int main(int argc, char **argv) {

	GtkBuilder *builder;

	gtk_init (&argc, &argv);

	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "PokeMakerWind.glade", NULL);

	// Load up all windows
	windows[W_MAIN] = 		GTK_WIDGET (gtk_builder_get_object (builder, "w_main"));
	windows[W_STATS] = 		GTK_WIDGET (gtk_builder_get_object (builder, "w_stats"));
	windows[W_OTHERS] = 	GTK_WIDGET (gtk_builder_get_object (builder, "w_others"));
	windows[W_AESTHETICS] = GTK_WIDGET (gtk_builder_get_object (builder, "w_aesthetics"));
	windows[W_MOVES] = 		GTK_WIDGET (gtk_builder_get_object (builder, "w_moves"));
	windows[W_EXP] =		GTK_WIDGET (gtk_builder_get_object (builder, "w_exp"));
	windows[W_EVOLUTIONS] =	GTK_WIDGET (gtk_builder_get_object (builder, "w_evolutions"));

	// Load up individual objects
	tv_monsters = GTK_WIDGET(gtk_builder_get_object(builder, "tv_monsters"));

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));

	// Load external resources
	loadTypes("../resources/data/types.pke");


	// Set up the Tree View. Would be done in Glade, but I can't get it working
	// Set up data structure
	GtkTreeIter iter;
	GtkListStore *ls_tv_monsters = gtk_list_store_new(1, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(tv_monsters), GTK_TREE_MODEL(ls_tv_monsters));
	gtk_list_store_append(ls_tv_monsters, &iter);
	gtk_list_store_set(ls_tv_monsters, &iter, 0, "NEW", -1);
	// Set up display for Tree View
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	GtkTreeViewColumn *col = gtk_tree_view_column_new();
	col = gtk_tree_view_column_new_with_attributes ("Monster", renderer, "text", 0, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(tv_monsters), col);

	toggleEditable(FALSE);

	gtk_widget_show(windows[W_MAIN]);
	gtk_main();

	return 0;
}

