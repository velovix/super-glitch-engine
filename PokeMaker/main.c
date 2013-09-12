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

GtkWidget *b_update;
GtkWidget *b_editmoves, *b_editaesthetics, *b_editstats, *b_editothers,
	*b_editevolutions, *b_editexp;

GtkWidget *sb_weight, *sb_height;

GtkWidget *cbt_monsters;
GtkWidget *cbt_evomon;
GtkWidget *cbt_type1, *cbt_type2;

GtkWidget *e_species;
GtkWidget *tv_nmoves, *tv_tmmoves;

GtkWidget *sb_ability, *sb_catchrate, *sb_helditem;
GtkWidget *sb_battlesprite, *sb_icon, *sb_cry, *sb_bodystyle, *sb_footprint;
GtkWidget *sb_hp, *sb_att, *sb_def, *sb_spatt, *sb_spdef, *sb_speed;
GtkWidget *sb_ehp, *sb_eatt, *sb_edef, *sb_espatt, *sb_espdef, *sb_espeed;

GtkWidget *rb_erratic, *rb_fast, *rb_medfast, *rb_medslow, *rb_slow, *rb_fluctuating;

monFile_t monFile;

void toggleEditable(gboolean editable) {
	gtk_widget_set_sensitive(e_name, editable);
	gtk_widget_set_sensitive(b_update, editable);
	gtk_widget_set_sensitive(cbt_type1, editable);
	gtk_widget_set_sensitive(cbt_type2, editable);
	gtk_widget_set_sensitive(sb_weight, editable);
	gtk_widget_set_sensitive(sb_height, editable);

	gtk_widget_set_sensitive(b_editmoves, editable);
	gtk_widget_set_sensitive(b_editaesthetics, editable);
	gtk_widget_set_sensitive(b_editstats, editable);
	gtk_widget_set_sensitive(b_editothers, editable);
	gtk_widget_set_sensitive(b_editevolutions, editable);
	gtk_widget_set_sensitive(b_editexp, editable);
}

void newMonFile() {
	// Prime the monFile_t for later use
	monFile.header.version = MONFILE_VERSION;
	monFile.header.count = 0;

	monFile.mons = NULL;
}

