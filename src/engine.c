#include "datagui.h"

/* create and initialize new control breadth first search directed acyclic graph iterator */

static DgControlIter *control_iter_new(DgControl *control) {

  /* create new breadth first search directed acyclic graph iterator */

  DgControlIter *iter = g_new(DgControlIter, 1);

  /* set control node for breadth first search directed acyclic graph iterator */

  iter->ctl = control;

  /* create new fringe for breadth first search directed acyclic graph iterator */

  iter->fringe = g_queue_new();

  /* return breadth first search directed acyclic graph iterator */

  return iter;

}

/* retrieve next control breadth first search directed acyclic graph iterator */

static DgControlIter *control_iter_next(DgControlIter *iter) {

  DgControls *dctls;

  /* iterate through all detail controls */

  for (dctls = g_list_first(iter->ctl->details); dctls; dctls = dctls->next) {

    /* retrieve detail control node */

    DgControl *dctl = (DgControl *) dctls->data;

    /* push detail control node onto fringe tail */

    g_queue_push_tail(iter->fringe, (gpointer) dctl);

  }

  /* set current control node to control node at head of fringe */

  iter->ctl = g_queue_is_empty(iter->fringe) ? NULL : (DgControl *) g_queue_peek_head(iter->fringe);

  /* pop control node at head of fringe */

  g_queue_pop_head_link(iter->fringe);

  /* return current breadth first search directed acyclic graph iterator */

  return iter;

}

/* return an SQL quoted version of the given string */

static gchar *sql_quote(gchar *strval) {

  GString *string = g_string_new(strval);

  gsize i;

  for (i = 0; i < string->len; i++)

    if (string->str[i] == '\'')

      g_string_insert_c(string, i++, '\'');

  return g_strconcat("'", string->str, "'", NULL);

}

/* set sensitive property for all exposer widgets */

static void exposers_set_sensitive(DgExposers *exposers, gboolean sensitive) {

  DgExposers *exps;

  /* iterate through exposers corresponding to selected row */

  for (exps = g_list_first(exposers); exps; exps = exps->next) {

    /* retrieve exposer */

    DgExposer *exp = (DgExposer *) exps->data;

    /* set widget sensitivity */

    gtk_widget_set_sensitive(exp->widget, sensitive);

  }

}

/* set editable property for all exposer widgets */

static void exposers_set_editable(DgExposers *exposers, gboolean editable) {

  DgExposers *exps;

  /* iterate through exposers corresponding to selected row */

  for (exps = g_list_first(exposers); exps; exps = exps->next) {

    /* retrieve exposer */

    DgExposer *exp = (DgExposer *) exps->data;

    /* set widget editability */

    if (GTK_IS_ENTRY(exp->widget) || GTK_IS_TEXT_VIEW(exp->widget))

      g_object_set(G_OBJECT(exp->widget), "editable", editable, NULL);

  }

}

/* set cursor position of all exposer widgets to home position */

static void exposers_set_home_position(DgExposers *exposers) {

  DgExposers *exps;

  /* iterate through exposers corresponding to selected row */

  for (exps = g_list_first(exposers); exps; exps = exps->next) {

    /* retrieve exposer */

    DgExposer *exp = (DgExposer *) exps->data;

    /* set home position */

    if (GTK_IS_EDITABLE(exp->widget))

      gtk_editable_set_position(GTK_EDITABLE(exp->widget), 0);

  }

}

/* allow first exposer widget to grab the input focus */

static void exposers_grab_focus(DgExposers *exposers) {

  /* retrieve first exposer */

  DgExposer *exp = (DgExposer *) g_list_first(exposers)->data;

  /* give focus to first exposer widget */

  gtk_widget_grab_focus(exp->widget);

}

/* set text for exposer widget */

static void exposer_set_text(GtkWidget *widget, const gchar *text) {

  if (GTK_IS_ENTRY(widget)) {

    gtk_entry_set_text(GTK_ENTRY(widget), text);

  }

  else if (GTK_IS_TEXT_VIEW(widget)) {

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));

    gtk_text_buffer_set_text(buffer, text, -1);

  }

  else if (GTK_IS_SPIN_BUTTON(widget)) {

//if (text) {
//g_printf("hello: %s\n", text);
//    gdouble value = g_ascii_strtod(text, NULL);

//printf("value: %lf\n", value);

//    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), value);

//if (value == 0)
//puts("a");

//else
//puts("b");
//}

  }

}

