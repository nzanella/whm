/* associate actions with callbacks */

#include "interface.h"
#include "callbacks.h"

/* associate menu actions with menu callbacks */

static void menus_setup() {

  PageNumbers *page_numbers;

  /* connect database connection window menu item */

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_file_connect_item1")),
    "activate", G_CALLBACK(window_callback_present), glade_xml_get_widget(xml, "conn_window1"));

  /* connect quit menu item */

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_file_quit_item1")),
    "activate", G_CALLBACK(menuitem_callback_quit), NULL);

  /* connect company information window menu item */

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_edit_companyinfo_item1")),
    "activate", G_CALLBACK(window_callback_present), glade_xml_get_widget(xml, "cin_window1"));

  /* connect notebook navigation menu items */

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_previous_item1")),
    "activate", G_CALLBACK(menuitem_callback_previous), NULL);

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_next_item1")),
    "activate", G_CALLBACK(menuitem_callback_next), NULL);

  /* connect notebook page selection menu items */

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 0; page_numbers->pit = -1; page_numbers->sit = -1;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_inventory_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 1; page_numbers->pit = -1; page_numbers->sit = -1;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_suppliers_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 2; page_numbers->pit = -1; page_numbers->sit = -1;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_representatives_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 3; page_numbers->pit = -1; page_numbers->sit = -1;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_customers_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 4; page_numbers->pit = -1; page_numbers->sit = -1;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_itemcatalogue_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 5; page_numbers->pit = -1; page_numbers->sit = -1;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_purchases_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 6; page_numbers->pit = 0; page_numbers->sit = -1;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_purchaseditems_received_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 6; page_numbers->pit = 1; page_numbers->sit = -1;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_purchaseditems_returned_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 6; page_numbers->pit = 2; page_numbers->sit = -1;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_purchaseditems_refunded_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 7; page_numbers->pit = -1; page_numbers->sit = -1;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_containercategories_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 8; page_numbers->pit = -1; page_numbers->sit = -1;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_containers_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 9; page_numbers->pit = -1; page_numbers->sit = -1;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_sales_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 10; page_numbers->pit = -1; page_numbers->sit = 0;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_solditems_sent_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 10; page_numbers->pit = -1; page_numbers->sit = 1;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_solditems_returned_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 10; page_numbers->pit = -1; page_numbers->sit = 2;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_solditems_refunded_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 11; page_numbers->pit = -1; page_numbers->sit = -1;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_cashregister_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

  page_numbers = g_new(PageNumbers, 1);
  page_numbers->main = 12; page_numbers->pit = -1; page_numbers->sit = -1;

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_go_history_item1")),
    "activate", G_CALLBACK(menuitem_callback_page_selection), page_numbers);

}

/* associate toolbutton actions with toolbutton callbacks */

static void toolbuttons_setup() {

  /* construct notebook hierarchy data structure */

  notebook_tree_construct();

  /* connect notebook navigation toolbuttons */

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_previous_toolbutton1")),
    "clicked", G_CALLBACK(toolbutton_callback_previous), NULL);

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_next_toolbutton1")),
    "clicked", G_CALLBACK(toolbutton_callback_next), NULL);

  /* connect database connection toolbuttons */

  g_signal_connect(glade_xml_get_widget(xml, "whm_connect_toolbutton1"),
    "clicked", G_CALLBACK(window_callback_present), glade_xml_get_widget(xml, "conn_window1"));

}

/* associate notebook actions with notebook callbacks */

static void notebooks_setup() {

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_notebook1")),
  "switch-page", G_CALLBACK(notebook_tree_callback_set_current), NULL);

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_purchaseditems_notebook1")),
  "switch-page", G_CALLBACK(notebook_tree_callback_set_current), NULL);

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "whm_solditems_notebook1")),
  "switch-page", G_CALLBACK(notebook_tree_callback_set_current), NULL);

}

/* associate delete event on window with callback that hides window */