void newMon(char *name) {
	printf("Making a new mon!\n");
	// Allocate more monster data
	monFile.mons =
		(monFileObj_t*)realloc(monFile.mons, sizeof(monFileObj_t)*(monFile.header.count+1));
	printf("--alloc'd\n");

	int curr = monFile.header.count;
	// Set the name
	for(int i=0; i<10; i++) {
		monFile.mons[curr].basic.name[i] = name[i];
	}
	printf("--named\n");

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

	printf("--done!\n\n");
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

void closeAllEditWindows() {
	for(int i=0; i<W_EXP; i++) {
		gtk_widget_hide(windows[i]);
	}
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
void b_load_clicked(GtkWidget *obj, gpointer user_data) {
	pk_freeMonFile(&monFile);

	int err = pk_openMonFile(&monFile, "../resources/data/mons.pke");
	if(err != 0) {
		printf("[ERROR] %i\n", err);
	}

	updateMons();
}

void b_save_clicked(GtkWidget *obj, gpointer user_data) {

	int err = pk_saveMonFile(&monFile, "../resources/data/mons.pke");
	if(err != 0) {
		printf("[ERROR] %i\n", err);
	}
}

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
	newMon("NEW ");

	updateMons();
}

void cbt_monsters_changed(GtkWidget *obj, gpointer user_data) {

	currMon = gtk_combo_box_get_active(GTK_COMBO_BOX(obj));

	// If no monster is selected, nothing else needs to be done
	if(currMon == -1) {
		toggleEditable(FALSE);
		return;
	} else {
		toggleEditable(TRUE);
	}

	// Update widgets to reflect monster
	// Basic window
	gtk_entry_set_text(GTK_ENTRY(e_name), gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(obj)));
	gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_type1), monFile.mons[currMon].basic.type1);
	gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_type2), monFile.mons[currMon].basic.type2);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_height), ((double)monFile.mons[currMon].basic.height)*0.01);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_weight), ((double)monFile.mons[currMon].basic.weight)*0.01);
	// Stat window
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_hp), monFile.mons[currMon].stats.b_hp);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_att), monFile.mons[currMon].stats.b_att);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_def), monFile.mons[currMon].stats.b_def);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_spatt), monFile.mons[currMon].stats.b_spAtt);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_spdef), monFile.mons[currMon].stats.b_spDef);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_speed), monFile.mons[currMon].stats.b_speed);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_ehp), monFile.mons[currMon].stats.ev_hp);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_eatt), monFile.mons[currMon].stats.ev_att);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_edef), monFile.mons[currMon].stats.ev_def);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_espatt), monFile.mons[currMon].stats.ev_spAtt);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_espdef), monFile.mons[currMon].stats.ev_spDef);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_espeed), monFile.mons[currMon].stats.ev_speed);
	// Evolution window
	if(monFile.mons[currMon].evolution.evolves == 1) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_evolves), TRUE);
	} else {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_evolves), TRUE);
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_evomon), monFile.mons[currMon].evolution.mon);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_evolevel), monFile.mons[currMon].evolution.level);
	// Aesthetics window
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_battlesprite), monFile.mons[currMon].aesthetics.battleSprite);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_cry), monFile.mons[currMon].aesthetics.cry);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_icon), monFile.mons[currMon].aesthetics.icon);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_bodystyle), monFile.mons[currMon].aesthetics.body);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_footprint), monFile.mons[currMon].aesthetics.footprint);
	// Experience window
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_erratic), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_fast), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_medfast), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_medslow), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_slow), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_fluctuating), FALSE);
	switch(monFile.mons[currMon].experience.type) {
	case EXP_ERRATIC:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_erratic), TRUE);
		break;
	case EXP_FAST:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_fast), TRUE);
		break;
	case EXP_MEDFAST:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_medfast), TRUE);
		break;
	case EXP_MEDSLOW:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_medslow), TRUE);
		break;
	case EXP_SLOW:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_slow), TRUE);
		break;
	case EXP_FLUCTUATING:
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(rb_fluctuating), TRUE);
		break;
	}
	// Others window
	gtk_entry_set_text(GTK_ENTRY(e_species), monFile.mons[currMon].etc.species);
	if(monFile.mons[currMon].etc.genderless == 1) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_nogender), TRUE);
	} else {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_nogender), FALSE);
	}
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_genderratio), monFile.mons[currMon].etc.malePerc);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_ability), monFile.mons[currMon].etc.ability);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_catchrate), monFile.mons[currMon].etc.catchRate);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_helditem), monFile.mons[currMon].etc.heldItem);
}

void b_update_clicked(GtkWidget *obj, gpointer user_data) {

	if(currMon == -1) {
		return;
	}

	// GTK likes to reset dropdowns when they are changed
	int tmpCurrMon = currMon;

	char *name = (char*)gtk_entry_get_text(GTK_ENTRY(e_name));

	bool fill = false;
	for(int i=0; i<10; i++) {
		if(!(i < gtk_entry_get_text_length(GTK_ENTRY(e_name)))) {
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

void b_moves_save_clicked(GtkWidget *obj, gpointer user_data) {

	// Script parsing stuff
}

void b_others_save_clicked(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	const char *species = gtk_entry_get_text(GTK_ENTRY(e_species));

	bool fill = false;
	for(int i=0; i<13; i++) {
		if(!i < gtk_entry_get_text_length(GTK_ENTRY(e_species))) {
			fill = true;
		}
		if(fill) {
			monFile.mons[currMon].etc.species[i] = ' ';
		} else {
			monFile.mons[currMon].etc.species[i] = species[i];
		}
	}
}

/* --- Simple per-monster events --- */
void cbt_type1_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	// Set the current monster's first type to the comobo box value
	monFile.mons[currMon].basic.type1 = gtk_combo_box_get_active(GTK_COMBO_BOX(obj));
}

void cbt_type2_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	// Set the current monster's second type to the combo box value
	monFile.mons[currMon].basic.type2 = gtk_combo_box_get_active(GTK_COMBO_BOX(obj));
}

void sb_battlesprite_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].aesthetics.battleSprite =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_cry_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}
	
	monFile.mons[currMon].aesthetics.cry =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_icon_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].aesthetics.icon =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_footprint_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].aesthetics.footprint =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_bodystyle_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].aesthetics.body =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_evolevel_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].evolution.level =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_height_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	// Shift up the decimals and store as an int for easier file management
	monFile.mons[currMon].basic.height =
		(int)(gtk_spin_button_get_value(GTK_SPIN_BUTTON(obj))*100);
}