/* clear all exposer widgets */

static void exposers_clear(DgExposers *exposers) {

  DgExposers *exps;

  /* iterate through exposers corresponding to selected row */

  for (exps = g_list_first(exposers); exps; exps = exps->next) {

    /* retrieve exposer */

    DgExposer *exp = (DgExposer *) exps->data;

    /* clear exposer text */

    exposer_set_text(exp->widget, "");

  }

}

/* set exposers text content from control node extractors */

static void exposers_populate(DgControl *control) {

  gint j;

  DgExposers *exps;

  /* iterate through result set row fields */

  for (j = 0; j < gda_data_model_get_n_columns(control->dm); j++) {

    /* retrieve result set field attributes */

    GdaFieldAttributes *attrs = gda_data_model_describe_column(control->dm, j);

    /* retrieve column field name from result set field attributes */

    const gchar *name = gda_field_attributes_get_name(attrs);

    /* retrieve column value from result set field attributes */

    const GdaValue *value = gda_data_model_get_value_at(control->dm, j, control->rownum);

    /* convert column value to string */

    gchar *str = value->type == GDA_VALUE_TYPE_NULL ? "" : gda_value_stringify(value);

    /* iterate through exposers corresponding to selected row */

    for (exps = g_list_first(control->manip->exps); exps; exps = exps->next) {

      /* retrieve exposer */

      DgExposer *exp = (DgExposer *) exps->data;

      /* check whether exposer name matches field name */

      if (g_ascii_strcasecmp(exp->field->name, name) == 0) {

        /* retrieve exposer widget */

        GtkWidget *widget = (GtkWidget *) exp->widget;

        /* set exposer widget text accordingly */

        exposer_set_text(widget, str);

      }

    }

  }

}

/* get field value from exposer */

static gchar *exposer_get_field_value(DgControl *control, gchar *fname, gchar *emptyval) {

  DgControl *ctl;

  /* iterate through current control node and its ancestral master control nodes */

  for (ctl = control; ctl; ctl = ctl->master) {

    DgExposers *exps;

    /* iterate through exposers corresponding to selected row */

    for (exps = g_list_first(ctl->manip->exps); exps; exps = exps->next) {

      /* retrieve exposer */

      DgExposer *exp = (DgExposer *) exps->data;

      /* check whether exposer field name matches sought field name */

      if (g_ascii_strcasecmp(exp->field->name, fname) == 0) {

        gchar *strval;

        /* retrieve exposer widget */

        GtkWidget *widget = (GtkWidget *) exp->widget;

        /* retrieve widget text */

        if (GTK_IS_ENTRY(widget)) {

          strval = (gchar *) gtk_entry_get_text(GTK_ENTRY(widget));

        }

        else if (GTK_IS_TEXT_VIEW(widget)) {

          GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(widget));

          GtkTextIter start, end;

          gtk_text_buffer_get_bounds(buffer, &start, &end);

          strval = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);

        }

        else if (GTK_IS_SPIN_BUTTON(widget)) {

          gdouble value = gtk_spin_button_get_value(GTK_SPIN_BUTTON(widget));

          gchar *text = (gchar *) g_malloc(100);

          strval = g_ascii_dtostr(text, 100, value);

        }

        /* check whether widget text is empty */

        if (g_ascii_strcasecmp(strval, "") == 0) {

          /* return user supplied empty value */

          return emptyval;

        }

        else {

          /* retrieve type to determine whether to quote string */

          GdaValueType type = exp->field->type;

          if (type == GDA_VALUE_TYPE_STRING || type == GDA_VALUE_TYPE_DATE)

            return sql_quote(strval);

        }

      }

    }

  }

  /* exposer field name not found in current and ancestral master controls */

  return emptyval;

}

/* get field value from extractor */

