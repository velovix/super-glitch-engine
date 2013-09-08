#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>

#include "../common/typeFile.h"

#define INV_YES		1
#define INV_NO		0

#define CURRVERSION		2

int currType = -1;
int typeSlot = 0;

GtkWidget *cbt_type;
GtkWidget *cbt_resistances;
GtkWidget *cbt_weaknesses;
GtkWidget *e_typename;
GtkWidget *sb_weaknesses;
GtkWidget *sb_resistances;
GtkWidget *s_resistances;
GtkWidget *s_weaknesses;
GtkWidget *cb_immunity;
GtkWidget *b_update;

typeFile_t typeFile;

void toggleEditable(gboolean editable)
{
	gtk_widget_set_sensitive(GTK_WIDGET(e_typename), editable);
	gtk_widget_set_sensitive(GTK_WIDGET(b_update), editable);
	gtk_widget_set_sensitive(GTK_WIDGET(sb_weaknesses), editable);
	gtk_widget_set_sensitive(GTK_WIDGET(sb_resistances), editable);
	gtk_widget_set_sensitive(GTK_WIDGET(cbt_weaknesses), editable);
	gtk_widget_set_sensitive(GTK_WIDGET(cbt_resistances), editable);
	gtk_widget_set_sensitive(GTK_WIDGET(cb_immunity), editable);
}

void newTypeFile()
{
	typeFile.header.version = TYPEFILE_VERSION;
	typeFile.header.count = 0;

	typeFile.types = NULL;
}

void clearTypes()
{
	currType = -1;
	for(int i=0; i<typeFile.header.count; i++) {
		for(int j=0; j<8; j++) {
			typeFile.types[i].header.name[j] = ' ';
		}
	}

	typeFile.header.count = 0;
}

void newType(char* name)
{
	// Allocate more type data
	typeFile.types =
		(typeFileObj_t*)realloc(typeFile.types, sizeof(typeFileObj_t)*(typeFile.header.count+1));
	typeFile.types[typeFile.header.count].val = typeFile.header.count;
	typeFile.types[typeFile.header.count].header.resCnt = 0;
	typeFile.types[typeFile.header.count].header.weakCnt = 0;

	typeFile.types[typeFile.header.count].res = NULL;
	typeFile.types[typeFile.header.count].weak = NULL;

	bool fill = false;
	for(int i=0; i<8; i++) {
		if(name[i] == ' ') {
			fill = true;
		}
		if(fill) {
			typeFile.types[typeFile.header.count].header.name[i] = ' ';
		} else {
			typeFile.types[typeFile.header.count].header.name[i] = name[i];
		}
	}
	typeFile.header.count++;
}

void editType(char* name, int val)
{
	bool fill = false;
	for(int i=0; i<8; i++) {
		if(name[i] == ' ') {
			fill = true;
		}
		if(fill) {
			typeFile.types[val].header.name[i] = ' ';
		} else {
			typeFile.types[val].header.name[i] = name[i];
		}
	}
}

void updateTypes()
{
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(cbt_type));
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(cbt_weaknesses));
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(cbt_resistances));
	for(int i=0; i<typeFile.header.count; i++) {
		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(cbt_type), currType,
			(char*)typeFile.types[i].header.name);
		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(cbt_weaknesses), currType,
			(char*)typeFile.types[i].header.name);
		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(cbt_resistances), currType,
			(char*)typeFile.types[i].header.name);
	}
}

void on_window_destroy (GtkWidget *object, gpointer user_data)
{
	gtk_main_quit();
}

void cb_immunity_changed(GtkWidget *obj, gpointer user_data) {
	int currRes = (int)gtk_range_get_value(GTK_RANGE(s_resistances))-1;
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obj)) == FALSE) {
		typeFile.types[currType].res[currRes].inv = INV_NO;
	} else {
		typeFile.types[currType].res[currRes].inv = INV_YES;
	}
}

void sb_weaknesses_changed(GtkWidget *obj, gpointer user_data)
{
	if(currType == -1) {
		return;
	}

	int val = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));

	typeFile.types[currType].header.weakCnt = val;
	// Reallocate weakness data accordingly
	typeFile.types[currType].weak = realloc(typeFile.types[currType].weak,
		sizeof(weakEntry_f)*val);

	if(val >= 1) {
		gtk_range_set_range(GTK_RANGE(s_weaknesses), 1, val);
		gtk_widget_set_sensitive(GTK_WIDGET(s_weaknesses), TRUE);
		gtk_widget_set_sensitive(cbt_weaknesses, TRUE);
	} else {
		gtk_range_set_range(GTK_RANGE(s_weaknesses), 0, 0);
		gtk_widget_set_sensitive(GTK_WIDGET(s_weaknesses), FALSE);
		gtk_widget_set_sensitive(cbt_weaknesses, FALSE);
	}
}

void sb_resistances_changed(GtkWidget *obj, gpointer user_data) {
	if(currType == -1) {
		return;
	}

	int val = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));

	typeFile.types[currType].header.resCnt = val;
	// Reallocate resistance data accordingly
	typeFile.types[currType].res = realloc(typeFile.types[currType].res,
		sizeof(resEntry_f)*val);

	if(val >= 1) {
		gtk_range_set_range(GTK_RANGE(s_resistances), 1, val);
		gtk_widget_set_sensitive(GTK_WIDGET(s_resistances), TRUE);
		gtk_widget_set_sensitive(cbt_resistances, TRUE);
	} else {
		gtk_range_set_range(GTK_RANGE(s_resistances), 0, 0);
		gtk_widget_set_sensitive(GTK_WIDGET(s_resistances), FALSE);
		gtk_widget_set_sensitive(cbt_resistances, FALSE);
	}
}

