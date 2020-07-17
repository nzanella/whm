#ifndef PARSER_H
#define PARSER_H

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <libgda/libgda.h>
#include <glib/gprintf.h>

typedef struct _DgField {
  gchar *name;        /* field name appearing in an SQL query */
  gboolean required;  /* whether this field is required */
  GdaValueType type;  /* corresponding application data type */
} DgField;

typedef GList DgFields;  /* field names appearing in an SQL query */

typedef struct _DgExtractor {
  gchar *query;      /* text comprising SQL query used to extract data */
  DgFields *fields;  /* field names appearing in SQL query used to extract data */
} DgExtractor;

typedef struct _DgSelector {
  GtkCList *clist;   /* widget from which user selects data */
  DgFields *fields;  /* pointers to constructed fields whose corresponding data is available for selection */
} DgSelector;

typedef struct _DgExposer {
  GtkWidget *widget;  /* widget used to expose retrieved query data */
  DgField *field;     /* pointers to constructed fields whose corresponding data is available for exposure */
} DgExposer;

typedef GList DgExposers;

typedef enum _DgQueryType {
  DG_QUERY_TYPE_ORDINARY,       /* ordinary SQL query */
  DG_QUERY_TYPE_EMPTYCHECK,     /* check that result set of SQL query is empty */
  DG_QUERY_TYPE_NONEMPTYCHECK,  /* check that result set of SQL query is nonempty */
  DG_QUERY_TYPE_EMPTYTEST       /* take action according to whether result set of SQL query is empty */
} DgQueryType;

typedef GList DgQueries;  /* queries used to manipulate the database */

typedef struct _DgQuery {
  DgQueryType type;         /* specify whether the result set should be checked */
  gchar *query;             /* text comprising SQL query used to manipulate database */
  gchar *error_message;     /* when a check fails this error message is displayed */
  DgQueries *emptycase;     /* when a test yields empty these queries are executed */
  DgQueries *nonemptycase;  /* when a test yields nonempty these queries are executed */
} DgQuery;

typedef struct _DgExecutor {
  GtkToggleButton *atb;  /* add toggle button */
  GtkToggleButton *mtb;  /* modify toggle button */
  GtkToggleButton *rtb;  /* remove toggle button */
  GtkToggleButton *stb;  /* submit toggle button */
  GtkToggleButton *ctb;  /* cancel toggle button */
  DgQueries *inserter;   /* queries used to manipulate the database upon an add request */
  DgQueries *updater;    /* queries used to manipulate the database upon a modify request */
  DgQueries *deleter;    /* queries used to manipulate the database upon a remove request */
} DgExecutor;

typedef struct _DgManipulator {
  DgExposers *exps;  /* widget used to expose data to user and possibly allow manipulation */
  DgExecutor *exec;  /* when set to some value points to actions which may be carried out */
} DgManipulator;

typedef enum _DgControlState {
  DG_NONE,    /* user has not initiated any operation */
  DG_ADD,     /* user has initiated an add operation */
  DG_MODIFY,  /* user has initiated a modify operation */
  DG_REMOVE   /* user has initiated a remove operation */
} DgControlState;

typedef GList DgControls;

struct _DgXML;

typedef struct _DgControl {
  gchar *cname;          /* control name used to identify control */
  DgExtractor *extr;     /* pointer to data extraction structure */
  DgSelector *sel;       /* pointer to data selection structure */
  DgManipulator *manip;  /* pointer to data manipulation structure */
  GdaDataModel *dm;      /* result set from executed queries */
  gint rownum;           /* result set selected row number */
  DgControlState state;  /* control state */
  struct _DgControl *master;  /* master control node */
  DgControls *details;        /* detail control nodes */
  struct _DgXML *dg;     /* back pointer to data gui structure */
} DgControl;

typedef struct _DgMasterDetail {
  DgControl *master;   /* master control node */
  DgControl *detail;   /* detail control node */
  DgFields *fields;    /* master detail fields */
} DgMasterDetail;

typedef struct _DgMaplet {
  DgField *sfield;  /* sender field */
  DgField *rfield;  /* receiver field */
} DgMaplet;

typedef GList DgMaplets;

typedef struct _DgSendReceive {
  DgControl *sender;     /* control which knows about values to be copied to receiver */
  DgControl *receiver;   /* control which is to receive values from sender */
  DgMaplets *maplets;    /* correspondence of sender fields to receiver fields */
  GtkButton *messenger;  /* optional widget used to initiate data movement */
} DgSendReceive;

typedef GList DgMasterDetails;
typedef GList DgSendReceives;

typedef struct _DgXML {
  DgControls *ctls;         /* complete list of controls */
  DgMasterDetails *mds;     /* complete list of master details */
  DgSendReceives *srs;      /* complete list of send receives */
  GladeXML *xml;            /* glade XML user interface structure */
  GdaConnection *cnc;       /* database connection */
  GtkDialog *error_dialog;  /* dialog used to display errors */
  GtkLabel *error_label;    /* dialog label used to display error */
} DgXML;

typedef struct _DgControlIter {
  DgControl *ctl;
  GQueue *fringe;
} DgControlIter;

void dg_print_control(DgControl *control);

void dg_print_controls(DgControls *controls);

DgControl *dg_get_control_by_name(DgControls *controls, const char *cname);

#endif // PARSER_H
