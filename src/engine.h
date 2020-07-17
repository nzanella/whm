#ifndef ENGINE_H
#define ENGINE_H

#include <gtk/gtk.h>
#include <libgda/libgda.h>
#include <glib/gprintf.h>
#include "parser.h"

void dg_callback_row_selected(GtkCList *clist, gint i, gint j, GdkEventButton *event, gpointer data);

void dg_callback_a_clicked(GtkToggleButton *togglebutton, gpointer data);

void dg_callback_m_clicked(GtkToggleButton *togglebutton, gpointer data);

void dg_callback_r_clicked(GtkToggleButton *togglebutton, gpointer data);

void dg_callback_s_clicked(GtkToggleButton *togglebutton, gpointer data);

void dg_callback_c_clicked(GtkToggleButton *togglebutton, gpointer data);

void dg_callback_messenger_clicked(GtkButton *messenger, gpointer data);

#endif // ENGINE_H
