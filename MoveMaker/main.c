#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>

#include "headers.h"

int moveCnt = 0;
int currMove = -1;

char moveList[100][13];

GtkWidget *cbt_move;
GtkWidget *cbt_type;
GtkWidget *e_name;
GtkWidget *tv_script;
GtkWidget *b_update;
GtkWidget *sb_pp;

moveObj_t moves[50];

void toggleEditable(gboolean editable)
{
	gtk_widget_set_sensitive(GTK_WIDGET(cbt_type), editable);
	gtk_widget_set_sensitive(GTK_WIDGET(e_name), editable);
	gtk_widget_set_sensitive(GTK_WIDGET(tv_script), editable);
	gtk_widget_set_sensitive(GTK_WIDGET(b_update), editable);
	gtk_widget_set_sensitive(GTK_WIDGET(sb_pp), editable);
}

void clearMoves()
{
	moveCnt = 0;
	currMove = -1;
	for(int i=0; i<100; i++) {
		for(int j=0; j<12; j++) {
			moveList[i][j] = ' ';
		}
	}
}

void newMove(char* name)
{
	bool fill = false;
	for(int i=0; i<12; i++) {
		if(name[i] == ' ' && name[i+1] == ' ') {
			fill = true;
		}
		if(fill) {
			moveList[moveCnt][i] = ' ';
		} else {
			moveList[moveCnt][i] = name[i];
		}
	}
	moveCnt++;
}

void editMove(char* name, int val)
{
	bool fill = false;
	for(int i=0; i<12; i++) {
		if(name[i] == ' ' && name[i+1] == ' ') {
			fill = true;
		}
		if(fill) {
			moveList[val][i] = ' ';
		} else {
			moveList[val][i] = name[i];
		}
	}
}

void updateMoves()
{
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(cbt_move));
	for(int i=0; i<moveCnt; i++) {
		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(cbt_move), currMove,
			moveList[i]);
	}
}

void loadTypes(char *filename)
{
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(cbt_type));

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

		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(cbt_type), i,
			tmpChar);

		printf("   %s\n", tmpChar);

		fseek(file, resCnt*(sizeof(int)+sizeof(char))+weakCnt*sizeof(int),
			SEEK_CUR);
	}

	fclose(file);

	printf("done!\n");
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
	gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_type), moves[currMove].info.type);

	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_pp), moves[currMove].info.pp);

	if(moves[currMove].script != NULL) {
		gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv_script)),
			moves[currMove].script,	moves[currMove].info.scriptLen);
	}
}

void cbt_type_changed(GtkWidget *obj, gpointer user_data)
{
	moves[currMove].info.type =
		gtk_combo_box_get_active(GTK_COMBO_BOX(cbt_type));
}

void b_update_clicked(GtkWidget *obj, gpointer user_data)
{
	if(currMove == -1) {
		return;
	}

	// Set script text
	moves[currMove].script = getTextViewText(tv_script);
	moves[currMove].info.scriptLen = getTextViewLen(tv_script);

	// Gtk sets currMove to -1 when entries are removed
	int tmpCurrMove = currMove;

	editMove((char*)gtk_entry_get_text(GTK_ENTRY(e_name)), currMove);
	updateMoves();

	gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_move), tmpCurrMove);

}

void sb_pp_changed(GtkWidget *obj, gpointer user_data)
{
	moves[currMove].info.pp =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sb_pp));
}

void b_newmove_clicked(GtkWidget *obj, gpointer user_data)
{
	newMove("NEW");
	updateMoves();
}

void b_load_clicked(GtkWidget *obj, gpointer user_data)
{
	clearMoves();
	printf("Loading..\n");
	FILE * file;
	file = fopen("../resources/data/moves.pke", "r");

	if(file == NULL) {
		printf("[ERROR] Missing types.pke file!\n");
		return;
	}

	moveHeader_f header;
	fread(&header.version, sizeof(int), 1, file);
	if(header.version != CURRVERSION) {
		printf("[ERROR] moves.pke is V%i, but V%i is expected!\n",
			header.version, CURRVERSION);
		return;
	}
	fread(&header.count, sizeof(int), 1, file);
	
	for(int i=0; i<header.count; i++) {
		char tmpChar[12];
		fread(&tmpChar[0], sizeof(char[12]), 1, file);
		newMove(tmpChar);
		fread(&moves[i].info.type, sizeof(int), 1, file);
		fread(&moves[i].info.scriptLen, sizeof(int), 1, file);

		printf("   %s\n", moveList[i]);

		printf("   Type          : %i\n", moves[i].info.type);
		printf("   Script Length : %i\n", moves[i].info.scriptLen);

		moves[i].script = (char*)malloc(sizeof(char)*moves[i].info.scriptLen);
		fread(moves[i].script, sizeof(char), moves[i].info.scriptLen, file);
	}

	fclose(file);

	updateMoves();
	printf("done!\n");
}

void b_save_clicked(GtkWidget *obj, gpointer user_data)
{
	printf("Start\n");
	FILE * file;
	file = fopen("../resources/data/moves.pke", "wb");

	if(file == NULL) {
		printf("[ERROR] Creating moves.pke file!\n");
		return;
	}

	moveHeader_f header;
	header.version = CURRVERSION;
	header.count = moveCnt;

	fwrite(&header.version, sizeof(int), 1, file);
	fwrite(&header.count, sizeof(int), 1, file);

	for(int i=0; i<moveCnt; i++) {
		for(int j=0; j<12; j++) {
			moves[i].info.name[j] = moveList[i][j];
		}
		fwrite(&moves[i].info.name, sizeof(char), 12, file);

		printf("   Saving %s...\n", moves[i].info.name);

		printf("   Type: %i\n", moves[i].info.type);
		printf("   PP: %i\n", moves[i].info.pp);
		printf("   Script Length: %i\n", moves[i].info.scriptLen);

		fwrite(&moves[i].info.type, sizeof(int), 1, file);
		fwrite(&moves[i].info.pp, sizeof(int), 1, file);
		fwrite(&moves[i].info.scriptLen, sizeof(int), 1, file);

		fwrite(moves[i].script, sizeof(char), moves[i].info.scriptLen, file);
	}

	fclose(file);

	printf("done!\n\n");
}

int main(int argc, char **argv) {

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

	return 0;
}

