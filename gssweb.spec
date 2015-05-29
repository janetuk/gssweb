%global _firefox_app_id \{ec8030f7-c20a-464f-9b0e-13a3a9e97384\}

# common macros, yet to be defined. see:
# https://fedoraproject.org/wiki/User:Kalev/MozillaExtensionsDraft
%global _moz_extensions %{_datadir}/mozilla/extensions
%global _firefox_extdir %{_moz_extensions}/%{_firefox_app_id}

# needed for this package
%global extension_id gssweb@painless-security.com
%global _our_extdir %{_firefox_extdir}/%{extension_id}
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
BuildRequires:  unzip
Requires:	moonshot-ui



%description
This includes the json GSSAPI executables and libraries.

%package -n firefox-gssweb
Summary: Firefox Support for GssWeb
Requires: mozilla-filesystem, gssweb

%description -n firefox-gssweb
An extension to support gssweb for Firefox web browser.


%prep
%setup -q -n gssweb


%build
%cmake
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
     install -d $RPM_BUILD_ROOT/%{_our_extdir}
xpi=`pwd`/browsers/firefox/gssweb.xpi&&(cd $RPM_BUILD_ROOT/%{_our_extdir}&&unzip $xpi)



%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
%{_libdir}/libjsongssapi.so
 %{_libdir}/libjsongssapi.so.*
%{_bindir}/json_gssapi
%files -n firefox-gssweb
%defattr(-,root,root,-)
%{_our_extdir}



%changelog

