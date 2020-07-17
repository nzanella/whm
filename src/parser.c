#include "datagui.h"
#include "util.h"

static GList *mdXmlNodes = NULL;
static GList *srXmlNodes = NULL;
static GList *clXmlNodes = NULL;

/* print control structure */

void dg_print_control(DgControl *control) {

  DgFields *flds;

  g_printf("Control Name: %s\n", control->cname);

  g_printf("  Extractor fields:\n");

  for (flds = g_list_first(control->extr->fields); flds; flds = flds->next) {

    DgField *fld = (DgField *) flds->data;

    g_printf("    Extractor field: %s\n", fld->name);

  }

  g_printf("  Selector fields:\n");

  for (flds = g_list_first(control->sel->fields); flds; flds = flds->next) {

    DgField *fld = (DgField *) flds->data;

    g_printf("    Selector field: %s\n", fld->name);

  }

  if (control->manip) {

    DgExposers *exps;

    g_printf("  Exposer fields:\n");

    for (exps = g_list_first(control->manip->exps); exps; exps = exps->next) {

      DgExposer *exp = (DgExposer *) exps->data;

      g_printf("    Exposer field: %s\n", exp->field->name);

    }

  }
  if (control->master)

    g_printf("  Master Name: %s\n", control->master->cname);

  DgControls *dtls;

  for (dtls = g_list_first(control->details); dtls; dtls = dtls->next) {

    DgControl *dtl = (DgControl *) dtls->data;

    g_printf("  Detail Name: %s\n", dtl->cname);

  }

}

/* print all control structures */

void dg_print_controls(DgControls *controls) {

  DgControls *ctls;

  for (ctls = g_list_first(controls); ctls; ctls = ctls->next) {

    DgControl *ctl = (DgControl *) ctls->data;

    dg_print_control(ctl);

  }

}

/* retrieve control structure corresponding to control name */

DgControl *dg_get_control_by_name(DgControls *controls, const char *cname) {

  DgControls *ctls;

  /* iterate through supplied controls */

  for (ctls = g_list_first(controls); ctls; ctls = ctls->next) {

    /* retrieve control */

    DgControl *ctl = (DgControl *) ctls->data;

    /* check whether current control name matches supplied control name */

    if (g_ascii_strcasecmp(ctl->cname, cname) == 0)

      /* retrieve control */

      return ctl;

  }

  /* control not found */

  return NULL;

}

/* retrieve field structure corresponding to given field name */

static DgField *get_field_by_name(DgFields *fields, const char *fname) {

  DgFields *flds;

  /* iterate through supplied fields */

  for (flds = g_list_first(fields); flds; flds = flds->next) {

    /* retrieve field */

    DgField *fld = (DgField *) flds->data;

    /* check whether current field name matches supplied field name */

    if (g_ascii_strcasecmp(fld->name, fname) == 0)

      /* retrieve field */

      return fld;

  }

  /* field not found */

  return NULL;

}

static void parse_check(xmlNodePtr node, DgQuery **query) {

  xmlNodePtr child;

  /* retrieve children */

  child = node->children;

  /* process children */

  while (child) {

    /* check whether current node is a query element */

    if (xmlStrcmp(child->name, "query") == 0) {

      /* ensure query element contains query text */

      g_assert(child->children && child->children->type == XML_TEXT_NODE);

      /* set query text */

      (*query)->query = (gchar *) xmlNodeGetContent(child);

    }

    /* check whether current node is an error message element */

    else if (xmlStrcmp(child->name, "errormessage") == 0) {

      /* ensure error message element contains error message text */

      g_assert(child->children && child->children->type == XML_TEXT_NODE);

      /* set query text */

      (*query)->error_message = (gchar *) xmlNodeGetContent(child);

    }

    /* proceed to process next sibling node */

    child = child->next;

  }

}

