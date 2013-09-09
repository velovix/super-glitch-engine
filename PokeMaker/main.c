#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>

#include "../common/typeFile.h"
#include "../common/moveFile.h"
#include "../common/monFile.h"

// Window constants
#define W_MAIN 			0
#define W_MOVES			1
#define W_AESTHETICS	2
#define W_STATS			3
#define W_OTHERS		4
#define W_EVOLUTIONS	5
#define W_EXP			6

int currMon = -1;

GtkWidget *windows[7];
int currEditWindow = 0;

// Widgets
GtkWidget *cb_evolves;
GtkWidget *cb_nogender;
GtkWidget *sb_genderratio;
GtkWidget *sb_evolevel;
GtkWidget *e_name;

GtkWidget *cbt_monsters;
GtkWidget *cbt_evomon;
GtkWidget *cbt_type1, *cbt_type2;

GtkWidget *sb_hp, *sb_att, *sb_def, *sb_spatt, *sb_spdef, *sb_speed;
GtkWidget *sb_ehp, *sb_eatt, *sp_edef, *sp_espatt, *sp_espdef, *sp_espeed;

monFile_t monFile;

void toggleEditable(gboolean editable) {

}

void newMonFile() {
	// Prime the monFile_t for later use
	monFile.header.version = monFILE_VERSION;
	monFile.header.count = 0;

	monFile.mons = NULL;
}

void newMon(char *name) {
	// Allocate more monster data
	monFile.mons =
		(monFileObj_t*)realloc(monFile.mons, sizeof(monFileObj_t)*(monFile.header.count+1));

	int curr = monFile.header.count;
	// Set the name
	for(int i=0; i<10; i++) {
		monFile.mons[curr].basic.name[i] = name[i];
	}

	// Make all settings default
	monFile.mons[curr].basic.type1 = monFile.mons[curr].basic.type2 = 0;
	monFile.mons[curr].basic.height = monFile.mons[curr].basic.weight = 0;
	monFile.mons[curr].stats.b_hp = monFile.mons[curr].stats.b_att =
		monFile.mons[curr].stats.b_def = 0;
	monFile.mons[curr].stats.b_spAtt = monFile.mons[curr].stats.b_spDef =
		monFile.mons[curr].stats.b_speed = 0;
	monFile.mons[curr].stats.ev_hp = monFile.mons[curr].stats.ev_att =
		monFile.mons[curr].stats.ev_def = 0;
	monFile.mons[curr].stats.ev_spAtt = monFile.mons[curr].stats.ev_spDef =
		monFile.mons[curr].stats.ev_speed = 0;
	monFile.mons[curr].movelist.natLearnedCnt = monFile.mons[curr].movelist.tmLearnedCnt = 0;
	monFile.mons[curr].movelist.natLearned = NULL;
	monFile.mons[curr].movelist.tmLearned = NULL;
	monFile.mons[curr].experience.type = 0;
	monFile.mons[curr].evolution.evolves = 0;
	monFile.mons[curr].aesthetics.battleSprite = monFile.mons[curr].aesthetics.icon =
		monFile.mons[curr].aesthetics.body = monFile.mons[curr].aesthetics.footprint = 0;
	monFile.mons[curr].etc.genderless = 0;
	monFile.mons[curr].etc.malePerc = 50;
	monFile.mons[curr].etc.ability = 0;
	monFile.mons[curr].etc.catchRate = 255;
	monFile.mons[curr].etc.heldItem = 0;

	monFile.header.count++;
}

void updateMons() {
	// Remove the combo box entries and reinput to guarantee order
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(cbt_monsters));
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(cbt_evomon));
	for(int i=0; i<monFile.header.count; i++) {
		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(cbt_monsters), i,
			monFile.mons[i].basic.name);
		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(cbt_evomon), i,
			monFile.mons[i].basic.name);
	}
}

void openEditWindow(int newWindow) {
	gtk_widget_show(windows[newWindow]);
}

// --- MAIN WINDOW BUTTONS CLICKED ---
void b_editstats_clicked(GtkWidget *obj, gpointer user_data) {
	openEditWindow(W_STATS);
}

void b_editaesthetics_clicked(GtkWidget *obj, gpointer user_data) {
	openEditWindow(W_AESTHETICS);
}

void b_editmovelist_clicked(GtkWidget *obj, gpointer user_data) {
	openEditWindow(W_MOVES);
}

void b_editothers_clicked(GtkWidget *obj, gpointer user_data) {
	openEditWindow(W_OTHERS);
}

void b_editevolutions_clicked(GtkWidget *obj, gpointer user_data) {
	openEditWindow(W_EVOLUTIONS);
}

void b_editexp_clicked(GtkWidget *obj, gpointer user_data) {
	openEditWindow(W_EXP);
}

// --- OTHER STUFF
void cb_evolves_toggled(GtkWidget *obj, gpointer user_data) {
	gboolean on = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obj));

	gtk_widget_set_sensitive(cbt_evomon, on);
	gtk_widget_set_sensitive(sb_evolevel, on);
}

