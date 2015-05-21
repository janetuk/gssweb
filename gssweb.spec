%include %{_rpmconfigdir}/macros.mozilla
Name:		gssweb
Version:	0.1
Release:	1%{?dist}
Summary:	GSS-API for Javascript and the Web

Group:		Security Tools
License:	BSD
URL:		http://www.project-moonshot.org/
Source0:	gssweb-%{version}.tar.gz
BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root

BuildRequires:	 openssl-devel, cppunit-devel, jansson-devel, krb5-devel, glib-devel
BuildRequires:  mozilla-build
Requires:	moonshot-ui



%description
This includes the json GSSAPI executables and libraries.

%package -n firefox-gssweb
Summary: Firefox Support for GssWeb
Arch: noarch


%prep
%setup -q -n gssweb


%build
%cmake
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
%xpi_unpack -f browsers/firefox/gssweb.xpi
%xpi_symlink -a %{firefox_app_id}


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%{_libdir}/libjsongssapi.so
 %{_libdir}/libjsongssapi.so.*
%{_bindir}/json_gssapi
%files -n firefox-gssweb
%defattr(-,root,root,-)
%{_firefox_extdir}/
%{_mozilla_common_extdir}/%{name}/



%changelog

