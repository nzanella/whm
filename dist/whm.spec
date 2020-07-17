# Warehouse Manager RPM spec file.

Summary: A warehouse management system.
Name: whm
Version: 1.1.88
Release: 1.fc3
License: GPL
Icon: whm.xpm
Group: Office/Productivity
Source: ftp://ftp.sourceforge.net/pub/sourceforge/w/wh/whm/%{name}-%{version}.tar.gz
URL: http://whm.sf.net/
Distribution: Fedora Core 3
Vendor: Neil Zanella <nzanella@users.sourceforge.net>
Packager: Neil Zanella <nzanella@users.sourceforge.net>
BuildRoot: %{_tmppath}/%{name}-%{version}-root
BuildRequires: gtk2-devel libglade2-devel libxml2-devel libgda-devel libxslt
Requires: gtk2 libglade2 libxml2 libgda

%description
Warehouse Manager is a business application designed to allow you to manage
several aspects of your warehouse based enterprise from your computer.

%prep
%setup -n %{name}

%build
%{__make}

%install
%{__rm} -rf "%{buildroot}"
%{makeinstall}

%clean
%{__rm} -rf "%{buildroot}"

%files
%defattr(-, root, root)
%{_bindir}/whm
%dir %{_datadir}/whm
%dir %{_datadir}/icons/whm
%dir %{_datadir}/omf/whm
%dir %{_datadir}/xml/datagui
%dir %{_docdir}/whm
%{_datadir}/applications/whm.desktop
%{_datadir}/whm/data.sql
%{_datadir}/whm/datagui.xml
%{_datadir}/whm/whm.glade
%{_datadir}/icons/whm.xpm
%{_datadir}/icons/whm/*
%{_datadir}/omf/whm/whm-C.omf
%{_datadir}/xml/datagui/datagui-1.0.dtd
%{_docdir}/whm/whm.dia
%{_docdir}/whm/manual.html
%{_docdir}/whm/manual.xml
%{_docdir}/whm/COPYING
%{_docdir}/whm/CREDITS
%{_docdir}/whm/INSTALL
%{_docdir}/whm/README

%changelog
* Wed Oct 20 2004 Neil Zanella <nzanella@users.sourceforge.net>
- initial release
