#include "common.h"
#include "interface.h"
#include "datagui.h"

/* glade user interface structure */

GladeXML *xml;

/* data gui behavioral structure */

DgXML *dg;

/* handles to widgets which need be retained */
/* past disposal of user interface structure */

GdaClient *client;
GtkWindow *whm_window1;
GtkComboBox *conn_provider_combobox1;
GtkEntry *conn_host_entry1;
GtkSpinButton *conn_port_spinbutton1;
GtkEntry *conn_database_entry1;
GtkEntry *conn_username_entry1;
GtkEntry *conn_password_entry1;
GtkCheckButton *conn_rememberpassword_checkbutton1;
GtkDialog *nconnc_dialog1;
GtkDialog *connc_dialog1;

int main(int argc, char **argv) {

  /* initialize user interface engine */

  gtk_init(&argc, &argv);

  /* initialize database engine */

  gda_init(PACKAGE_NAME, PACKAGE_VERSION, argc, argv);

  /* retrieve user interface structure used to retrieve widgets */

  xml = glade_xml_new(GLADE_FILE, NULL, LOCALEDIR);

  /* setup behavioral error dialog */

  GtkDialog *error_dialog = GTK_DIALOG(glade_xml_get_widget(xml, "error_dialog1"));

  /* setup behavioral error label */

  GtkLabel *error_label = GTK_LABEL(glade_xml_get_widget(xml, "error_label1"));

  /* setup data driven user interface behavior */
  dg = dg_xml_new(DATAGUI_FILE, xml, error_dialog, error_label);

  /* setup user interface aspects unrelated to database drive behavior */

  interface_setup();

  /* dispose of user interface structure used to retrieve widgets */

  g_object_unref(G_OBJECT(xml));

  /* enter event loop */

  gtk_main();

  /* control does not reach here */

  return 0;

}
