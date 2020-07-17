#ifndef DATAGUI_H
#define DATAGUI_H

#include "parser.h"
#include "engine.h"

/* setup data driven behavior as specified in Data GUI */
/* file using widgets from given Glade structure */

DgXML *dg_xml_new(gchar *filename, GladeXML *xml, GtkDialog *error_dialog, GtkLabel *error_label);

/* call this method whenever the connection state changes */

void dg_connection(DgXML *dg, GdaConnection *cnc);

#endif // DATAGUI_H
