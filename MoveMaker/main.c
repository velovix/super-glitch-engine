#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>

#include "../common/moveFile.h"
#include "../common/typeFile.h"

int currMove = -1;

GtkWidget *cbt_move;
GtkWidget *cbt_type;
GtkWidget *e_name;
GtkWidget *tv_script;
GtkWidget *b_update;
GtkWidget *sb_pp;

moveFile_t moveFile;

void toggleEditable(gboolean editable)
{
	gtk_widget_set_sensitive(GTK_WIDGET(cbt_type), editable);
	gtk_widget_set_sensitive(GTK_WIDGET(e_name), editable);
	gtk_widget_set_sensitive(GTK_WIDGET(tv_script), editable);
	gtk_widget_set_sensitive(GTK_WIDGET(b_update), editable);
	gtk_widget_set_sensitive(GTK_WIDGET(sb_pp), editable);
}

void newMapFile()
{
	moveFile.header.version = MOVEFILE_VERSION;
	moveFile.header.count = 0;

	moveFile.moves = NULL;
}

void newMove(char* name)
{
	bool fill = false;

	// Allocate more move space
	moveFile.moves =
		(moveEntry_f*)realloc(moveFile.moves, sizeof(moveEntry_f)*(moveFile.header.count+1));

	moveFile.moves[moveFile.header.count].scriptLen = 0;
	moveFile.moves[moveFile.header.count].type = 0;
	moveFile.moves[moveFile.header.count].pp = 0;
	moveFile.moves[moveFile.header.count].script = NULL;

	for(int i=0; i<12; i++) {
		if(name[i] == ' ' && name[i+1] == ' ') {
			fill = true;
		}
		if(fill) {
			moveFile.moves[moveFile.header.count].name[i] = ' ';
		} else {
			moveFile.moves[moveFile.header.count].name[i] = name[i];
		}
	}
	moveFile.header.count++;
}

void editMove(char* name, int val)
{
	bool fill = false;
	for(int i=0; i<12; i++) {
		if(name[i] == ' ' && name[i+1] == ' ') {
			fill = true;
		}
		if(fill) {
			moveFile.moves[val].name[i] = ' ';
		} else {
			moveFile.moves[val].name[i] = name[i];
		}
	}
}

void updateMoves()
{
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(cbt_move));
	for(int i=0; i<moveFile.header.count; i++) {
		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(cbt_move), currMove,
			moveFile.moves[i].name);
	}
}

void loadTypes(char *filename)
{
	typeFile_t typeFile;

	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(cbt_type));

	pk_openTypeFile(&typeFile, filename);

	for(int i=0; i<typeFile.header.count; i++) {
		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(cbt_type), i,
			typeFile.types[i].header.name);
	}

	pk_freeTypeFile(&typeFile);
}

char *getTextViewText(GtkWidget *object)
{
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(object));
	char *text;

	gtk_text_buffer_get_bounds (buffer, &start, &end);
	text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);

	return text;
}

int getTextViewLen(GtkWidget *object)
{
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(object));

	return (int)gtk_text_buffer_get_char_count(buffer);
}

void on_window_destroy (GtkWidget *object, gpointer user_data)
{
	gtk_main_quit();
}

void cbt_move_changed(GtkWidget *obj, gpointer user_data)
{
	currMove = gtk_combo_box_get_active(GTK_COMBO_BOX(obj));
	if(currMove == -1) {
		toggleEditable(FALSE);
		return;
	} else {
		toggleEditable(TRUE);
	}
		
	gtk_entry_set_text(GTK_ENTRY(e_name), 
		gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(obj)));
	gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_type), moveFile.moves[currMove].type);

	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_pp), moveFile.moves[currMove].pp);

	if(moveFile.moves[currMove].script != NULL) {
		gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv_script)),
			moveFile.moves[currMove].script, moveFile.moves[currMove].scriptLen);
	}
}

void cbt_type_changed(GtkWidget *obj, gpointer user_data)
{
	moveFile.moves[currMove].type =
		gtk_combo_box_get_active(GTK_COMBO_BOX(cbt_type));
}

void b_update_clicked(GtkWidget *obj, gpointer user_data)
{
	if(currMove == -1) {
		return;
	}

	// Set script text
	moveFile.moves[currMove].script = getTextViewText(tv_script);
	moveFile.moves[currMove].scriptLen = getTextViewLen(tv_script);

	// Gtk sets currMove to -1 when entries are removed
	int tmpCurrMove = currMove;

	editMove((char*)gtk_entry_get_text(GTK_ENTRY(e_name)), currMove);
	updateMoves();

	gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_move), tmpCurrMove);

}

void sb_pp_changed(GtkWidget *obj, gpointer user_data)
{
	moveFile.moves[currMove].pp =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sb_pp));
}

void b_newmove_clicked(GtkWidget *obj, gpointer user_data)
{
	newMove("NEW");
	updateMoves();
}

void b_load_clicked(GtkWidget *obj, gpointer user_data)
{
	pk_freeMoveFile(&moveFile);

	pk_openMoveFile(&moveFile, "../resources/data/moves.pke");
	updateMoves();
}

void b_save_clicked(GtkWidget *obj, gpointer user_data)
{
	pk_saveMoveFile(&moveFile, "../resources/data/moves.pke");
}

int main(int argc, char **argv) {

	newMapFile();

	GtkBuilder *builder;
	GtkWidget *window;

	gtk_init (&argc, &argv);

	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "MoveMakerWind.glade", NULL);

	window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));

	cbt_move = GTK_WIDGET(gtk_builder_get_object(builder, "cbt_move"));
	cbt_type = GTK_WIDGET(gtk_builder_get_object(builder, "cbt_type"));
	e_name = GTK_WIDGET(gtk_builder_get_object(builder, "e_name"));
	tv_script = GTK_WIDGET(gtk_builder_get_object(builder, "tv_script"));
	b_update = GTK_WIDGET(gtk_builder_get_object(builder, "b_update"));
	sb_pp = GTK_WIDGET(gtk_builder_get_object(builder, "sb_pp"));
	

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));

	newMove("NEW");
	updateMoves();

	loadTypes("../resources/data/types.pke");

	toggleEditable(FALSE);

	gtk_widget_show(window);
	gtk_main();

	pk_freeMoveFile(&moveFile);
	return 0;
}