static void parse_queries(xmlNodePtr node, DgQueries **queries) {

  /* retrieve children */

  node = node->children;

  /* process children */

  while (node) {

    /* check whether current node is an empty test element */

    if (xmlStrcmp(node->name, "emptytest") == 0) {

      xmlNodePtr child;

      /* create new query */

      DgQuery *query = g_new0(DgQuery, 1);

      /* set query type */

      query->type = DG_QUERY_TYPE_EMPTYTEST;

      /* retrieve children */

      child = node->children;

      /* process children */

      while (child) {

        /* check whether current node is a query element */

        if (xmlStrcmp(child->name, "query") == 0) {

          /* ensure query element contains query text */

          g_assert(child->children && child->children->type == XML_TEXT_NODE);

          /* set query text */

          query->query = (gchar *) xmlNodeGetContent(child);

        }

        /* check whether current node is an empty case element */

        else if (xmlStrcmp(child->name, "emptycase") == 0) {

          /* parse empty case queries */

          parse_queries(child, &query->emptycase);

        }

        /* check whether current node is a nonempty case element */

        else if (xmlStrcmp(child->name, "nonemptycase") == 0) {

          /* parse nonempty case queries */

          parse_queries(child, &query->nonemptycase);

        }

        /* proceed to process next sibling node */

        child = child->next;

      }

      /* keep track of query */

      *queries = g_list_append(*queries, (gpointer) query);

    }

    /* check whether current node is an empty check element */

    if (xmlStrcmp(node->name, "emptycheck") == 0) {

      /* create new query */

      DgQuery *query = g_new(DgQuery, 1);

      /* set query type */

      query->type = DG_QUERY_TYPE_EMPTYCHECK;

      /* parse nonempty check element */

      parse_check(node, &query);

      /* keep track of query */

      *queries = g_list_append(*queries, (gpointer) query);

    }

    else if (xmlStrcmp(node->name, "nonemptycheck") == 0) {

      /* create new query */

      DgQuery *query = g_new(DgQuery, 1);

      /* set query type */

      query->type = DG_QUERY_TYPE_NONEMPTYCHECK;

      /* parse nonempty check element */

      parse_check(node, &query);

      /* keep track of query */

      *queries = g_list_append(*queries, (gpointer) query);

    }

    /* check whether current node is a query element */

    else if (xmlStrcmp(node->name, "query") == 0) {

      /* create new query */

      DgQuery *query = g_new(DgQuery, 1);

      /* set query type */

      query->type = DG_QUERY_TYPE_ORDINARY;

      /* ensure query element contains query text */

      g_assert(node->children && node->children->type == XML_TEXT_NODE);

      /* retrieve and set query text */

      query->query = (gchar *) xmlNodeGetContent(node);

      /* keep track of query */

      *queries = g_list_append(*queries, (gpointer) query);

    }

    /* proceed to process next sibling node */

    node = node->next;

  }

}

static void parse_extractor(xmlNodePtr node, DgControl *control) {

  /* create and keep track of new extractor */

  control->extr = g_new0(DgExtractor, 1);

  /* retrieve children */

  node = node->children;

  /* process children */

  while (node) {

    /* check whether current node is a query element */

    if (xmlStrcmp(node->name, "query") == 0) {

      /* ensure query element contains query text */

      g_assert(node->children && node->children->type == XML_TEXT_NODE);

      /* retrieve and set query text */

      control->extr->query = (gchar *) xmlNodeGetContent(node);

    }

    /* check whether current node is a field element */

    else if (xmlStrcmp(node->name, "field") == 0) {

      /* create new field */

      DgField *field = g_new(DgField, 1);

      /* retrieve whether field is required */

      gchar *required = (gchar *) xmlGetProp(node, "required");

      /* set whether field is required */

      field->required = required != NULL && g_ascii_strcasecmp(required, "yes") == 0 ? TRUE : FALSE;

      /* retrieve and set field name */

      field->name = (gchar *) xmlGetProp(node, "fname");

      /* ensure field name was retrieved successfully */

      if (!field->name) {

        g_fprintf(stderr, "Data GUI error: no field name specified for extractor field.\n");
        g_fprintf(stderr, "Control name: %s\n", control->cname);
        exit(EXIT_FAILURE);

      }

      /* retrieve and set field type */

      field->type = util_get_type_from_name((gchar *) xmlGetProp(node, "type"));

      /* ensure field has a type */

      g_assert(field->type);

      /* keep track of new field */

      control->extr->fields = g_list_append(control->extr->fields, (gpointer) field);

    }

    /* proceed to process next sibling node */

    node = node->next;

  }

}

