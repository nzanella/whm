#ifndef COMMON_H
#define COMMON_H

#include "config.h"

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <libgda/libgda.h>
#include <glib/gprintf.h>

extern GladeXML *xml;

extern GdaClient *client;
extern GdaConnection *cnc;

extern GtkWindow *whm_window1;
extern GtkComboBox *conn_provider_combobox1;
extern GtkEntry *conn_host_entry1;
extern GtkSpinButton *conn_port_spinbutton1;
extern GtkEntry *conn_database_entry1;
extern GtkEntry *conn_username_entry1;
extern GtkEntry *conn_password_entry1;
extern GtkCheckButton *conn_rememberpassword_checkbutton1;
extern GtkDialog *nconnc_dialog1;
extern GtkDialog *connc_dialog1;
extern GtkDialog *error_dialog1;
extern GtkLabel *error_label1;

#endif // COMMON_H