static gchar *extractor_get_field_value(DgControl *ctl, gchar *fname, gboolean quoted, gchar *emptyval) {

  int j;

  /* return empty value in case no row has been selected */

  if (ctl->rownum == -1)

    return emptyval;

  /* iterate through result set row fields */

  for (j = 0; j < gda_data_model_get_n_columns(ctl->dm); j++) {

    /* retrieve result set field attributes */

    GdaFieldAttributes *attrs = gda_data_model_describe_column(ctl->dm, j);

    /* retrieve column field name from result set field attributes */

    const gchar *name = gda_field_attributes_get_name(attrs);

    if (g_ascii_strcasecmp(fname, name) == 0) {

      /* retrieve column value from result set field attributes */

      const GdaValue *value = gda_data_model_get_value_at(ctl->dm, j, ctl->rownum);

      /* convert column value to string */

      gchar *strval = gda_value_stringify(value);

      /* retrieve type to determine whether to quote the string */

      GdaValueType type = value->type;

      if (type == GDA_VALUE_TYPE_STRING || type == GDA_VALUE_TYPE_DATE)

        return quoted ? sql_quote(strval) : strval;

      else if (type == GDA_VALUE_TYPE_NULL)

        return quoted ? emptyval : "";

      else

        return strval;

    }

  }

  g_printf("Data GUI error: undeclared control query field name.\n");
  g_printf("Control name: %s\n", ctl->cname);
  g_printf("Field name: %s\n", fname);
  exit(EXIT_FAILURE);

}

#define CTL_PREFIX "$ctl."
#define NEW_PREFIX "$new."
#define OLD_PREFIX "$old."

static gchar *eval_query(DgControl *control, const gchar *query, gchar *emptyval) {

  GString *result = g_string_new("");

  gint pos;

  for (pos = 0; pos < g_utf8_strlen(query, -1); pos++) {

    if (g_ascii_strncasecmp(query + pos, CTL_PREFIX, g_utf8_strlen(CTL_PREFIX, -1)) == 0) {

      pos += g_utf8_strlen(CTL_PREFIX, -1) - 1;

      GString *cname = g_string_new("");

      while (g_ascii_isalpha(query[++pos]) || query[pos] == '_')

        g_string_append_c(cname, query[pos]);

      g_assert(query[pos] == '.');

      GString *fname = g_string_new("");

      while (g_ascii_isalpha(query[++pos]))

        g_string_append_c(fname, query[pos]);

      DgControl *ctl = dg_get_control_by_name(control->dg->ctls, cname->str);

      g_assert(ctl);

      g_string_append(result, extractor_get_field_value(ctl, fname->str, TRUE, emptyval));

      g_string_append_c(result, query[pos]);

      g_string_free(fname, TRUE);

    }

    else if (g_ascii_strncasecmp(query + pos, NEW_PREFIX, g_utf8_strlen(NEW_PREFIX, -1)) == 0) {

      pos += g_utf8_strlen(NEW_PREFIX, -1) - 1;

      GString *fname = g_string_new("");

      while (g_ascii_isalpha(query[++pos]))

        g_string_append_c(fname, query[pos]);

      g_string_append(result, exposer_get_field_value(control, fname->str, emptyval));

      g_string_append_c(result, query[pos]);

      g_string_free(fname, TRUE);

    }

    else if (g_ascii_strncasecmp(query + pos, OLD_PREFIX, g_utf8_strlen(OLD_PREFIX, -1)) == 0) {

      pos += g_utf8_strlen(OLD_PREFIX, -1) - 1;

      GString *fname = g_string_new("");

      while (g_ascii_isalpha(query[++pos]))

        g_string_append_c(fname, query[pos]);

      g_string_append(result, extractor_get_field_value(control, fname->str, TRUE, emptyval));

      g_string_append_c(result, query[pos]);

      g_string_free(fname, TRUE);

    }

    else

      g_string_append_c(result, query[pos]);

  }

puts(result->str);

  return result->str;

}

/* retrieve result set from control query */