void cbt_type_changed(GtkWidget *obj, gpointer user_data)
{
	currType = gtk_combo_box_get_active(GTK_COMBO_BOX(obj));
	if(currType == -1) {
		toggleEditable(FALSE);
		return;
	} else {
		toggleEditable(TRUE);
	}


	gtk_entry_set_text(GTK_ENTRY(e_typename), gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(obj)));
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_weaknesses), (gdouble)typeFile.types[currType].header.weakCnt);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_resistances), (gdouble)typeFile.types[currType].header.resCnt);

	if(typeFile.types[currType].header.weakCnt > 0) {
		gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_weaknesses),
			typeFile.types[currType].weak[(int)gtk_range_get_value(GTK_RANGE(s_weaknesses))-1].type);
	}

	if(typeFile.types[currType].header.resCnt > 0) {
		gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_resistances),
			typeFile.types[currType].res[(int)gtk_range_get_value(GTK_RANGE(s_resistances))-1].type);
	}
}

void cbt_weaknesses_changed(GtkWidget *obj, gpointer user_data)
{
	if(gtk_combo_box_get_active(GTK_COMBO_BOX(obj)) == -1) {
		return;
	}

	if(typeFile.types[currType].header.weakCnt > 0) {
		typeFile.types[currType].weak[(int)gtk_range_get_value(GTK_RANGE(s_weaknesses))-1].type =
			(int)gtk_combo_box_get_active(GTK_COMBO_BOX(obj));
	}
}

void cbt_resistances_changed(GtkWidget *obj, gpointer user_data)
{
	if(gtk_combo_box_get_active(GTK_COMBO_BOX(obj)) == -1) {
		return;
	}

	if(typeFile.types[currType].header.resCnt > 0) {
		typeFile.types[currType].res[(int)gtk_range_get_value(GTK_RANGE(s_resistances))-1].type =
			(int)gtk_combo_box_get_active(GTK_COMBO_BOX(obj));
	}
}

void s_weaknesses_changed(GtkWidget *obj, gpointer user_data)
{
	gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_weaknesses),
		typeFile.types[currType].weak[(int)gtk_range_get_value(GTK_RANGE(s_weaknesses))-1].type);
}

void s_resistances_changed(GtkWidget *obj, gpointer user_data)
{
	gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_resistances),
		typeFile.types[currType].res[(int)gtk_range_get_value(GTK_RANGE(s_resistances))-1].type);
	if(typeFile.types[currType].res[(int)gtk_range_get_value(GTK_RANGE(obj))-1].inv == INV_YES) {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_immunity), TRUE);
	} else {
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(cb_immunity), FALSE);
	}
}

void b_update_clicked(GtkWidget *obj, gpointer user_data)
{
	if(currType == -1) {
		return;
	}

	// Gtk sets currType to -1 when entries are removed
	int tmpCurrType = currType;
	int tmpWeakType = (int)gtk_range_get_value(GTK_RANGE(s_weaknesses));
	int tmpResType = (int)gtk_range_get_value(GTK_RANGE(s_resistances));

	editType((char*)gtk_entry_get_text(GTK_ENTRY(e_typename)), currType);
	updateTypes();

	gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_type), tmpCurrType);
	gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_weaknesses), tmpWeakType);
	gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_resistances), tmpResType);
}

void b_newtype_clicked(GtkWidget *obj, gpointer user_data)
{
	newType("NEW");
	updateTypes();
}

void b_load_clicked(GtkWidget *obj, gpointer user_data)
{
	pk_freeTypeFile(&typeFile);
	pk_openTypeFile(&typeFile, "../resources/data/types.pke");

	updateTypes();
}

void b_save_clicked(GtkWidget *obj, gpointer user_data)
{
	pk_saveTypeFile(&typeFile, "../resources/data/types.pke");
}

int main(int argc, char **argv) {

	newTypeFile();

	GtkBuilder *builder;
	GtkWidget *window;

	gtk_init (&argc, &argv);

	builder = gtk_builder_new ();
	gtk_builder_add_from_file (builder, "TypeMakerWind.glade", NULL);

	window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));

	cbt_type = GTK_WIDGET(gtk_builder_get_object(builder, "cbt_type"));
	cbt_weaknesses = GTK_WIDGET(gtk_builder_get_object(builder, "cbt_weaknesses"));
	cbt_resistances = GTK_WIDGET(gtk_builder_get_object(builder, "cbt_resistances"));
	e_typename = GTK_WIDGET(gtk_builder_get_object(builder, "e_typename"));
	sb_weaknesses = GTK_WIDGET(gtk_builder_get_object(builder, "sb_weaknesses"));
	sb_resistances = GTK_WIDGET(gtk_builder_get_object(builder, "sb_resistances"));
	s_weaknesses = GTK_WIDGET(gtk_builder_get_object(builder, "s_weaknesses"));
	s_resistances = GTK_WIDGET(gtk_builder_get_object(builder, "s_resistances"));
	cb_immunity = GTK_WIDGET(gtk_builder_get_object(builder, "cb_immunity"));
	b_update = GTK_WIDGET(gtk_builder_get_object(builder, "b_update"));

	gtk_range_set_range(GTK_RANGE(s_weaknesses), 0, 0);
	gtk_range_set_range(GTK_RANGE(s_resistances), 0, 0);
	

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(G_OBJECT(builder));

	//newType("NEW     ");
	updateTypes();

	toggleEditable(FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(s_resistances), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(s_weaknesses), FALSE);

	gtk_widget_show(window);
	gtk_main();

	return 0;
}
