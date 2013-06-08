#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gtk/gtk.h>

#define INV_YES		1
#define INV_NO		0

int typeCnt = 0;
int currType = -1;
int typeSlot = 0;

char typeList[20][9];

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

typedef struct {
	int version;
	int count;
} typeHeader_f;

typedef struct {
	char name[8];
	int resCnt;
	int weakCnt;
} typeEntry_f;

typedef struct {
	int type;
	char inv;
} resEntry_f;

typedef struct {
	int type;
} weakEntry_f;

typedef struct {
	typeEntry_f info;
	resEntry_f res[20];
	weakEntry_f weak[20];
} typeObj_t;

typeObj_t types[50];

void report(int numb)
{
	printf("Type #%i\n", numb);
	printf("  Weaknesses:\n");
	for(int i=0; i<types[numb].info.weakCnt; i++) {
		printf("    %i: %i\n", i, types[numb].weak[i].type);
	}
	printf("  Resistances:\n");
	for(int i=0; i<types[numb].info.resCnt; i++) {
		printf("    %i: %i\n", i, types[numb].res[i].type);
	}
}

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

void clearTypes()
{
	for(int i=0; i<20; i++) {
		for(int j=0; j<8; j++) {
			typeList[i][j] = ' ';
		}
	}
}

void newType(char* name)
{
	bool fill = false;
	for(int i=0; i<8; i++) {
		if(name[i] == ' ') {
			fill = true;
		}
		if(fill) {
			typeList[typeCnt][i] = ' ';
		} else {
			typeList[typeCnt][i] = name[i];
		}
	}
	typeCnt++;
}

void editType(char* name, int val)
{
	bool fill = false;
	for(int i=0; i<8; i++) {
		if(name[i] == ' ') {
			fill = true;
		}
		if(fill) {
			typeList[val][i] = ' ';
		} else {
			typeList[val][i] = name[i];
		}
	}
}

void updateTypes()
{
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(cbt_type));
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(cbt_weaknesses));
	gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(cbt_resistances));
	for(int i=0; i<typeCnt; i++) {
		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(cbt_type), currType,
			typeList[i]);
		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(cbt_weaknesses), currType,
			typeList[i]);
		gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(cbt_resistances), currType,
			typeList[i]);
	}
}

void on_window_destroy (GtkWidget *object, gpointer user_data)
{
	gtk_main_quit();
}

void cb_immunity_changed(GtkWidget *obj, gpointer user_data) {
	int currRes = (int)gtk_range_get_value(GTK_RANGE(s_resistances))-1;
	if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(obj)) == FALSE) {
		types[currType].res[currRes].inv = INV_NO;
	} else {
		types[currType].res[currRes].inv = INV_YES;
	}
}

void sb_weaknesses_changed(GtkWidget *obj, gpointer user_data)
{
	if(currType == -1) {
		return;
	}

	int val = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));

	types[currType].info.weakCnt = val;

	if(val >= 1) {
		gtk_range_set_range(GTK_RANGE(s_weaknesses), 1, val);
		gtk_widget_set_sensitive(GTK_WIDGET(s_weaknesses), TRUE);
	} else {
		gtk_range_set_range(GTK_RANGE(s_weaknesses), 0, 0);
		gtk_widget_set_sensitive(GTK_WIDGET(s_weaknesses), FALSE);
	}
}

void sb_resistances_changed(GtkWidget *obj, gpointer user_data) {
	if(currType == -1) {
		return;
	}

	int val = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(obj));

	types[currType].info.resCnt = val;

	if(val >= 1) {
		gtk_range_set_range(GTK_RANGE(s_resistances), 1, val);
		gtk_widget_set_sensitive(GTK_WIDGET(s_resistances), TRUE);
	} else {
		gtk_range_set_range(GTK_RANGE(s_resistances), 0, 0);
		gtk_widget_set_sensitive(GTK_WIDGET(s_resistances), FALSE);
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
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_weaknesses), (gdouble)types[currType].info.weakCnt);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_resistances), (gdouble)types[currType].info.resCnt);
}

void cbt_weaknesses_changed(GtkWidget *obj, gpointer user_data)
{
	if(gtk_combo_box_get_active(GTK_COMBO_BOX(obj)) == -1) {
		return;
	}
	types[currType].weak[(int)gtk_range_get_value(GTK_RANGE(s_weaknesses))-1].type =
		(int)gtk_combo_box_get_active(GTK_COMBO_BOX(obj));
}

void cbt_resistances_changed(GtkWidget *obj, gpointer user_data)
{
	if(gtk_combo_box_get_active(GTK_COMBO_BOX(obj)) == -1) {
		return;
	}
	types[currType].res[(int)gtk_range_get_value(GTK_RANGE(s_resistances))-1].type =
		(int)gtk_combo_box_get_active(GTK_COMBO_BOX(obj));
}

void s_weaknesses_changed(GtkWidget *obj, gpointer user_data)
{
	gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_weaknesses), types[currType].weak[(int)gtk_range_get_value(GTK_RANGE(s_weaknesses))-1].type);
}

void s_resistances_changed(GtkWidget *obj, gpointer user_data)
{
	gtk_combo_box_set_active(GTK_COMBO_BOX(cbt_resistances), types[currType].res[(int)gtk_range_get_value(GTK_RANGE(s_resistances))-1].type);
	if(types[currType].res[(int)gtk_range_get_value(GTK_RANGE(obj))-1].inv == INV_YES) {
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
	report(currType);
}

void b_save_clicked(GtkWidget *obj, gpointer user_data)
{
	printf("Saving...");
	FILE * file;
	file = fopen("types.pke", "w");

	typeHeader_f header;
	header.version = 2;
	header.count = typeCnt;

	fwrite(&header.version, sizeof(int), 1, file);
	fwrite(&header.count, sizeof(int), 1, file);

	typeEntry_f typeHeader[typeCnt];
	for(int i=0; i<typeCnt; i++) {
		char *name = (char*)gtk_entry_get_text(GTK_ENTRY(e_typename));
		bool fill = false;
		for(int j=0; j<8; j++) {
			if(name[j] == ' ') {
				fill = true;
			}
			if(fill) {
				typeHeader[i].name[j] = ' ';
			} else {
				typeHeader[i].name[j] = name[j];
			}
		}
		typeHeader[i].resCnt = types[i].info.resCnt;
		typeHeader[i].weakCnt = types[i].info.weakCnt;
		fwrite(&typeHeader[i].resCnt, sizeof(int), 1, file);
		fwrite(&typeHeader[i].weakCnt, sizeof(int), 1, file);

		for(int j=0; j<typeHeader[i].resCnt; j++) {
			fwrite(&types[i].res[j].type, sizeof(int), 1, file);
			fwrite(&types[i].res[j].inv, sizeof(char), 1, file);
		}
		for(int j=0; j<typeHeader[i].weakCnt; j++) {
			fwrite(&types[i].weak[j].type, sizeof(int), 1, file);
		}
	}

	fclose(file);

	printf("done!\n\n");
}

int main(int argc, char **argv) {

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

	newType("NEW");
	updateTypes();

	toggleEditable(FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(s_resistances), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(s_weaknesses), FALSE);

	gtk_widget_show(window);
	gtk_main();

	return 0;
}