void sb_weight_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	// Shift up the decimals and store as an int for easier file management
	monFile.mons[currMon].basic.weight =
		(int)(gtk_spin_button_get_value(GTK_SPIN_BUTTON(obj))*100);
}

void sb_genderratio_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].etc.malePerc =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_ability_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].etc.ability =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_catchrate_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].etc.catchRate =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_helditem_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].etc.heldItem =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void cbt_evomon_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].evolution.mon =
		gtk_combo_box_get_active(GTK_COMBO_BOX(obj));
}

void sb_hp_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].stats.b_hp =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_att_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].stats.b_att =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_def_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].stats.b_def =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_spatt_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].stats.b_spAtt =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_spdef_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].stats.b_spDef =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_speed_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].stats.b_speed =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_ehp_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].stats.ev_hp =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_eatt_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].stats.ev_att =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_edef_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].stats.ev_def =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_espatt_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].stats.ev_spAtt =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_espdef_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].stats.ev_spDef =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

void sb_espeed_changed(GtkWidget *obj, gpointer user_data) {
	if(currMon == -1) {
		return;
	}

	monFile.mons[currMon].stats.ev_speed =
		gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));
}

/* -- Radio Buttons -- */
void rb_erratic_toggled(GtkWidget *obj, gpointer user_data) {
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obj))) {
		monFile.mons[currMon].experience.type = EXP_ERRATIC;
	}
}

void rb_fast_toggled(GtkWidget *obj, gpointer user_data) {
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obj))) {
		monFile.mons[currMon].experience.type = EXP_FAST;
	}
}

void rb_medfast_toggled(GtkWidget *obj, gpointer user_data) {
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obj))) {
		monFile.mons[currMon].experience.type = EXP_MEDFAST;
	}
}

void rb_medslow_toggled(GtkWidget *obj, gpointer user_data) {
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obj))) {
		monFile.mons[currMon].experience.type = EXP_MEDSLOW;
	}
}

void rb_slow_toggled(GtkWidget *obj, gpointer user_data) {
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obj))) {
		monFile.mons[currMon].experience.type = EXP_SLOW;
	}
}

void rb_fluctuating_toggled(GtkWidget *obj, gpointer user_data) {
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obj))) {
		monFile.mons[currMon].experience.type = EXP_FLUCTUATING;
	}
}

