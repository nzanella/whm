# Toplevel Makefile for whm.

version=1.1.88

topdir=/usr/src/redhat
prefix=/usr

all: src/whm data

src/whm:
	cd src; make MODEFLAGS=""

.PHONY: install

install: src/whm
	install -D -m 755 -o root -g root src/whm "$(prefix)/bin/whm"
	install -D -m 644 -o root -g root src/data/whm.desktop "$(prefix)/share/applications/whm.desktop"
	install -D -m 644 -o root -g root src/data/data.sql "$(prefix)/share/whm/data.sql"
	install -D -m 644 -o root -g root src/data/datagui.xml "$(prefix)/share/whm/datagui.xml"
	install -D -m 644 -o root -g root src/data/whm.glade "$(prefix)/share/whm/whm.glade"
	install -D -m 644 -o root -g root icons/whm.xpm "$(prefix)/share/icons/whm.xpm"
	install -D -m 644 -o root -g root icons/whm/walter-the-walrus.png "$(prefix)/share/icons/whm/walter-the-walrus.png"
	install -D -m 644 -o root -g root icons/whm/* "$(prefix)/share/icons/whm/"
	install -D -m 644 -o root -g root src/data/whm-C.omf "$(prefix)/share/omf/whm/whm-C.omf"
	install -D -m 644 -o root -g root src/data/datagui-1.0.dtd "$(prefix)/share/xml/datagui/datagui-1.0.dtd"
	install -D -m 644 -o root -g root src/data/whm.dia "$(prefix)/share/doc/whm/whm.dia"
	install -D -m 644 -o root -g root src/data/manual.html "$(prefix)/share/doc/whm/manual.html"
	install -D -m 644 -o root -g root src/data/manual.xml "$(prefix)/share/doc/whm/manual.xml"
	install -D -m 644 -o root -g root COPYING "$(prefix)/share/doc/whm/COPYING"
	install -D -m 644 -o root -g root CREDITS "$(prefix)/share/doc/whm/CREDITS"
	install -D -m 644 -o root -g root INSTALL "$(prefix)/share/doc/whm/INSTALL"
	install -D -m 644 -o root -g root README "$(prefix)/share/doc/whm/README"

.PHONY: uninstall

uninstall:
	rm -f "$(prefix)/bin/whm"
	rm -f "$(prefix)/share/applications/whm.desktop"
	rm -f "$(prefix)/share/icons/whm.xpm"
	rm -rf "$(prefix)/share/whm"
	rm -rf "$(prefix)/share/icons/whm"
	rm -rf "$(prefix)/share/omf/whm"
	rm -rf "$(prefix)/share/xml/datagui"
	rm -rf "$(prefix)/share/doc/whm"

.PHONY: installrpm

rpminstall: rpm
	cd /usr/src/redhat/RPMS/i386; rpm -e whm; rpm -Uvh whm-$(version)-1.fc3.i386.rpm

.PHONY: rpm

rpm: pack
	cp -f dist/whm.spec $(topdir)/SPECS/whm-$(version).spec
	cp ../whm-$(version).tar.gz $(topdir)/SOURCES/whm-$(version).tar.gz
	cp -f icons/whm.xpm $(topdir)/SOURCES/whm.xpm
	rpmbuild --define "_topdir $(topdir)" -ba "$(topdir)/SPECS/whm-$(version).spec"

.PHONY: pack

pack: clean version data
	cd ..; rm -f whm-$(version).tar.gz; tar -zcvf whm-$(version).tar.gz whm

.PHONY: clean

clean:
	cd src; make clean
	cd src/data; make clean

.PHONY: version

version:
	sed 's/Version:.*/Version: $(version)/' ./dist/whm.spec > ./dist/whm.spec.new
	mv -f ./dist/whm.spec.new ./dist/whm.spec
	sed 's/\#define PACKAGE_VERSION .*/\#define PACKAGE_VERSION "$(version)"/' ./src/config.h > ./src/config.h.new
	mv -f ./src/config.h.new ./src/config.h

.PHONY: data

data:
	cd src/data; make
