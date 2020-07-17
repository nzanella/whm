#ifndef CONFIG_H
#define CONFIG_H

/* package name */

#define PACKAGE "whm"

/* package version number */

#define PACKAGE_VERSION "1.1.88"

/* full package name */

#define PACKAGE_NAME "Warehouse Manager"

/* address where package bug reports should be sent */

#define PACKAGE_BUGREPORT "nzanella@users.sourceforge.net"

/* path to Glade file */

#ifndef MAINTAINER_MODE
#define GLADE_FILE "/usr/share/whm/whm.glade"
#else
#define GLADE_FILE "data/whm.glade"
#endif

/* path to Data GUI file */

#ifndef MAINTAINER_MODE
#define DATAGUI_FILE "/usr/share/whm/datagui.xml"
#else
#define DATAGUI_FILE "data/datagui.xml"
#endif

/* path to message catalogue */

#define LOCALEDIR "/usr/share/locale"

#endif // CONFIG_H