static GdaDataModel *get_result_set(DgControl *control, const gchar *query) {

  /* create command to be executed */

  GdaCommand *cmd = gda_command_new(eval_query(control, query, "NULL"),
    GDA_COMMAND_TYPE_SQL, GDA_COMMAND_OPTION_STOP_ON_ERRORS);

  /* check that command was created successfully */

  g_assert(cmd);

  /* execute query and retrieve data model result set */

  GdaDataModel *dm = gda_connection_execute_single_command(control->dg->cnc, cmd, NULL);

  /* ensure that result set was retrieved successfully */

  if (!dm) {

    g_printf("Error encountered while executing query:\n");
    g_printf("Control name: %s\n", control->cname);
    g_printf("Query: %s\n", gda_command_get_text(cmd));
    exit(EXIT_FAILURE);

  }

  /* free command */

  gda_command_free(cmd);

  /* return result set */

  return dm;

}

static void reset_control(DgControl *control) {

  /* display control state */

  control->state = DG_NONE;

  /* reset selection row number */

  control->rownum = -1;

  /* free current data model */

  g_free(control->dm);

  /* reset current data model */

  control->dm = NULL;

}

/* check whether control has active ancestral masters */

static gboolean has_active_ancestor_master(DgControl *control) {

  DgControl *ctl;

  /* iterate through ancestral master controls */

  for (ctl = control->master; ctl; ctl = ctl->master)

    /* check whether ancestral master control is active */

    if (ctl->state != DG_NONE)

      /* control has an active ancestral master */

      return TRUE;

  /* control has no active ancestral master */

  return FALSE;

}

static void display_control(DgControl *control) {

if (g_ascii_strcasecmp(control->cname, "sup_suppliers") == 0) {

  g_printf("control: %s\n", control->cname);
  g_printf("row id: %d\n", control->rownum);

}

  /* set columned list sensitive property */

  gtk_widget_set_sensitive(GTK_WIDGET(control->sel->clist),
    control->dg->cnc != NULL && control->state == DG_NONE);

  /* check whether manipulators exist */

  if (control->manip) {

    /* check whether control should have no selections */
    /* or one of its ancestral masters is active */
    /* or whether control is in the add state */

    if (control->rownum == -1 || has_active_ancestor_master(control) || control->state == DG_ADD) {

      /* unselect selected row provided such row exists */

      gtk_clist_unselect_all(control->sel->clist);

      /* clear exposers */

      exposers_clear(control->manip->exps);

    }

    /* check whether control should have selections */
    /* and none of its ancestral masters are active */

    else if (control->rownum != -1 && !has_active_ancestor_master(control)) {

      /* ensure columned list row supposed to appear selected is selected */

      gtk_clist_select_row(control->sel->clist, control->rownum, 0);

      /* populate exposers from selected data */

      exposers_populate(control);

    }

    /* set exposers sensitive properties */

    exposers_set_sensitive(control->manip->exps, control->dg->cnc != NULL);

    /* set exposers editable properties */

    exposers_set_editable(control->manip->exps,
      control->dg->cnc != NULL && (control->state == DG_ADD || control->state == DG_MODIFY));

    /* set exposers position to home position */

    exposers_set_home_position(control->manip->exps);

    /* check whether to set dates to their default values */

    if (control->state == DG_ADD) {

      ;//set_dates(ctl->manip->exps);

    }

    /* check whether to give focus to exposer widgets */

    if (control->state == DG_ADD || control->state == DG_MODIFY) {

      /* give focus to first exposer widget */

      exposers_grab_focus(control->manip->exps);

    }

    /* check whether executors exist */

    if (control->manip->exec) {

      /* block signals */

      g_signal_handlers_block_by_func(control->manip->exec->atb, dg_callback_a_clicked, control);
      g_signal_handlers_block_by_func(control->manip->exec->mtb, dg_callback_m_clicked, control);
      g_signal_handlers_block_by_func(control->manip->exec->rtb, dg_callback_r_clicked, control);
      g_signal_handlers_block_by_func(control->manip->exec->stb, dg_callback_s_clicked, control);
      g_signal_handlers_block_by_func(control->manip->exec->ctb, dg_callback_c_clicked, control);

      /* set toggle button active properties */

      gtk_toggle_button_set_active(control->manip->exec->atb, control->dg->cnc != NULL && control->state == DG_ADD);
      gtk_toggle_button_set_active(control->manip->exec->mtb, control->dg->cnc != NULL && control->state == DG_MODIFY);
      gtk_toggle_button_set_active(control->manip->exec->rtb, control->dg->cnc != NULL && control->state == DG_REMOVE);
      gtk_toggle_button_set_active(control->manip->exec->stb, FALSE);
      gtk_toggle_button_set_active(control->manip->exec->ctb, FALSE);

      /* set toggle button sensitive properties */

      gtk_widget_set_sensitive(GTK_WIDGET(control->manip->exec->atb),
        control->dg->cnc != NULL && (!control->master || control->master->rownum != -1) &&
        (control->state == DG_NONE || control->state == DG_ADD));
      gtk_widget_set_sensitive(GTK_WIDGET(control->manip->exec->mtb),
        control->dg->cnc != NULL && control->rownum != -1 &&
        (control->state == DG_NONE || control->state == DG_MODIFY));
      gtk_widget_set_sensitive(GTK_WIDGET(control->manip->exec->rtb),
        control->dg->cnc != NULL && control->rownum != -1 &&
        (control->state == DG_NONE || control->state == DG_REMOVE));
      gtk_widget_set_sensitive(GTK_WIDGET(control->manip->exec->stb),
        control->dg->cnc != NULL && control->state != DG_NONE);
      gtk_widget_set_sensitive(GTK_WIDGET(control->manip->exec->ctb),
        control->dg->cnc != NULL && control->state != DG_NONE);

      /* unblock signals */

      g_signal_handlers_unblock_by_func(control->manip->exec->atb, dg_callback_a_clicked, control);
      g_signal_handlers_unblock_by_func(control->manip->exec->mtb, dg_callback_m_clicked, control);
      g_signal_handlers_unblock_by_func(control->manip->exec->rtb, dg_callback_r_clicked, control);
      g_signal_handlers_unblock_by_func(control->manip->exec->stb, dg_callback_s_clicked, control);
      g_signal_handlers_unblock_by_func(control->manip->exec->ctb, dg_callback_c_clicked, control);

    }

  }

}