static void popup_window_setup(gpointer data, gpointer user_data) {

  g_signal_connect(GTK_WINDOW(glade_xml_get_widget(xml, (gchar *) data)),
    "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), NULL);

}

/* associate button click with callback to present popup window */

static void popup_button_setup(gpointer key, gpointer value, gpointer data) {

  g_signal_connect(glade_xml_get_widget(xml, (gchar *) key), "clicked",
    G_CALLBACK(window_callback_present), (gpointer) glade_xml_get_widget(xml, value));

}

/* associate button click with callback to hide popup window */

static void done_button_setup(gpointer key, gpointer value, gpointer data) {

  g_signal_connect(glade_xml_get_widget(xml, (gchar *) key), "clicked",
    G_CALLBACK(widget_callback_hide), (gpointer) glade_xml_get_widget(xml, value));

}

/* associate button click with callback to hide popup windows */

static void close_button_setup(gpointer key, gpointer value, gpointer data) {

  g_signal_connect(glade_xml_get_widget(xml, (gchar *) key), "clicked",
    G_CALLBACK(widget_callback_hide), (gpointer) glade_xml_get_widget(xml, value));

}

/* to associate popup window with parent window */

void widget_hide_self(GtkObject *object, gpointer data) {

  gtk_widget_hide(GTK_WIDGET(object));

}

static void parent_window_setup(gpointer key, gpointer value, gpointer data) {

  GtkWindow *child = GTK_WINDOW(glade_xml_get_widget(xml, (gchar *) key));
  GtkWindow *parent = GTK_WINDOW(glade_xml_get_widget(xml, (gchar *) value));

  gtk_window_set_transient_for(child, parent);
  gtk_window_set_destroy_with_parent(child, TRUE);

  g_signal_connect(G_OBJECT(parent), "delete-event",
    G_CALLBACK(widget_callback_hide_on_event), G_OBJECT(child));

}

/* associate actions related to popup windows */

static void popups_setup() {

  GList *popup_window_list = NULL;
  GHashTable *popup_button_hash = g_hash_table_new(NULL, NULL);
  GHashTable *close_button_hash = g_hash_table_new(NULL, NULL);
  GHashTable *done_button_hash = g_hash_table_new(NULL, NULL);
  GHashTable *parent_window_hash = g_hash_table_new(NULL, NULL);

  popup_window_list = g_list_append(popup_window_list, "bankd_window1");
  popup_window_list = g_list_append(popup_window_list, "supd_window1");
  popup_window_list = g_list_append(popup_window_list, "brid_window1");
  popup_window_list = g_list_append(popup_window_list, "ppid_window1");
  popup_window_list = g_list_append(popup_window_list, "subd_window1");
  popup_window_list = g_list_append(popup_window_list, "tcatd_window1");
  popup_window_list = g_list_append(popup_window_list, "repd_window1");
  popup_window_list = g_list_append(popup_window_list, "cusd_window1");
  popup_window_list = g_list_append(popup_window_list, "ppid_window1");
  popup_window_list = g_list_append(popup_window_list, "invd_window1");
  popup_window_list = g_list_append(popup_window_list, "contd_window1");
  popup_window_list = g_list_append(popup_window_list, "psid_window1");
  popup_window_list = g_list_append(popup_window_list, "cin_window1");
  popup_window_list = g_list_append(popup_window_list, "invd_window1");
  popup_window_list = g_list_append(popup_window_list, "conn_window1");
  popup_window_list = g_list_append(popup_window_list, "nconnc_dialog1");
  popup_window_list = g_list_append(popup_window_list, "connc_dialog1");
  popup_window_list = g_list_append(popup_window_list, "error_dialog1");

  g_hash_table_insert(popup_button_hash, "sup_bank_button1", "bankd_window1");
  g_hash_table_insert(popup_button_hash, "rep_bank_button1", "bankd_window1");
  g_hash_table_insert(popup_button_hash, "cus_bank_button1", "bankd_window1");
  g_hash_table_insert(popup_button_hash, "pur_supplier_button1", "supd_window1");
  g_hash_table_insert(popup_button_hash, "pur_brand_button1", "brid_window1");
  g_hash_table_insert(popup_button_hash, "pur_item_button1", "brid_window1");
  g_hash_table_insert(popup_button_hash, "pit_rec_purchase_button1", "ppid_window1");
  g_hash_table_insert(popup_button_hash, "pit_rec_supplier_button1", "ppid_window1");
  g_hash_table_insert(popup_button_hash, "pit_rec_brand_button1", "ppid_window1");
  g_hash_table_insert(popup_button_hash, "pit_rec_item_button1", "ppid_window1");
  g_hash_table_insert(popup_button_hash, "pit_ret_purchase_button1", "ppid_window1");
  g_hash_table_insert(popup_button_hash, "pit_ret_supplier_button1", "ppid_window1");
  g_hash_table_insert(popup_button_hash, "pit_ret_brand_button1", "ppid_window1");
  g_hash_table_insert(popup_button_hash, "pit_ret_item_button1", "ppid_window1");
  g_hash_table_insert(popup_button_hash, "pit_ref_purchase_button1", "ppid_window1");
  g_hash_table_insert(popup_button_hash, "pit_ref_supplier_button1", "ppid_window1");
  g_hash_table_insert(popup_button_hash, "pit_ref_brand_button1", "ppid_window1");
  g_hash_table_insert(popup_button_hash, "pit_ref_item_button1", "ppid_window1");
  g_hash_table_insert(popup_button_hash, "ccat_supplier_button1", "subd_window1");
  g_hash_table_insert(popup_button_hash, "ccat_brand_button1", "subd_window1");
  g_hash_table_insert(popup_button_hash, "ccat_taxcategory_button1", "tcatd_window1");
  g_hash_table_insert(popup_button_hash, "sal_repid_button1", "repd_window1");
  g_hash_table_insert(popup_button_hash, "sal_repsaleslocation_button1", "repd_window1");
  g_hash_table_insert(popup_button_hash, "sal_customer_button1", "cusd_window1");
  g_hash_table_insert(popup_button_hash, "sal_contcategory_button1", "contd_window1");
  g_hash_table_insert(popup_button_hash, "sal_contnumber_button1", "contd_window1");
  g_hash_table_insert(popup_button_hash, "sal_supplier_button1", "contd_window1");
  g_hash_table_insert(popup_button_hash, "sal_brand_button1", "contd_window1");
  g_hash_table_insert(popup_button_hash, "sal_item_button1", "contd_window1");
  g_hash_table_insert(popup_button_hash, "sit_sen_date_button1", "invd_window1");
  g_hash_table_insert(popup_button_hash, "sit_sen_invno_button1", "invd_window1");
  g_hash_table_insert(popup_button_hash, "sit_sen_sale_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_sen_suprequested_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_sen_brandrequested_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_sen_itemrequested_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_sen_supused_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_sen_brandused_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_sen_itemused_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_ret_sale_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_ret_suprequested_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_ret_brandrequested_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_ret_itemrequested_button", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_ret_supused_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_ret_brandused_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_ret_itemused_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_ref_sale_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_ref_suprequested_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_ref_brandrequested_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_ref_itemrequested_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_ref_supused_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_ref_brandused_button1", "psid_window1");
  g_hash_table_insert(popup_button_hash, "sit_ref_itemused_button1", "psid_window1");

  g_hash_table_insert(done_button_hash, "bankd_done_button1", "bankd_window1");
  g_hash_table_insert(done_button_hash, "supd_done_button1", "supd_window1");
  g_hash_table_insert(done_button_hash, "brid_done_button1", "brid_window1");
  g_hash_table_insert(done_button_hash, "ppid_done_button1", "ppid_window1");
  g_hash_table_insert(done_button_hash, "subd_done_button1", "subd_window1");
  g_hash_table_insert(done_button_hash, "tcatd_done_button1", "tcatd_window1");
  g_hash_table_insert(done_button_hash, "contd_done_button1", "contd_window1");
  g_hash_table_insert(done_button_hash, "repd_done_button1", "repd_window1");
  g_hash_table_insert(done_button_hash, "cusd_done_button1", "cusd_window1");
  g_hash_table_insert(done_button_hash, "psid_done_button1", "psid_window1");
  g_hash_table_insert(done_button_hash, "invd_done_button1", "invd_window1");

  g_hash_table_insert(close_button_hash, "conn_close_button1", "conn_window1");
  g_hash_table_insert(close_button_hash, "cin_close_button1", "cin_window1");
  g_hash_table_insert(close_button_hash, "nconnc_close_button1", "nconnc_dialog1");
  g_hash_table_insert(close_button_hash, "connc_close_button1", "connc_dialog1");
  //g_hash_table_insert(close_button_hash, "connc_close_button1", "conn_window1");
  g_hash_table_insert(close_button_hash, "error_close_button1", "error_dialog1");

  g_hash_table_insert(parent_window_hash, "bankd_window1", "whm_window1");
  g_hash_table_insert(parent_window_hash, "supd_window1", "whm_window1");
  g_hash_table_insert(parent_window_hash, "brid_window1", "whm_window1");
  g_hash_table_insert(parent_window_hash, "ppid_window1", "whm_window1");
  g_hash_table_insert(parent_window_hash, "subd_window1", "whm_window1");
  g_hash_table_insert(parent_window_hash, "tcatd_window1", "whm_window1");
  g_hash_table_insert(parent_window_hash, "repd_window1", "whm_window1");
  g_hash_table_insert(parent_window_hash, "cusd_window1", "whm_window1");
  g_hash_table_insert(parent_window_hash, "conn_window1", "whm_window1");
  g_hash_table_insert(parent_window_hash, "cin_window1", "whm_window1");
  g_hash_table_insert(parent_window_hash, "invd_window1", "whm_window1");
  //g_hash_table_insert(parent_window_hash, "nconn_dialog1", "conn_window1");
  //g_hash_table_insert(parent_window_hash, "conn_dialog1", "conn_window1");
  //not working

  g_list_foreach(popup_window_list, popup_window_setup, NULL);
  g_hash_table_foreach(popup_button_hash, popup_button_setup, NULL);
  g_hash_table_foreach(done_button_hash, done_button_setup, NULL);
  g_hash_table_foreach(close_button_hash, close_button_setup, NULL);
  g_hash_table_foreach(parent_window_hash, parent_window_setup, NULL);

}

/* setup specified combo box to hold values from a list */

GtkWidget *combo_box_text_setup(GtkWidget *combo_box) {

  GtkListStore *store;
  GtkCellRenderer *cell;

  store = gtk_list_store_new(1, G_TYPE_STRING);
  gtk_combo_box_set_model(GTK_COMBO_BOX(combo_box), GTK_TREE_MODEL(store));
  g_object_unref(store);

  cell = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo_box), cell, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo_box), cell, "text", 0, NULL);

  return combo_box;

}

