/* define callbacks and auxiliary */
/* data structures and functions */

#include "callbacks.h"
#include "parser.h"
#include "datagui.h"

extern GladeXML *xml;
extern DgXML *dg;

static GtkNotebook *whm_notebook1;
static GtkNotebook *whm_purchaseditems_notebook1;
static GtkNotebook *whm_solditems_notebook1;

/* use this flag as a signal guard */

static gboolean not_navigating_notebook = TRUE;

/* construct data structure used to represent notebook hierarchy */

#define NUM_TOP_NOTEBOOK_PAGES 13
#define PIT_NOTEBOOK_INDEX 6
#define NUM_PIT_NOTEBOOK_PAGES 3
#define SIT_NOTEBOOK_INDEX 10
#define NUM_SIT_NOTEBOOK_PAGES 3

void notebook_tree_construct(void) {

  /* retrieve notebooks */

  whm_notebook1 = GTK_NOTEBOOK(glade_xml_get_widget(xml, "whm_notebook1"));
  whm_purchaseditems_notebook1 = GTK_NOTEBOOK(glade_xml_get_widget(xml, "whm_purchaseditems_notebook1"));
  whm_solditems_notebook1 = GTK_NOTEBOOK(glade_xml_get_widget(xml, "whm_solditems_notebook1"));

  /* construct structure representing main notebook */

  notebook_tree_current = g_new0(NotebookTree, 1);

  notebook_tree_current->notebook = whm_notebook1;

  notebook_tree_current->children = g_new0(NotebookTree *, NUM_TOP_NOTEBOOK_PAGES);

  /* construct structure representing purchased items notebook */

  notebook_tree_current->children[PIT_NOTEBOOK_INDEX] = g_new0(NotebookTree, 1);

  notebook_tree_current->children[PIT_NOTEBOOK_INDEX]->notebook = whm_purchaseditems_notebook1;

  notebook_tree_current->children[PIT_NOTEBOOK_INDEX]->parent = notebook_tree_current;

  notebook_tree_current->children[PIT_NOTEBOOK_INDEX]->children = g_new0(NotebookTree *, NUM_PIT_NOTEBOOK_PAGES);

  /* construct structure representing sold items notebook */

  notebook_tree_current->children[SIT_NOTEBOOK_INDEX] = g_new0(NotebookTree, 1);

  notebook_tree_current->children[SIT_NOTEBOOK_INDEX]->notebook = whm_solditems_notebook1;

  notebook_tree_current->children[SIT_NOTEBOOK_INDEX]->parent = notebook_tree_current;

  notebook_tree_current->children[SIT_NOTEBOOK_INDEX]->children = g_new0(NotebookTree *, NUM_SIT_NOTEBOOK_PAGES);

}

/* set current page to previous page in notebook hierarchy */

void notebook_tree_previous_page(void) {

  not_navigating_notebook = FALSE;

  while (gtk_notebook_get_current_page(notebook_tree_current->notebook) == 0 && notebook_tree_current->parent)

    notebook_tree_current = notebook_tree_current->parent;

  gtk_notebook_set_current_page(notebook_tree_current->notebook,
    (gtk_notebook_get_current_page(notebook_tree_current->notebook)
    + gtk_notebook_get_n_pages(notebook_tree_current->notebook) - 1)
    % gtk_notebook_get_n_pages(notebook_tree_current->notebook));

  while (notebook_tree_current->children[gtk_notebook_get_current_page(notebook_tree_current->notebook)]) {
 
    notebook_tree_current = notebook_tree_current->children[gtk_notebook_get_current_page(notebook_tree_current->notebook)];

    gtk_notebook_set_current_page(notebook_tree_current->notebook,
      gtk_notebook_get_n_pages(notebook_tree_current->notebook) - 1);

  }

  not_navigating_notebook = TRUE;

}

/* set current page to next page in notebook hierarchy */

void notebook_tree_next_page(void) {

  not_navigating_notebook = FALSE;

  while (gtk_notebook_get_current_page(notebook_tree_current->notebook)
    == gtk_notebook_get_n_pages(notebook_tree_current->notebook) - 1 && notebook_tree_current->parent)

    notebook_tree_current = notebook_tree_current->parent;

  gtk_notebook_set_current_page(notebook_tree_current->notebook,
    (gtk_notebook_get_current_page(notebook_tree_current->notebook) + 1)
    % gtk_notebook_get_n_pages(notebook_tree_current->notebook));

  while (notebook_tree_current->children[gtk_notebook_get_current_page(notebook_tree_current->notebook)]) {

    notebook_tree_current = notebook_tree_current->children[gtk_notebook_get_current_page(notebook_tree_current->notebook)];

    gtk_notebook_set_current_page(notebook_tree_current->notebook, 0);

  }

  not_navigating_notebook = TRUE;

}

/* quit application from menu item */

void menuitem_callback_quit(GtkMenuItem *menuitem, gpointer data) {

  gtk_main_quit();

}

/* select notebook pages from menu items */