void refresh_control(DgControl *ctl) {

  gint num_rows, i, j;

  DgFields *flds;

  /* set old data model to current data model */

  GdaDataModel *old_dm = ctl->dm;

  /* set old number of rows to current number of rows */

  gint old_num_rows = ctl->dm ? gda_data_model_get_n_rows(ctl->dm) : 0;

  /* set old selected row number to current selected row number */

  gint old_rownum = ctl->rownum;

  /* retrieve result set from control using query */

  ctl->dm = get_result_set(ctl, ctl->extr->query);

  /* retrieve current number of rows */

  num_rows = gda_data_model_get_n_rows(ctl->dm);

  /* reset current control selected row number */

  ctl->rownum = -1;

  /* clear columned list */

  gtk_clist_clear(ctl->sel->clist);

  /* check whether an old data model exists */

  if (num_rows && old_dm) {

    /* search for row index of row to be selected */

    for (i = 0; ctl->rownum == -1 && i < MIN(old_num_rows, num_rows); i++)

      for (j = 0; ctl->rownum == -1 && j < gda_data_model_get_n_columns(ctl->dm); j++) {

        GdaValue *value = (GdaValue *) gda_data_model_get_value_at(ctl->dm, j, i);

        GdaValue *old_value = (GdaValue *) gda_data_model_get_value_at(old_dm, j, i);

        if (gda_value_compare(value, old_value) != 0)

          ctl->rownum = i;

      }

    if (ctl->rownum == -1) {

      if (num_rows > old_num_rows)

        ctl->rownum = num_rows - 1;

      else if (old_rownum != -1)

        ctl->rownum = old_rownum;

    }

  }

  /* iterate through result set rows to populate columned list */

  for (i = 0; i < gda_data_model_get_n_rows(ctl->dm); i++) {

    /* set aside space for columned list row text */

    gchar **text = g_new0(gchar *, g_list_length(ctl->sel->fields)); //the zero seems to fix a segfault...

    /* iterate through result set fields displayed by columned list */

    for (j = 0, flds = g_list_first(ctl->sel->fields); flds; flds = flds->next, j++) {

      DgField *fld = (DgField *) flds->data;

      /* retrieve position of columned list field in result set */

      gint pos = g_list_index(ctl->extr->fields, (gpointer) fld);

      /* ensure position of columned list field was found */

      g_assert(pos != -1);

      /* retrieve value from result set */

      GdaValue *value = (GdaValue *) gda_data_model_get_value_at(ctl->dm, pos, i);

      /* set converted string value */

      text[j] = value->type == GDA_VALUE_TYPE_NULL ? "" : gda_value_stringify(value);

      g_assert(text[j]);

    }

    /* append row of strings to result set */

    gtk_clist_append(ctl->sel->clist, text);

    /* release character strings used to populate columned list row */

    for (j = 0; j < g_list_length(ctl->sel->fields); j++)

      g_free(text[j]);

    /* deallocate space used to hold columned list row text */

    g_free(text);

  }

  /* clear old data model if old data model exists */

  if (old_dm) g_object_unref(old_dm);

  /* reset appearance of other user interface widgets */

  display_control(ctl);

}

