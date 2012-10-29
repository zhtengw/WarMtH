#!/bin/bash

# Copyright 2012  Aten Zhang, SYSU, China
# All rights reserved.
#
#   Permission to use, copy, modify, and distribute this software for
#   any purpose with or without fee is hereby granted, provided that
#   the above copyright notice and this permission notice appear in all
#   copies.
#
#   THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
#   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
#   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
#   IN NO EVENT SHALL THE AUTHORS AND COPYRIGHT HOLDERS AND THEIR
#   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
#   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
#   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
#   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
#   OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
#   SUCH DAMAGE.
# -----------------------------------------------------------------------------
#
# Slackware SlackBuild script 
# ===========================
# By:          Aten Zhang <atenzd@gmail.com>
# For:         WarMtH
# Descr:       a GUI front end of MentoHUST
# URL:         http://code.google.com/p/mentohust/ 
# Needs:       
# Changelog:   
# 1.0-1:       11/Sep/2012 by Aten Zhang <atenzd@gmail.com>
#              * Initial build.
# Run 'sh mentohust.SlackBuild' to build a Slackware package.
# The package (.tgz) and .txt file as well as build logs are created in /tmp .
# Install it using 'installpkg'. 
#
# -----------------------------------------------------------------------------

# Set initial variables:

PKGNAM=warmth
#VERSION=${VERSION:-$(echo $PKGNAM-*.tar.?z* | rev | cut -f 3- -d . | cut -f 1 -d - | rev)}
VERSION=${VERSION:-1.0}
BUILD=${BUILD:-1}
NUMJOBS=${NUMJOBS:" -j4 "}
TAG=${TAG:-_aten}

# Where do we look for sources?
SRCDIR=${SRCDIR:-$(cd $(dirname $0); pwd)}

# Place to build (TMP) package (PKG) and output (OUTPUT) the program:
CWD=$(pwd)
TMP=${TMP:-/tmp/build}
PKG=$TMP/package-$PKGNAM
OUTPUT=${OUTPUT:-/tmp}


# Automatically determine the architecture we're building on:
if [ -z "$ARCH" ]; then
  case "$( uname -m )" in
    i?86) export ARCH=i486 ;;
    arm*) export ARCH=arm ;;
    # Unless $ARCH is already set, use uname -m for all other archs:
       *) export ARCH=$( uname -m ) ;;
  esac
fi

case "$ARCH" in
  i486)      SLKCFLAGS="-O2 -march=i486 -mtune=i686"
             SLKLDFLAGS=""; LIBDIRSUFFIX=""
             ;;
  x86_64)    SLKCFLAGS="-O2 -fPIC"
             SLKLDFLAGS="-L/usr/lib64"; LIBDIRSUFFIX="64"
             ;;
  *)         SLKCFLAGS="-O2"
             SLKLDFLAGS=""; LIBDIRSUFFIX=""
             ;;
esac

# Exit the script on errors:
set -e
trap 'echo "$0 FAILED at line ${LINENO}" | tee $OUTPUT/error-${PKGNAM}.log' ERR
# Catch unitialized variables:
set -u
P1=${1:-1}

# Create working directories:
mkdir -p $OUTPUT          # place for the package to be saved
mkdir -p $TMP/tmp-$PKGNAM # location to build the source
mkdir -p $PKG             # place for the package to be built
rm -rf $PKG/*             # always erase old package's contents
rm -rf $TMP/tmp-$PKGNAM/* # remove the remnants of previous build
rm -rf $OUTPUT/{configure,make,install,error,makepkg}-$PKGNAM.log
                          # remove old log files
                                                                                                                                                                                     

# --- PACKAGE BUILDING ---

echo "++"
echo "|| $PKGNAM-$VERSION"
echo "++"

# Get the source code from svn
#svn checkout http://mentohust.googlecode.com/svn/trunk/ $TMP/tmp-$PKGNAM
cp -r $CWD/* $TMP/tmp-$PKGNAM
cd $TMP/tmp-$PKGNAM

# patch to disable fully buffered (need line buffered to show message on gui)
#sed -i '/atexit/i\setvbuf(stdout,(char *)NULL, _IONBF, 0);' src/mentohust.c
patch -Np0 -i ${CWD}/mentohust-linebuf.patch

chown -R root:root .
chmod -R u+w,go+r-w,a+X-s .

cd mentohust
sh autogen.sh

./configure \
  --prefix=/usr \
  --libdir=/usr/lib${LIBDIRSUFFIX} \
  --mandir=/usr/man \
  --build=$ARCH-slackware-linux 

make $NUMJOBS || make || exit 1
make install DESTDIR=$PKG || exit 1

cd ../
lrelease l10n/*.ts
qmake WarMtH.pro
make
make install INSTALL_ROOT=$PKG

chmod 4755 $PKG/usr/bin/mentohust
mv $PKG/etc/mentohust.conf $PKG/etc/mentohust.conf.new
# Add documentation:
rm -fr $PKG/usr/share/doc
mkdir -p $PKG/usr/doc/$PKGNAM-$VERSION
cp -a AUTHORS ChangeLog README NEWS \
  $PKG/usr/doc/$PKGNAM-$VERSION || true
chown -R root:root $PKG/usr/doc/$PKGNAM-$VERSION
find $PKG/usr/doc -type f -exec chmod 644 {} \;

# Strip binaries (if any):
find $PKG | xargs file | grep -e "executable" -e "shared object" | grep ELF \
  | cut -f 1 -d : | xargs strip --strip-unneeded 2> /dev/null || true

cp -r $TMP/tmp-$PKGNAM/DEBIAN $PKG

cd $TMP

dpkg-deb -b $PKG 