void menuitem_callback_page_selection(GtkMenuItem *menuitem, gpointer data) {

  PageNumbers *page_numbers = (PageNumbers *) data;

  if (page_numbers->main >= 0)

    gtk_notebook_set_current_page(whm_notebook1, page_numbers->main);

  if (page_numbers->pit >= 0)

    gtk_notebook_set_current_page(whm_purchaseditems_notebook1, page_numbers->pit);

  if (page_numbers->sit >= 0)

    gtk_notebook_set_current_page(whm_solditems_notebook1, page_numbers->sit);

}

/* set current notebook page to previous notebook page in notebook hierarchy */

void menuitem_callback_previous(GtkMenuItem *item, gpointer data) {

  notebook_tree_previous_page();

}

/* set current notebook page to next notebook page in notebook hierarchy */

void menuitem_callback_next(GtkMenuItem *button, gpointer data) {

  notebook_tree_next_page();

}

/* set current page to previous page in notebook hierarchy */

void toolbutton_callback_previous(GtkToolButton *button, gpointer data) {

  notebook_tree_previous_page();

}

/* set current page to next page in notebook hierarchy */

void toolbutton_callback_next(GtkToolButton *button, gpointer data) {

  notebook_tree_next_page();

}

/* update current notebook tree node when a notebook page is clicked */

void notebook_tree_callback_set_current(GtkNotebook *notebook,
  GtkNotebookPage *page, guint page_num, gpointer data) {

  // first ensure that the user really clicked on a page
  // rather than on a navigation toolbutton or menu item

  if (not_navigating_notebook) {

    // check whether user clicked on an ancestor notebook tree

    while (notebook_tree_current->notebook != notebook && notebook_tree_current->parent) {

      notebook_tree_current = notebook_tree_current->parent;

    }

    // set current notebook tree to lowest notebook on descendent notebook tree

    while (notebook_tree_current->children[page_num]) {

      notebook_tree_current = notebook_tree_current->children[page_num];

      page_num = gtk_notebook_get_current_page(notebook_tree_current->notebook);

    }

  }

}

/* present window on receipt of internal signal */

void window_callback_present(GtkObject *object, gpointer data) {

  gtk_window_present(GTK_WINDOW(data));

}

/* hide widget on receipt of internal signal */

void widget_callback_hide(GtkObject *object, gpointer data) {

  gtk_widget_hide(GTK_WIDGET(data));

}

/* hide widget on receipt of external event */

gboolean widget_callback_hide_on_event(GtkWidget *widget, GdkEvent event, gpointer data) {

  gtk_widget_hide(widget);

  /* do not allow event to propagate any further */

  return TRUE;

}

/* connect to specified data source */

#define PORT_LEN_MAX 5
#define PORT_PREFIX_LEN 5

void database_callback_connect(GtkButton *button, gpointer data) {

  GdaConnection *cnc = NULL;
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *cnc_string, *provider, *host, *port;
  gchar *database, *username, *password;

  /* retrieve provider */

  gtk_combo_box_get_active_iter(conn_provider_combobox1, &iter);
  model = gtk_combo_box_get_model(conn_provider_combobox1);
  gtk_tree_model_get(model, &iter, 0, &provider, -1);

  /* construct connection string omitting host and port for Unix when invalid */

  database = g_strconcat("DATABASE=", gtk_entry_get_text(conn_database_entry1), NULL);
  username = g_strconcat("USER=", gtk_entry_get_text(conn_username_entry1), NULL);
  password = g_strconcat("PASSWORD=", gtk_entry_get_text(conn_password_entry1), NULL);

  cnc_string = g_strjoin(";", database, username, password, NULL);

  if (g_ascii_strcasecmp(host = (gchar *) gtk_entry_get_text(conn_host_entry1), "")) {
    host = g_strconcat("HOST=", host, NULL);
    cnc_string = g_strjoin(";", cnc_string, host, NULL);
  }

  if (gtk_spin_button_get_value_as_int(conn_port_spinbutton1) != 0) {
    gchar port_string[PORT_LEN_MAX + PORT_PREFIX_LEN + 1];
    sprintf(port_string, "%d", gtk_spin_button_get_value_as_int(conn_port_spinbutton1));
    port = g_strconcat("PORT=", port_string, NULL);
    cnc_string = g_strjoin(";", cnc_string, port, NULL);
  }

  /* close old connection if it exists */

  if (GDA_IS_CONNECTION(cnc)) {

    gda_client_close_all_connections(client);

    cnc = NULL;

  }

  if (GDA_IS_CONNECTION(cnc = gda_client_open_connection_from_string(client,
    provider, cnc_string, (GdaConnectionOptions) 0))) {

    if (1) {//check whether is we want password to be saved

      gda_config_save_data_source("warehouse", provider, cnc_string, "warehouse", username, password);

    }

    gtk_widget_show_all(GTK_WIDGET(connc_dialog1));

  } else

    gtk_widget_show_all(GTK_WIDGET(nconnc_dialog1));

  /* reset data gui connection state */

  dg_connection(dg, cnc);

}