/* retrieve top level master control for specified control */
/* ... unused
/fo
static DgControl *get_topmost_master(DgControl *control) {

  DgControl *master = control;

  if (master)

    while (master->master)

      master = master->master;

  return master;

}
*/

/* refresh all controls after modifying control which may be null on startup */

static void refresh_controls(DgControls *controls) {

  DgControls *ctls;

  /* iterate through all controls */

  for (ctls = g_list_first(controls); ctls; ctls = ctls->next) {

    /* retrieve control */

    DgControl *ctl = (DgControl *) ctls->data;

    /* ensure that control is not a descendent control */

    if (!ctl->master) {

      DgControlIter *iter;

      /* iterate through control and its descendent details */

      for (iter = control_iter_new(ctl); iter->ctl; iter = control_iter_next(iter)) {

        /* refresh control */

        refresh_control(iter->ctl);

      }

      g_free(iter);

    }

  }

}

void dg_callback_row_selected(GtkCList *clist, gint i, gint j, GdkEventButton *event, gpointer data) {

  /* retrieve control */

  DgControl *control = (DgControl *) data;

  /* block signals */

  g_signal_handlers_block_by_func(clist, dg_callback_row_selected, control);

  /* check whether new row number is different from old row number to guard against looping */

  if (i != control->rownum) {

    /* set control row number to selected row number */

    control->rownum = i;

    /* display control widgets to reflect state changes */

    display_control(control);

    /* populate columned lists pertaining to detail control nodes */

    DgControlIter *iter = control_iter_new(control);

    for (iter = control_iter_next(iter); iter->ctl; iter = control_iter_next(iter)) {

      /* retrieve and refresh detail control */

      refresh_control(iter->ctl);

    }

    g_free(iter);

  }

  /* unblock signals */

  g_signal_handlers_unblock_by_func(clist, dg_callback_row_selected, control);

}

void dg_connection(DgXML *dg, GdaConnection *cnc) {

  /* set database connection parameter */

  dg->cnc = cnc;

  /* check whether database connection exists */

  if (cnc != NULL) {

    /* refresh controls updating their result sets and displaying them */

    refresh_controls(dg->ctls);

  }

  /* database connection does not exist */

  else {

    /* reset controls */

    g_list_foreach(dg->ctls, (GFunc) reset_control, NULL);

    /* display controls */

    g_list_foreach(dg->ctls, (GFunc) display_control, NULL);

  }

}

/* display descendent detail control nodes */

static void display_descendent_details(DgControl *control) {

  /* iterate through and display descendent detail control nodes disposing of their state and selections */

  DgControlIter *iter = control_iter_new(control);

  for (iter = control_iter_next(iter); iter->ctl; iter = control_iter_next(iter)) {

    reset_control(iter->ctl);

    display_control(iter->ctl);

  }

  g_free(iter);

}