/* display GDA providers which support SQL in connection window's combo box */

void providers_display() {

  GtkWidget *conn_provider_combobox1 = glade_xml_get_widget(xml, "conn_provider_combobox1");

  conn_provider_combobox1 = combo_box_text_setup(conn_provider_combobox1);

  GList *prov_list = gda_config_get_provider_list();

  if (prov_list == NULL) {

    g_printf("No database providers found.\n");

    exit(EXIT_FAILURE);

  } else {

    GList *node;

    for (node = g_list_first(prov_list); node != NULL; node = g_list_next(node)) {

      //if (gda_connection_supports(cnc, GDA_CONNECTION_FEATURE_SQL))

      gtk_combo_box_append_text(GTK_COMBO_BOX(conn_provider_combobox1), ((GdaProviderInfo *) node->data)->id);

    }

    gda_config_free_provider_list(prov_list);

    gtk_combo_box_set_active(GTK_COMBO_BOX(conn_provider_combobox1), 0);

  }

}

/* activate combo box entry matching supplied text */

void providers_set_active(gchar *text) {

  GtkTreeIter iter;

  GtkTreeModel *model = gtk_combo_box_get_model(conn_provider_combobox1);

  gboolean valid = gtk_tree_model_get_iter_first(model, &iter);

  gchar *value;

  while (valid) {

    gtk_tree_model_get(model, &iter, 0, &value, -1);

    if (g_ascii_strcasecmp(value, text) == 0)

      gtk_combo_box_set_active_iter(conn_provider_combobox1, &iter);

    valid = gtk_tree_model_iter_next(model, &iter);

  }

}