/* -- RESOURCE LOAD/SAVE FUNCTIONS -- */

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

	b_update =				GTK_WIDGET (gtk_builder_get_object(builder, "b_update"));
	b_editmoves =			GTK_WIDGET (gtk_builder_get_object(builder, "b_editmovelist"));
	b_editstats =			GTK_WIDGET (gtk_builder_get_object(builder, "b_editstats"));
	b_editexp =				GTK_WIDGET (gtk_builder_get_object(builder, "b_editexp"));
	b_editaesthetics =		GTK_WIDGET (gtk_builder_get_object(builder, "b_editaesthetics"));
	b_editothers =			GTK_WIDGET (gtk_builder_get_object(builder, "b_editothers"));
	b_editevolutions =		GTK_WIDGET (gtk_builder_get_object(builder, "b_editevolutions"));

	sb_height =				GTK_WIDGET (gtk_builder_get_object(builder, "sb_height"));
	sb_weight = 			GTK_WIDGET (gtk_builder_get_object(builder, "sb_weight"));

	cbt_monsters =			GTK_WIDGET (gtk_builder_get_object(builder, "cbt_monsters"));
	cbt_evomon = 			GTK_WIDGET (gtk_builder_get_object(builder, "cbt_evomon"));
	cbt_type1 = 			GTK_WIDGET (gtk_builder_get_object(builder, "cbt_type1"));
	cbt_type2 = 			GTK_WIDGET (gtk_builder_get_object(builder, "cbt_type2"));

	e_species =				GTK_WIDGET (gtk_builder_get_object(builder, "e_species"));
	tv_nmoves =				GTK_WIDGET (gtk_builder_get_object(builder, "tv_nmoves"));
	tv_tmmoves = 			GTK_WIDGET (gtk_builder_get_object(builder, "tv_tmmoves"));

	rb_erratic =			GTK_WIDGET (gtk_builder_get_object(builder, "rb_erratic"));
	rb_fast =				GTK_WIDGET (gtk_builder_get_object(builder, "rb_fast"));
	rb_medfast =			GTK_WIDGET (gtk_builder_get_object(builder, "rb_medfast"));
	rb_medslow = 			GTK_WIDGET (gtk_builder_get_object(builder, "rb_medslow"));
	rb_slow = 				GTK_WIDGET (gtk_builder_get_object(builder, "rb_slow"));
	rb_fluctuating =		GTK_WIDGET (gtk_builder_get_object(builder, "rb_fluctuating"));

	sb_ability =			GTK_WIDGET (gtk_builder_get_object(builder, "sb_ability"));
	sb_genderratio =		GTK_WIDGET (gtk_builder_get_object(builder, "sb_genderratio"));
	sb_catchrate =			GTK_WIDGET (gtk_builder_get_object(builder, "sb_catchrate"));
	sb_helditem = 			GTK_WIDGET (gtk_builder_get_object(builder, "sb_helditem"));
	sb_battlesprite =		GTK_WIDGET (gtk_builder_get_object(builder, "sb_battlesprite"));
	sb_cry = 				GTK_WIDGET (gtk_builder_get_object(builder, "sb_cry"));
	sb_footprint =			GTK_WIDGET (gtk_builder_get_object(builder, "sb_footprint"));
	sb_bodystyle =			GTK_WIDGET (gtk_builder_get_object(builder, "sb_bodystyle"));
	sb_icon = 				GTK_WIDGET (gtk_builder_get_object(builder, "sb_icon"));
	sb_hp = 				GTK_WIDGET (gtk_builder_get_object(builder, "sb_hp"));
	sb_att = 				GTK_WIDGET (gtk_builder_get_object(builder, "sb_att"));
	sb_def = 				GTK_WIDGET (gtk_builder_get_object(builder, "sb_def"));
	sb_spatt = 				GTK_WIDGET (gtk_builder_get_object(builder, "sb_spatt"));
	sb_spdef = 				GTK_WIDGET (gtk_builder_get_object(builder, "sb_spdef"));
	sb_speed = 				GTK_WIDGET (gtk_builder_get_object(builder, "sb_speed"));
	sb_ehp = 				GTK_WIDGET (gtk_builder_get_object(builder, "sb_ehp"));
	sb_eatt = 				GTK_WIDGET (gtk_builder_get_object(builder, "sb_eatt"));
	sb_edef = 				GTK_WIDGET (gtk_builder_get_object(builder, "sb_edef"));
	sb_espatt = 			GTK_WIDGET (gtk_builder_get_object(builder, "sb_espatt"));
	sb_espdef = 			GTK_WIDGET (gtk_builder_get_object(builder, "sb_espdef"));
	sb_espeed = 			GTK_WIDGET (gtk_builder_get_object(builder, "sb_espeed"));

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));

	// Load external resources
	loadTypes("../resources/data/types.pke");
	loadMoves("../resources/data/moves.pke");

	toggleEditable(FALSE);

	gtk_widget_show(windows[W_MAIN]);
	gtk_main();

	pk_freeMonFile(&monFile);


	return 0;
}