static void parse_selector(xmlNodePtr node, DgControl *control) {

  /* create and keep track of new selector */

  control->sel = g_new0(DgSelector, 1);

  /* retrieve selector columned list widget name */

  gchar *name = (gchar *) xmlGetProp(node, "name");

  /* ensure selector columned list widget name was retrieved successfully */

  if (!name) {

    g_fprintf(stderr, "Data GUI error: no widget name specified for selector columned list widget.\n");
    g_fprintf(stderr, "Control name: %s\n", control->cname);
    exit(EXIT_FAILURE);

  }

  /* retrieve and set selector columned list widget */

  control->sel->clist = GTK_CLIST(glade_xml_get_widget(control->dg->xml, name));

  /* ensure selector columned list widget was retrieved successfully */

  if (!control->sel->clist) {

    g_fprintf(stderr, "Data GUI error: selector columned list widget with given name does not exist.\n");
    g_fprintf(stderr, "Control name: %s\n", control->cname);
    g_fprintf(stderr, "Widget name: %s\n", name);
    exit(EXIT_FAILURE);

  }

  /* retrieve children */

  node = node->children;

  /* process children */

  while (node) {

    /* check whether current node is a header element */

    if (xmlStrcmp(node->name, "header") == 0) {

      /* retrieve field name corresponding to header */

      gchar *fname = (gchar *) xmlGetProp(node, "fname");

      /* ensure field name was retrieved successfully */

      if (!fname) {

        g_fprintf(stderr, "Data GUI error: no field name specified for header.\n");
        g_fprintf(stderr, "Control name: %s\n", control->cname);
        exit(EXIT_FAILURE);

      }

      /* retrieve and set field corresponding to header */

      DgField *field = get_field_by_name(control->extr->fields, fname);

      /* ensure selector has a field */

      if (!field) {

        g_fprintf(stderr, "Data GUI error: selector field not found among extractor fields.\n");
        g_fprintf(stderr, "Control name: %s\n", control->cname);
        g_fprintf(stderr, "Field name: %s\n", fname);
        exit(EXIT_FAILURE);

      }

      /* keep track of field */

      control->sel->fields = g_list_append(control->sel->fields, (gpointer) field);

    }

    /* proceed to process next sibling node */

    node = node->next;

  }

  /* number of columned list headers */

  gint num_columns;

  /* retrieve number of columned list headers */

  g_object_get(G_OBJECT(control->sel->clist), "n-columns", &num_columns, NULL);

  /* ensure number of selector fields metches number of columned list headers */

  if (num_columns != g_list_length(control->sel->fields)) {

    g_fprintf(stderr, "Data GUI error: selector fields do not match columned list fields in number.\n");
    g_fprintf(stderr, "Control name: %s\n", control->cname);
    g_fprintf(stderr, "Selector fields: %d\n", g_list_length(control->sel->fields));
    g_fprintf(stderr, "Columned list fields: %d\n", num_columns);
    exit(EXIT_FAILURE);

  }

}

static void parse_exposer(xmlNodePtr node, DgControl *control) {

  /* create new exposer */

  DgExposer *exp = g_new(DgExposer, 1);

  /* retrieve field name corresponding to exposer */

  gchar *fname = (gchar *) xmlGetProp(node, "fname");

  /* ensure field name was retrieved successfully */

  if (!fname) {

    g_fprintf(stderr, "Data GUI error: no field name specified for exposer.\n");
    g_fprintf(stderr, "Control name: %s\n", control->cname);
    exit(EXIT_FAILURE);

  }

  /* retrieve and set field corresponding to exposer widget */

  exp->field = get_field_by_name(control->extr->fields, fname);

  /* ensure exposer has a field */

  if (!exp->field) {

    g_fprintf(stderr, "Data GUI error: undeclared exposer field name.\n");
    g_fprintf(stderr, "Control name: %s\n", control->cname);
    g_fprintf(stderr, "Field name: %s\n", fname);
    exit(EXIT_FAILURE);

  }

  /* retrieve exposer widget name */

  gchar *name = (gchar *) xmlGetProp(node, "name");

  /* ensure widget name was retrieved successfully */

  if (!name) {

    g_fprintf(stderr, "Data GUI error: no widget name specified for exposer.\n");
    g_fprintf(stderr, "Control name: %s\n", control->cname);
    exit(EXIT_FAILURE);

  }

  /* set exposer widget */

  exp->widget = glade_xml_get_widget(control->dg->xml, name);

  /* ensure exposer widget exists */

  if (!exp->widget) {

    g_fprintf(stderr, "Data GUI error: exposer widget with given name does not exist.\n");
    g_fprintf(stderr, "Control name: %s\n", control->cname);
    g_fprintf(stderr, "Widget name: %s\n", name);
    exit(EXIT_FAILURE);

  }

  /* keep track of exposer */

  control->manip->exps = g_list_append(control->manip->exps, (gpointer) exp);

}

static void parse_tb(xmlNodePtr node, DgControl *control, GtkToggleButton **tb) {

  /* retrieve toggle button name */

  gchar *tbname = (gchar *) xmlGetProp(node, "name");

  /* ensure toggle button name was retrieved successfully */

  if (!tbname) {

    g_fprintf(stderr, "Data GUI error: executor toggle button name not specified.\n");
    g_fprintf(stderr, "Control name: %s\n", control->cname);
    exit(EXIT_FAILURE);

  }

  /* retrieve and set toggle button */

  *tb = GTK_TOGGLE_BUTTON(glade_xml_get_widget(control->dg->xml, tbname));

  /* ensure toggle button was retrieved and set successfully */

  if (!*tb) {

    g_fprintf(stderr, "Data GUI error: toggle button with specified name does not exist.\n");
    g_fprintf(stderr, "Control name: %s\n", control->cname);
    g_fprintf(stderr, "Toggle button name: %s\n", tbname);
    exit(EXIT_FAILURE);

  }

}

