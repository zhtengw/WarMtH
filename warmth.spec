Name:           warmth
Version:        1.2.1
Release:        1%{?dist}
Summary:        A Ruijie and Cernet supplicant
Summary(zh_CN.UTF-8): 锐捷和塞尔认证

Group:          Applications/Internet
License:        GPLv3
URL:            http://code.google.com/p/warmth/
Source0:        http://warmth.googlecode.com/files/warmth-%{version}.tar.xz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  glibc-devel libpcap-devel qt-devel automake autoconf 
Requires:       libpcap qt 

%description
This package contains a Ruijie and Cernet supplicant from HustMoon Studio.

%description -l zh_CN.UTF-8
WarMtH is a GUI front end of MentoHust, mentohust是用来进行锐捷和塞尔认证的。因为官方没有Linux版本或者Linux版本很不好用
^_^


%prep
%setup -q


%build
patch -Np0 -i mentohust-linebuf.patch
cd mentohust
sh autogen.sh
%configure --prefix=/usr \
  --libdir=/usr/lib${LIBDIRSUFFIX} \
  --mandir=/usr/man
make %{?_smp_mflags}
cd ..
mkdir build \
  && cd build \
     &&  cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DBUILD_SHARED_LIBS:BOOL=ON \
    && make
cd ..


%install
rm -rf $RPM_BUILD_ROOT
cd mentohust
make install DESTDIR=$RPM_BUILD_ROOT
cd ../build
make install DESTDIR=$RPM_BUILD_ROOT
cd ..
chmod 4755 $RPM_BUILD_ROOT/usr/bin/mentohust
mv $RPM_BUILD_ROOT%{_defaultdocdir}/mentohust/README $RPM_BUILD_ROOT%{_prefix}/doc/%{name}/README.mentohust
rm -r $RPM_BUILD_ROOT%{_defaultdocdir}
%find_lang %{name}

%clean
rm -rf $RPM_BUILD_ROOT

%files -f %{name}.lang
%defattr(-,root,root,-)
%attr(4755,root,root) %{_bindir}/mentohust
%{_bindir}/warmth
%{_prefix}/lib/*
%{_prefix}/share/*
%config /etc/mentohust.conf
%{_mandir}/man1/mentohust*
%doc %{_prefix}/doc/%{name}/*

%changelog