static gboolean execute_queries(DgControl *control, DgQueries *queries, gchar *error_message) {

  DgQueries *qs;

  /* iterate through queries to be executed */

  for (qs = g_list_first(queries); qs; qs = qs->next) {

    /* retrieve query to be executed */

    DgQuery *q = (DgQuery *) qs->data;

    /* execute constructed command */

    switch (q->type) {

      /* check whether query is used for branching based on result set emptyness */

      case DG_QUERY_TYPE_EMPTYTEST: {

        /* retrieve result set from control using query */

        GdaDataModel *dm = get_result_set(control, q->query);

        /* retrieve queries to be conditionally executed */

        DgQueries *queries = gda_data_model_get_n_rows(dm) ? q->nonemptycase : q->emptycase;

        /* execute queries and ensure queries are executed successfully */

        if (!execute_queries(control, queries, error_message))

          /* somewhere along the way an empty check failed so do not allow execution to proceed */

          return FALSE;

        break;

      }

      /* check whether query is used to ensure result set emptyness */

      case DG_QUERY_TYPE_EMPTYCHECK: {

        /* retrieve result set from control using query */

        GdaDataModel *dm = get_result_set(control, q->query);

        /* ensure emptyness of data model result set */

        if (gda_data_model_get_n_rows(dm) != 0) {

          /* display popup window with error message */

          gchar *message = g_strconcat("<big>", error_message, ":\n", q->error_message, "</big>", NULL);
          gtk_label_set_markup(control->dg->error_label, message);
          gtk_widget_show_all(GTK_WIDGET(control->dg->error_dialog));

          /* reset submit toggle button active properties */

          gtk_toggle_button_set_active(control->manip->exec->stb, FALSE);

          /* return status code to indicate empty check failure */

          return FALSE;

        }

        break;

      }

      /* check whether query is used to ensure result set nonemptyness */

      case DG_QUERY_TYPE_NONEMPTYCHECK: {

        /* retrieve result set from control using query */

        GdaDataModel *dm = get_result_set(control, q->query);

        /* ensure emptyness of data model result set */

        if (gda_data_model_get_n_rows(dm) == 0) {

          /* display popup window with error message */

          gchar *message = g_strconcat("<big>", error_message, ":\n", q->error_message, "</big>", NULL);
          gtk_label_set_markup(control->dg->error_label, message);
          gtk_widget_show_all(GTK_WIDGET(control->dg->error_dialog));

          /* reset submit toggle button active properties */

          gtk_toggle_button_set_active(control->manip->exec->stb, FALSE);

          /* return status code to indicate empty check failure */

          return FALSE;

        }

        break;

      }

      /* check whether query is used to ensure manipulate database */

      case DG_QUERY_TYPE_ORDINARY: {

        /* create command to be executed */

        GdaCommand *cmd = gda_command_new(eval_query(control, q->query, "DEFAULT"),
          GDA_COMMAND_TYPE_SQL, GDA_COMMAND_OPTION_STOP_ON_ERRORS);

        /* check that command was created successfully */

        g_assert(cmd);

        /* execute database manipulation query */

        if (gda_connection_execute_non_query(control->dg->cnc, cmd, NULL) == -1) {

          g_printf("Error encountered while executing query:\n");
          g_printf("Control name: %s\n", control->cname);
          g_printf("Query: %s\n", gda_command_get_text(cmd));
          exit(EXIT_FAILURE);

        }

        /* free command */

        gda_command_free(cmd);

        break;

      }

    }

  }

  /* return successfully */

  return TRUE;

}

/* ensure user has specified values for all required fields prior to submission */

static gboolean check_required_fields(DgControl *control) {

  DgFields *flds;

  /* iterate through all fields */

  for (flds = g_list_first(control->extr->fields); flds; flds = flds->next) {

    /* retrieve field */

    DgField *fld = (DgField *) flds->data;

    /* check whether field is required and has not been specified as nonempty */

    if (fld->required && g_ascii_strcasecmp(exposer_get_field_value(control, fld->name, ""), "") == 0) {

      /* display popup window with error message */

      gchar *message = g_strconcat("<big>", "Could not manipulate database:\nSome required fields were not specified.\n", "</big>", NULL);
      gtk_label_set_markup(control->dg->error_label, message);
      gtk_widget_show_all(GTK_WIDGET(control->dg->error_dialog));

      /* some required fields were not specified */

      return FALSE;

    }

  }

  /* all required fields were specified */

  return TRUE;

}