static void parse_executor(xmlNodePtr node, DgControl *control) {

  /* create and set executor */

  control->manip->exec = g_new0(DgExecutor, 1);

  /* retrieve children */

  node = node->children;

  /* process children */

  while (node) {

    /* check whether current node is an add element */

    if (xmlStrcmp(node->name, "add") == 0) {

      /* parse add element */

      parse_tb(node, control, &control->manip->exec->atb);

    }

    /* check whether current node is a modify element */

    else if (xmlStrcmp(node->name, "modify") == 0) {

      /* parse modify element */

      parse_tb(node, control, &control->manip->exec->mtb);

    }

    /* check whether current node is a remove element */

    else if (xmlStrcmp(node->name, "remove") == 0) {

      /* parse remove element */

      parse_tb(node, control, &control->manip->exec->rtb);

    }

    /* check whether current node is a submit element */

    else if (xmlStrcmp(node->name, "submit") == 0) {

      /* parse submit element */

      parse_tb(node, control, &control->manip->exec->stb);

    }

    /* check whether current node is a cancel element */

    else if (xmlStrcmp(node->name, "cancel") == 0) {

      /* parse cancel element */

      parse_tb(node, control, &control->manip->exec->ctb);

    }

    /* check whether current node is an inserter element */

    else if (xmlStrcmp(node->name, "inserter") == 0) {

      /* parse inserter element */

      parse_queries(node, &control->manip->exec->inserter);

    }

    /* check whether current node is a updater element */

    else if (xmlStrcmp(node->name, "updater") == 0) {

      /* parse updater element */

      parse_queries(node, &control->manip->exec->updater);

    }

    /* check whether current node is a remover element */

    else if (xmlStrcmp(node->name, "deleter") == 0) {

      /* parse deleter element */

      parse_queries(node, &control->manip->exec->deleter);

    }

    /* proceed to process next sibling node */

    node = node->next;

  }

}

static void parse_cloneexecutor(xmlNodePtr node, DgControl *control, DgControl *srccontrol) {

  /* create and set executor */

  control->manip->exec = g_new0(DgExecutor, 1);

  /* retrieve children */

  node = node->children;

  /* process children */

  while (node) {

    /* check whether current node is an add element */

    if (xmlStrcmp(node->name, "add") == 0) {

      /* parse add element */

      parse_tb(node, control, &control->manip->exec->atb);

    }

    /* check whether current node is a modify element */

    else if (xmlStrcmp(node->name, "modify") == 0) {

      /* parse modify element */

      parse_tb(node, control, &control->manip->exec->mtb);

    }

    /* check whether current node is a remove element */

    else if (xmlStrcmp(node->name, "remove") == 0) {

      /* parse remove element */

      parse_tb(node, control, &control->manip->exec->rtb);

    }

    /* check whether current node is a submit element */

    else if (xmlStrcmp(node->name, "submit") == 0) {

      /* parse submit element */

      parse_tb(node, control, &control->manip->exec->stb);

    }

    /* check whether current node is a cancel element */

    else if (xmlStrcmp(node->name, "cancel") == 0) {

      /* parse cancel element */

      parse_tb(node, control, &control->manip->exec->ctb);

    }

    /* proceed to process next sibling node */

    node = node->next;

  }

  /* set clone control inserter to source control inserter */

  control->manip->exec->inserter = srccontrol->manip->exec->inserter;

  /* set clone control updater to source control updater */

  control->manip->exec->updater = srccontrol->manip->exec->updater;

  /* set clone control deleter to source control deleter */

  control->manip->exec->deleter = srccontrol->manip->exec->deleter;

}

static void parse_manipulator(xmlNodePtr node, DgControl *control) {

  /* create and keep track of new manipulator */

  control->manip = g_new0(DgManipulator, 1);

  /* retrieve children */

  node = node->children;

  /* process children */

  while (node) {

    /* check whether current node is an exposer element */

    if (xmlStrcmp(node->name, "exposer") == 0) {

      /* parse exposer element */

      parse_exposer(node, control);

    /* check whether current node is an executor element */

    } else if (xmlStrcmp(node->name, "executor") == 0) {

      /* parse executor element */

      parse_executor(node, control);

    }

    /* proceed to process next sibling node */

    node = node->next;

  }

}

