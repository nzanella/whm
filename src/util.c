#include "util.h"

GdaValueType util_get_type_from_name(gchar *typename) {

  if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_NULL") == 0)
    return GDA_VALUE_TYPE_NULL;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_BIGINT") == 0)
    return GDA_VALUE_TYPE_BIGINT;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_BIGUINT") == 0)
    return GDA_VALUE_TYPE_BIGUINT;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_BINARY") == 0)
    return GDA_VALUE_TYPE_BINARY;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_BLOB") == 0)
    return GDA_VALUE_TYPE_BLOB;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_BOOLEAN") == 0)
    return GDA_VALUE_TYPE_BOOLEAN;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_DATE") == 0)
    return GDA_VALUE_TYPE_DATE;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_DOUBLE") == 0)
    return GDA_VALUE_TYPE_DOUBLE;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_GEOMETRIC_POINT") == 0)
    return GDA_VALUE_TYPE_GEOMETRIC_POINT;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_GOBJECT") == 0)
    return GDA_VALUE_TYPE_GOBJECT;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_INTEGER") == 0)
    return GDA_VALUE_TYPE_INTEGER;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_LIST") == 0)
    return GDA_VALUE_TYPE_LIST;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_MONEY") == 0)
    return GDA_VALUE_TYPE_MONEY;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_NUMERIC") == 0)
    return GDA_VALUE_TYPE_NUMERIC;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_SINGLE") == 0)
    return GDA_VALUE_TYPE_SINGLE;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_SMALLINT") == 0)
    return GDA_VALUE_TYPE_SMALLINT;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_SMALLUINT") == 0)
    return GDA_VALUE_TYPE_SMALLUINT;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_STRING") == 0)
    return GDA_VALUE_TYPE_STRING;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_TIME") == 0)
    return GDA_VALUE_TYPE_TIME;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_TIMESTAMP") == 0)
    return GDA_VALUE_TYPE_TIMESTAMP;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_TINYINT") == 0)
    return GDA_VALUE_TYPE_TINYINT;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_TINYUINT") == 0)
    return GDA_VALUE_TYPE_TINYUINT;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_TYPE") == 0)
    return GDA_VALUE_TYPE_TYPE;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_UINTEGER") == 0)
    return GDA_VALUE_TYPE_UINTEGER;
  else if (g_ascii_strcasecmp(typename, "GDA_VALUE_TYPE_UNKNOWN") == 0)
    return GDA_VALUE_TYPE_UNKNOWN;
  else {
    g_printf("Unknown type name: %s.", typename);
    exit(EXIT_FAILURE);
  }

}