void cb_nogender_toggled(GtkWidget *obj, gpointer user_data) {
	gboolean on = !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obj));

	gtk_widget_set_sensitive(sb_genderratio, on);
}

void b_new_clicked(GtkWidget *obj, gpointer user_data) {
	newMon("NEW");

	updateMons();

	// Select the newly made monster
	gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_monsters), monFile.header.count-1);
}

void cbt_monsters_changed(GtkWidget *obj, gpointer user_data) {

	currMon = gtk_combo_box_get_active(GTK_COMBO_BOX(obj));

	// If no monster is selected, nothing else needs to be done
	if(currMon == -1) {
		return;
	}

	// Update widgets to reflect monster
	gtk_entry_set_text(GTK_ENTRY(e_name), gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(obj)));
}

void b_update_clicked(GtkWidget *obj, gpointer user_data) {

	// GTK likes to reset dropdowns when they are changed
	int tmpCurrMon = currMon;

	char *name = (char*)gtk_entry_get_text(GTK_ENTRY(e_name));

	bool fill = false;
	for(int i=0; i<8; i++) {
		if(name[i] == ' ' && name[i+1] == ' ') {
			fill = true;
		}
		if(fill) {
			monFile.mons[currMon].basic.name[i] = ' ';
		} else {
			monFile.mons[currMon].basic.name[i] = name[i];
		}
	}

	updateMons();

	// Set the combo boxes back to their original state
	gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_monsters), tmpCurrMon);
}

void loadTypes(char *filename) {
	typeFile_t typeFile;

	pk_openTypeFile(&typeFile, filename);

	for(int i=0; i<typeFile.header.count; i++) {
		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(cbt_type1), i, typeFile.types[i].header.name);
		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(cbt_type2), i, typeFile.types[i].header.name);
	}

	pk_freeTypeFile(&typeFile);
}

void loadMoves(char *filename) {
	moveFile_t moveFile;

	pk_openMoveFile(&moveFile, filename);

	pk_freeMoveFile(&moveFile);
}

void on_window_destroy (GtkWidget *object, gpointer user_data) {
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

	// Make extra windows only hide when you "x" out
	for(int i=W_MOVES; i<7; i++) {
		g_signal_connect (G_OBJECT(windows[i]), "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);
	}

	// Load widgets
	cb_evolves =			GTK_WIDGET (gtk_builder_get_object(builder, "cb_evolves"));
	cb_nogender = 			GTK_WIDGET (gtk_builder_get_object(builder, "cb_nogender"));
	sb_genderratio = 		GTK_WIDGET (gtk_builder_get_object(builder,"sb_genderratio"));
	sb_evolevel = 			GTK_WIDGET (gtk_builder_get_object(builder, "sb_evolevel"));
	e_name = 				GTK_WIDGET (gtk_builder_get_object(builder, "e_name"));

	cbt_monsters =			GTK_WIDGET (gtk_builder_get_object(builder, "cbt_monsters"));
	cbt_evomon = 			GTK_WIDGET (gtk_builder_get_object(builder, "cbt_evomon"));
	cbt_type1 = 			GTK_WIDGET (gtk_builder_get_object(builder, "cbt_type1"));
	cbt_type2 = 			GTK_WIDGET (gtk_builder_get_object(builder, "cbt_type2"));

	sb_hp = 				GTK_WIDGET (gtk_builder_get_object(builder, "sb_hp"));
	sb_att = 				GTK_WIDGET (gtk_builder_get_object(builder, "cb_att"));
	sb_def = 				GTK_WIDGET (gtk_builder_get_object(builder, "cb_def"));
	sb_spatt = 				GTK_WIDGET (gtk_builder_get_object(builder, "cb_spatt"));
	sb_spdef = 				GTK_WIDGET (gtk_builder_get_object(builder, "cb_spdef"));
	sb_speed = 				GTK_WIDGET (gtk_builder_get_object(builder, "cb_speed"));
	sb_ehp = 				GTK_WIDGET (gtk_builder_get_object(builder, "cb_ehp"));
	sb_eatt = 				GTK_WIDGET (gtk_builder_get_object(builder, "cb_eatt"));
	sp_edef = 				GTK_WIDGET (gtk_builder_get_object(builder, "cb_edef"));
	sp_espatt = 			GTK_WIDGET (gtk_builder_get_object(builder, "cb_espatt"));
	sp_espdef = 			GTK_WIDGET (gtk_builder_get_object(builder, "cb_espdef"));
	sp_espeed = 			GTK_WIDGET (gtk_builder_get_object(builder, "cb_espeed"));

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));

	// Load external resources
	loadTypes("../resources/data/types.pke");
	loadMoves("../resources/data/moves.pke");

	toggleEditable(FALSE);

	gtk_widget_show(windows[W_MAIN]);
	gtk_main();

	return 0;
}