static void parse_clonemanipulator(xmlNodePtr node, DgControl *control, DgControl *srccontrol) {

  /* create and keep track of new clone manipulator */

  control->manip = g_new0(DgManipulator, 1);

  /* retrieve children */

  node = node->children;

  /* process children */

  while (node) {

    /* check whether current node is an exposer element */

    if (xmlStrcmp(node->name, "exposer") == 0) {

      /* parse exposer element */

      parse_exposer(node, control);

    /* check whether current node is a clone executor element */

    } else if (xmlStrcmp(node->name, "cloneexecutor") == 0) {

      /* parse executor element */

      parse_cloneexecutor(node, control, srccontrol);

    }

    /* proceed to process next sibling node */

    node = node->next;

  }

}

static DgControl *control_new(DgXML *dg) {

  DgControl *ctl = g_new0(DgControl, 1);

  /* initialize control state */

  ctl->state = DG_NONE;

  /* initialize row number corresponding to selected row */

  ctl->rownum = -1;

  /* set back pointer from control to data gui structure */

  ctl->dg = dg;

  /* return control node */

  return ctl;

}

static void parse_control(xmlNodePtr node, DgXML *dg) {

  /* create new control */

  DgControl *control = control_new(dg);

  /* retrieve and set control name */

  control->cname = (gchar *) xmlGetProp(node, "cname");

  /* ensure control name was retrieved successfully */

  if (!control->cname) {

    g_fprintf(stderr, "Data GUI error: no control name specified for control.\n");
    exit(EXIT_FAILURE);

  }

  /* retrieve children */

  node = node->children;

  /* process children */

  while (node) {

    /* check whether current node is an extractor element */

    if (xmlStrcmp(node->name, "extractor") == 0) {

      /* parse extractor element */

      parse_extractor(node, control);

    }

    /* check whether current node is a selector element */

    else if (xmlStrcmp(node->name, "selector") == 0) {

      /* parse selector element */

      parse_selector(node, control);

    }

    /* check whether current node is a manipulator element */

    else if (xmlStrcmp(node->name, "manipulator") == 0) {

      /* parse manipulator element */

      parse_manipulator(node, control);

    }

    /* proceed to process next sibling node */

    node = node->next;

  }

  /* keep track of new control */

  dg->ctls = g_list_append(dg->ctls, (gpointer) control);

}

static void parse_clonecontrol(xmlNodePtr node, DgXML *dg) {

  /* create new control */

  DgControl *control = control_new(dg);

  /* retrieve and set control name */

  control->cname = (gchar *) xmlGetProp(node, "cname");

  /* ensure control name was retrieved successfully */

  if (!control->cname) {

    g_fprintf(stderr, "Data GUI error: no control name specified for clone control.\n");
    exit(EXIT_FAILURE);

  }

  /* retrieve and set source control name */

  gchar *srccname = (gchar *) xmlGetProp(node, "srccname");

  /* ensure source control name was retrieved successfully */

  if (!srccname) {

    g_fprintf(stderr, "Data GUI error: no control name specified for source control.\n");
    g_fprintf(stderr, "Clone control name: %s\n", control->cname);
    exit(EXIT_FAILURE);

  }

  /* retrieve source control */

  DgControl *srccontrol = dg_get_control_by_name(dg->ctls, srccname);

  /* ensure source control was retrieved successfully */

  if (!srccontrol) {

    g_fprintf(stderr, "Data GUI error: source control for clone control does not exist.\n");
    g_fprintf(stderr, "Clone control name: %s\n", control->cname);
    g_fprintf(stderr, "Source control name: %s\n", srccname);
    exit(EXIT_FAILURE);

  }

  /* create and keep track of new extractor */

  control->extr = g_new(DgExtractor, 1);

  /* set clone control extractor query to source control extractor query */

  control->extr->query = srccontrol->extr->query;

  /* set clone control extractor fields to source control extractor fields */

  control->extr->fields = srccontrol->extr->fields;

  /* retrieve children */

  node = node->children;

  /* process children */

  while (node) {

    /* check whether current node is a clone selector element */

    if (xmlStrcmp(node->name, "cloneextractor") == 0) {

      /* retrieve children */

      xmlNodePtr child = node->children;

      while (child) {

        /* check whether current node is a query element */

        if (xmlStrcmp(child->name, "query") == 0) {

          /* ensure query element contains query text */

          g_assert(child->children && child->children->type == XML_TEXT_NODE);

          /* update query text */

          control->extr->query = (gchar *) xmlNodeGetContent(child);

        }

        /* proceed to process next sibling node */

        child = child->next;

      }

    }

    /* check whether current node is a selector element */

    if (xmlStrcmp(node->name, "selector") == 0) {

      /* parse selector element */

      parse_selector(node, control);

    }

    /* check whether current node is a clone manipulator element */

    else if (xmlStrcmp(node->name, "clonemanipulator") == 0) {

      /* parse clone manipulator element */

      parse_clonemanipulator(node, control, srccontrol);

    }

    /* proceed to process next sibling node */

    node = node->next;

  }

  /* keep track of new control */

  dg->ctls = g_list_append(dg->ctls, (gpointer) control);

}