void dg_callback_a_clicked(GtkToggleButton *togglebutton, gpointer data) {

  /* retrieve control */

  DgControl *control = (DgControl *) data;

  /* set control state to add state */

  control->state = DG_ADD;

  /* display control */

  display_control(control);

  /* display descendent detail control nodes */

  display_descendent_details(control);

}

void dg_callback_m_clicked(GtkToggleButton *togglebutton, gpointer data) {

  /* retrieve control */

  DgControl *control = (DgControl *) data;

  /* set control state to modify state */

  control->state = DG_MODIFY;

  /* display control */

  display_control(control);

  /* display descendent detail control nodes */

  display_descendent_details(control);

}

void dg_callback_r_clicked(GtkToggleButton *togglebutton, gpointer data) {

  /* retrieve control */

  DgControl *control = (DgControl *) data;

  /* set control state to remove state */

  control->state = DG_REMOVE;

  /* display control */

  display_control(control);

  /* display descendent detail control nodes */

  display_descendent_details(control);

}

void dg_callback_s_clicked(GtkToggleButton *togglebutton, gpointer data) {

  /* retrieve control */

  DgControl *control = (DgControl *) data;

  /* whether submission was successful */

  gboolean successful = FALSE;

  /* ensure all required fields have been specified */

  if (check_required_fields(control)) {

    /* execute queries according to control state */

    switch (control->state) {

      case DG_ADD:

        if (execute_queries(control, control->manip->exec->inserter, "Could not insert record"))

          successful = TRUE;

        break;

      case DG_MODIFY:

        if (execute_queries(control, control->manip->exec->updater, "Could not update record"))

          successful = TRUE;

        break;

      case DG_REMOVE:

        if (execute_queries(control, control->manip->exec->deleter, "Could not delete record"))

          successful = TRUE;

        break;

      case DG_NONE:

        /* control should never reach here */

        g_assert(0);

        break;

    }

  }

  if (successful) {

    /* set control state to none state */

    control->state = DG_NONE;

    /* refresh controls updating their result sets and displaying controls */

    refresh_controls(control->dg->ctls);

  } else {

    /* block signals */

    g_signal_handlers_block_by_func(control->manip->exec->stb, dg_callback_s_clicked, control);

    /* reset submit toggle button active properties */

    gtk_toggle_button_set_active(control->manip->exec->stb, FALSE);

    /* unblock signals */

    g_signal_handlers_unblock_by_func(control->manip->exec->stb, dg_callback_s_clicked, control);

  }

}

void dg_callback_c_clicked(GtkToggleButton *togglebutton, gpointer data) {

  /* retrieve control */

  DgControl *control = (DgControl *) data;

  /* set control state to none state */

  control->state = DG_NONE;

  /* enable control */

  display_control(control);

  /* display descendent detail control nodes */

  display_descendent_details(control);

}

void dg_callback_messenger_clicked(GtkButton *messenger, gpointer data) {

  /* retrieve send receive */

  DgSendReceive *sr = (DgSendReceive *) data;

  /* retrieve receiver state */

  DgControlState state = sr->receiver->state;

  /* set receiver data to sender data */

  DgMaplets *mplts;

  /* ensure receiver is in add or modify state */

  if (state != DG_ADD && state != DG_MODIFY)

    /* in this case nothing should be done */

    return;

  /* iterate through send receive maplets */

  for (mplts = g_list_first(sr->maplets); mplts; mplts = mplts->next) {

    /* retrieve maplet */

    DgMaplet *mplt = (DgMaplet *) mplts->data;

    /* retrieve maplet sender text */

    gchar *text = extractor_get_field_value(sr->sender, mplt->sfield->name, FALSE, "NULL");

    /* set retrieved text in all receiver widgets which need to display it */

    DgExposers *exps;

    /* iterate through send receive exposers */

    for (exps = g_list_first(sr->receiver->manip->exps); exps; exps = exps->next) {

      /* retrieve exposer */

      DgExposer *exp = (DgExposer *) exps->data;

      /* check whether exposer field matches maplet receiver field */

      if (g_ascii_strcasecmp(exp->field->name, mplt->rfield->name) == 0) {

        /* set receiver maplet widget text */

        exposer_set_text(exp->widget, text);

      }

    }

  }

}
