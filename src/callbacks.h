/* anything needed to setup callbacks */

#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "common.h"

/* data structure used to store notebook page numbers */
/* to be selected with negative values indicating that */
/* no selection is necessary for corresponding notebook */

typedef struct _PageNumbers {

  gint main, pit, sit;

} PageNumbers;

/* data structure used to represent notebook hierarchy */

typedef struct _NotebookTree {

  GtkNotebook *notebook;

  struct _NotebookTree *parent;

  struct _NotebookTree **children;

} NotebookTree;

/* construct data structure used to represent notebook hierarchy */

void notebook_tree_construct(void);

/* set current notebook page to previous notebook page in notebook hierarchy */

void notebook_tree_previous_page(void);

/* set current notebook page to next notebook page in notebook hierarchy */

void notebook_tree_next_page(void);

/* instance used to represent current notebook in notebook hierarchy */

NotebookTree *notebook_tree_current;

/* quit application from menu item */

void menuitem_callback_quit(GtkMenuItem *menuitem, gpointer data);

/* set current notebook page to previous notebook page in notebook hierarchy */

void menuitem_callback_previous(GtkMenuItem *item, gpointer data);

/* set current notebook page to next notebook page in notebook hierarchy */

void menuitem_callback_next(GtkMenuItem *item, gpointer data);

/* set current notebook page to page corresponding to selected menu item */

void menuitem_callback_page_selection(GtkMenuItem *menuitem, gpointer data);

/* set current notebook page to previous notebook page in notebook hierarchy */

void toolbutton_callback_previous(GtkToolButton *button, gpointer data);

/* set current notebook page to next notebook page in notebook hierarchy */

void toolbutton_callback_next(GtkToolButton *button, gpointer data);

/* update current notebook tree node when a notebook page is clicked */

void notebook_tree_callback_set_current(GtkNotebook *notebook, GtkNotebookPage *, guint, gpointer);

/* present window on receipt of internal signal */

void window_callback_present(GtkObject *object, gpointer data);

/* hide widget on receipt of internal signal */

void widget_callback_hide(GtkObject *object, gpointer data);

/* hide widget on receipt of external event */

gboolean widget_callback_hide_on_event(GtkWidget *widget, GdkEvent event, gpointer data);

/* connect to database */

void database_callback_connect(GtkButton *button, gpointer data);

#endif // CALLBAKCS_H