/* setup clone controls */

static void setup_clonecontrols(DgXML *dg, GList *clXmlNodes) {

  GList *nodes;

  /* process clone control nodes */

  for (nodes = g_list_first(clXmlNodes); nodes; nodes = nodes->next) {

    /* retrieve clone control node */

    xmlNodePtr node = (xmlNodePtr) nodes->data;

    /* parse clone control element */

    parse_clonecontrol(node, dg);

  }

}

/* setup master detail relationships */

static void setup_masterdetails(DgXML *dg, GList *mdXmlNodes) {

  GList *nodes;

  /* process master detail nodes */

  for (nodes = g_list_first(mdXmlNodes); nodes; nodes = nodes->next) {

    /* retrieve master detail node */

    xmlNodePtr node = (xmlNodePtr) nodes->data;

    /* create new master detail */

    DgMasterDetail *md = g_new0(DgMasterDetail, 1);

    /* retrieve master control name */

    gchar *mcname = (gchar *) xmlGetProp(node, "mcname");

    /* ensure master control name was retrieved successfully */

    if (!mcname) {

      g_fprintf(stderr, "Data GUI error: no name specified for master control.\n");
      exit(EXIT_FAILURE);

    }

    /* retrieve detail control name */

    gchar *dcname = (gchar *) xmlGetProp(node, "dcname");

    /* ensure detail control name was retrieved successfully */

    if (!dcname) {

      g_fprintf(stderr, "Data GUI error: no name specified for detail control.\n");
      exit(EXIT_FAILURE);

    }

    /* set master control */

    md->master = dg_get_control_by_name(dg->ctls, mcname);

    /* ensure master detail has a master control */

    if (!md->master) {

      g_fprintf(stderr, "Data GUI error: master control does not exist.\n");
      g_fprintf(stderr, "Master control name: %s\n", mcname);
      exit(EXIT_FAILURE);

    }

    /* set detail control */

    md->detail = dg_get_control_by_name(dg->ctls, dcname);

    /* ensure master detail has a detail control */

    if (!md->detail) {

      g_fprintf(stderr, "Data GUI error: detail control does not exist.\n");
      g_fprintf(stderr, "Detail control name: %s\n", dcname);
      exit(EXIT_FAILURE);

    }

    /* retrieve children */

    node = node->children;

    /* process children */

    while (node) {

      /* check whether current node is a master detail field element */

      if (xmlStrcmp(node->name, "mdfield") == 0) {

        /* retrieve field element name */

        gchar *fname = (gchar *) xmlGetProp(node, "fname");

        /* ensure field name was retrieved successfully */

        if (!fname) {

          g_fprintf(stderr, "Data GUI error: no field name specified for master detail field.\n");
          g_fprintf(stderr, "Master control name: %s\n", mcname);
          g_fprintf(stderr, "Detail control name: %s\n", dcname);
          exit(EXIT_FAILURE);

        }

        /* retrieve master detail field corresponding to field name */

        DgField *mdfield = get_field_by_name(md->master->extr->fields, fname);

        /* ensure master detail field was found */

        if (!mdfield) {

          g_fprintf(stderr, "Data GUI error: specified master detail field does not exist.\n");
          g_fprintf(stderr, "Master detail field name: %s\n", fname);
          g_fprintf(stderr, "Master control name: %s\n", mcname);
          g_fprintf(stderr, "Detail control name: %s\n", dcname);
          exit(EXIT_FAILURE);

        }

        /* keep track of master detail field */

        md->fields = g_list_append(md->fields, (gpointer) mdfield);

      }

      /* proceed to process next sibling node */

      node = node->next;

    }

    /* keep track of new master detail */

    dg->mds = g_list_append(dg->mds, (gpointer) md);

  }

}

/* setup send receive relationships */