/* method used to set up the database specific aspects of the application interface */

static void database_setup() {

  GdaDataSourceInfo *info;

  client = gda_client_new();

  providers_display();

  if ((info = gda_config_find_data_source("warehouse"))) {

    providers_set_active(info->provider);
    gtk_entry_set_text(conn_username_entry1, info->username + 5);
    gtk_entry_set_text(conn_password_entry1, info->password + 9);

  }

}

void interface_setup(void) {

  whm_window1 = GTK_WINDOW(glade_xml_get_widget(xml, "whm_window1"));
  conn_provider_combobox1 = GTK_COMBO_BOX(glade_xml_get_widget(xml, "conn_provider_combobox1"));
  conn_host_entry1 = GTK_ENTRY(glade_xml_get_widget(xml, "conn_host_entry1"));
  conn_port_spinbutton1 = GTK_SPIN_BUTTON(glade_xml_get_widget(xml, "conn_port_spinbutton1"));
  conn_database_entry1 = GTK_ENTRY(glade_xml_get_widget(xml, "conn_database_entry1"));
  conn_username_entry1 = GTK_ENTRY(glade_xml_get_widget(xml, "conn_username_entry1"));
  conn_password_entry1 = GTK_ENTRY(glade_xml_get_widget(xml, "conn_password_entry1"));
  conn_rememberpassword_checkbutton1 = GTK_CHECK_BUTTON(glade_xml_get_widget(xml, "conn_rememberpassword_checkbutton1"));
  nconnc_dialog1 = GTK_DIALOG(glade_xml_get_widget(xml, "nconnc_dialog1"));
  connc_dialog1 = GTK_DIALOG(glade_xml_get_widget(xml, "connc_dialog1"));

  g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "conn_connect_button1")), "clicked",
    G_CALLBACK(database_callback_connect), NULL);

  g_signal_connect(whm_window1, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  menus_setup();
  toolbuttons_setup();
  notebooks_setup();
  popups_setup();
  database_setup();

}
