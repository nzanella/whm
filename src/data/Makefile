# Makefile for Warehouse Manager documentation.

TARG = manual.html
DBSS = /usr/share/sgml/docbook/xsl-stylesheets

manual.html: manual.xml
	xsltproc -o ${TARG}.html ${DBSS}/html/docbook.xsl ${TARG}.xml

clean:
	rm -f whm.dia~ whm.glade.bak whm.gladep whm.gladep.bak