static void setup_sendreceives(DgXML *dg, GList *srXmlNodes) {

  GList *nodes;

  /* process send receive nodes */

  for (nodes = g_list_first(srXmlNodes); nodes; nodes = nodes->next) {

    /* retrieve send receive node */

    xmlNodePtr node = (xmlNodePtr) nodes->data;

    /* create new send receive */

    DgSendReceive *sr = g_new0(DgSendReceive, 1);

    /* retrieve sender control node name */

    gchar *scname = (gchar *) xmlGetProp(node, "scname");

    /* ensure sender name was retrieved successfully */

    if (!scname) {

      g_fprintf(stderr, "Data GUI error: no sender control name specified.\n");
      exit(EXIT_FAILURE);

    }

    /* ensure receiver name was retrieved successfully */

    gchar *rcname = (gchar *) xmlGetProp(node, "rcname");

    if (!rcname) {

      g_fprintf(stderr, "Data GUI error: no receiver control name specified.\n");
      exit(EXIT_FAILURE);

    }

    /* set sender control */

    sr->sender = dg_get_control_by_name(dg->ctls, scname);

    /* ensure send receive has a sender control */

    if (!sr->sender) {

      g_fprintf(stderr, "Data GUI error: sender control does not exist.\n");
      g_fprintf(stderr, "Sender control name: %s\n", scname);
      exit(EXIT_FAILURE);

    }

    /* set receiver control */

    sr->receiver = dg_get_control_by_name(dg->ctls, rcname);

    /* ensure send receive has a receiver control */

    if (!sr->receiver) {

      g_fprintf(stderr, "Data GUI error: receiver control does not exist.\n");
      g_fprintf(stderr, "Receiver control name: %s\n", rcname);
      exit(EXIT_FAILURE);

    }

    /* retrieve children */

    node = node->children;

    /* process children */

    while (node) {

      /* check whether current node is a maplet element */

      if (xmlStrcmp(node->name, "maplet") == 0) {

        /* create new maplet */

        DgMaplet *maplet = g_new(DgMaplet, 1);

        /* retrieve sender field name */

        gchar *sfname = (gchar *) xmlGetProp(node, "sfname");

        /* ensure sender field name was retrieved successfully */

        if (!sfname) {

          g_fprintf(stderr, "Data GUI error: no sender field name specified for send receive maplet.\n");
          g_fprintf(stderr, "Sender control name: %s\n", sr->sender->cname);
          exit(EXIT_FAILURE);

        }

        /* retrieve receiver field name */

        gchar *rfname = (gchar *) xmlGetProp(node, "rfname");

        /* ensure receiver field name was retrieved successfully */

        if (!rfname) {

          g_fprintf(stderr, "Data GUI error: no receiver field name specified for send receive maplet.\n");
          g_fprintf(stderr, "Sender control name: %s\n", sr->receiver->cname);
          exit(EXIT_FAILURE);

        }

        /* retrieve sender field name */

        maplet->sfield = get_field_by_name(sr->sender->extr->fields, sfname);

        /* ensure sender field was found */

        if (!maplet->sfield) {

          g_fprintf(stderr, "Data GUI error: send receive maplet send field not found.\n");
          g_fprintf(stderr, "Sender control name: %s\n", sr->sender->cname);
          g_fprintf(stderr, "Sender field name: %s\n", sfname);
          exit(EXIT_FAILURE);

        }

        /* retrieve receiver field name */

        maplet->rfield = get_field_by_name(sr->receiver->extr->fields, rfname);

        /* ensure receiver field was found */

        if (!maplet->rfield) {

          g_fprintf(stderr, "Data GUI error: send receive maplet receive field not found.\n");
          g_fprintf(stderr, "Receiver control name: %s\n", sr->receiver->cname);
          g_fprintf(stderr, "Receiver field name: %s\n", rfname);
          exit(EXIT_FAILURE);

        }

        /* keep track of new maplet */

        sr->maplets = g_list_append(sr->maplets, (gpointer) maplet);

      }

      /* check whether current node is a messenger element */

      else if (xmlStrcmp(node->name, "messenger") == 0) {

        /* retrieve messenger widget name */

        gchar *name = (gchar *) xmlGetProp(node, "name");

        /* retrieve and set messenger widget */

        sr->messenger = GTK_BUTTON(glade_xml_get_widget(dg->xml, name));

      }

      /* proceed to process next sibling node */

      node = node->next;

    }

    /* keep track of new send receive */

    dg->srs = g_list_append(dg->srs, (gpointer) sr);

  }

}

/* carry out steps needed to finalize data gui structure */

static void finalize(DgXML *dg) {

  DgControls *ctls;

  /* iterate through all controls */

  for (ctls = g_list_first(dg->ctls); ctls; ctls = ctls->next) {

    /* retrieve control */

    DgControl *ctl = (DgControl *) ctls->data;

    /* setup master detail relationships */

    DgMasterDetails *mds;

    /* iterate through master details */

    for (mds = g_list_first(dg->mds); mds; mds = mds->next) {

      /* retrieve master detail */

      DgMasterDetail *md = (DgMasterDetail *) mds->data;

      /* check whether current control matches master control */

      if (ctl == md->master) {

        /* set detail control for current control */

        ctl->details = g_list_append(ctl->details, (gpointer) md->detail);

      /* check whether current control matches detail control */

      } else if (ctl == md->detail) {

        /* ensure current control does not already have a master */

        if (ctl->master) {

          g_fprintf(stderr, "Data GUI error: detail has duplicate master.\n");
          g_fprintf(stderr, "Detail name: %s\n", ctl->cname);
          g_fprintf(stderr, "Master name: %s\n", ctl->cname);
          g_fprintf(stderr, "Master name: %s\n", md->master->cname);
          exit(EXIT_FAILURE);

        }

        /* set master control for current control */

        ctl->master = md->master;

      }

    }

    /* register callbacks for current control selector signals */

    g_signal_connect(G_OBJECT(ctl->sel->clist),
      "select-row", G_CALLBACK(dg_callback_row_selected), ctl);

    /* register callbacks for current control executor signals */

    if (ctl->manip && ctl->manip->exec) {

      g_signal_connect(G_OBJECT(ctl->manip->exec->atb),
        "toggled", G_CALLBACK(dg_callback_a_clicked), ctl);

      g_signal_connect(G_OBJECT(ctl->manip->exec->mtb),
        "toggled", G_CALLBACK(dg_callback_m_clicked), ctl);

      g_signal_connect(G_OBJECT(ctl->manip->exec->rtb),
        "toggled", G_CALLBACK(dg_callback_r_clicked), ctl);

      g_signal_connect(G_OBJECT(ctl->manip->exec->stb),
        "toggled", G_CALLBACK(dg_callback_s_clicked), ctl);

      g_signal_connect(G_OBJECT(ctl->manip->exec->ctb),
        "toggled", G_CALLBACK(dg_callback_c_clicked), ctl);

    }

  }


  /* setup send receive relationships */

  DgMasterDetails *srs;

  /* iterate through send receives */

  for (srs = g_list_first(dg->srs); srs; srs = srs->next) {

    /* retrieve send receive */

    DgSendReceive *sr = (DgSendReceive *) srs->data;

    /* register callbacks for send receive signals */

    g_signal_connect(G_OBJECT(sr->messenger),
      "clicked", G_CALLBACK(dg_callback_messenger_clicked), sr);

  }

  /* ensure data gui connection is null as it should be initially */

  g_assert(dg->cnc == NULL);

  /* set data gui connection state to disconnected */

  dg_connection(dg, dg->cnc);

}

/* create new data gui structure from XML */

DgXML *dg_xml_new(gchar *filename, GladeXML *xml, GtkDialog *error_dialog, GtkLabel *error_label) {

  DgXML *dg;
  xmlDocPtr doc;
  xmlNodePtr node;

  /* allocate data gui structure */

  dg = g_new0(DgXML, 1);

  /* set data gui structure glade XML structure */

  dg->xml = xml;

  /* set error dialog */

  dg->error_dialog = error_dialog;

  /* set error label */

  dg->error_label = error_label;

  /* retrieve document */

  if ((doc = xmlParseFile(filename)) == NULL) {
    g_fprintf(stderr, "Data GUI error: could not parse document.\n");
    exit(EXIT_FAILURE);
  }

  /* retrieve document root */

  if ((node = xmlDocGetRootElement(doc)) == NULL) {
    g_fprintf(stderr, "Data GUI error: could not retrieve root element.\n");
    exit(EXIT_FAILURE);
  }

  /* retrieve root element */

  if (xmlStrcmp(node->name, "datagui") != 0) {
    g_fprintf(stderr, "Data GUI error: could not retrieve datagui element.\n");
    exit(EXIT_FAILURE);
  }

  /* retrieve children */

  node = node->children;

  /* process children */

  while (node) {

    /* check whether current node is a control element */

    if (xmlStrcmp(node->name, "control") == 0) {

      /* parse control element */

      parse_control(node, dg);

    }

    /* check whether current node is a clone control element */

    else if (xmlStrcmp(node->name, "clonecontrol") == 0) {

      /* let this node be parsed later */

      clXmlNodes = g_list_append(clXmlNodes, (gpointer) node);

    }

    /* check whether current node is a master detail element */

    else if (xmlStrcmp(node->name, "masterdetail") == 0) {

      /* let this node be parsed later */

      mdXmlNodes = g_list_append(mdXmlNodes, (gpointer) node);

    }

    /* check whether current node is a send receive element */

    else if (xmlStrcmp(node->name, "sendreceive") == 0) {

      /* let this node be parsed later */

      srXmlNodes = g_list_append(srXmlNodes, (gpointer) node);

    }

    /* proceed to process next sibling node */

    node = node->next;

  }

  /* steps which need be carried out to set up clone controls */

  setup_clonecontrols(dg, clXmlNodes);

  /* steps which need be carried out to set up master details */

  setup_masterdetails(dg, mdXmlNodes);

  /* steps which need be carried out to set up send receives */

  setup_sendreceives(dg, srXmlNodes);

  /* steps unrelated to parsing which need to be carried out */

  finalize(dg);

  /* return data gui structure */

  return dg;

}
