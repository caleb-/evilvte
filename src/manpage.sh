#!/bin/sh
# Copyright (C) 2008-2009  Wen-Yen Chuang <caleb AT calno DOT com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

COMMAND_ENABLED=`grep COMMAND_ $1 | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | grep -v FALSE | grep -v 0 | tail -n 1`
if [ "$COMMAND_ENABLED" = "" ]; then
  grep -v -i option misc/manpage.1 > misc/evilvte.1
  exit
fi

COMMAND_TAB_NUMBERS=`grep    COMMAND_TAB_NUMBERS    $1 | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
TAB_ENABLE=`grep             ' TAB '                $1 | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
if [ "$TAB_ENABLE" = "" ]; then
  COMMAND_TAB_NUMBERS=
fi

COMMAND_AT_ROOT_WINDOW=`grep COMMAND_AT_ROOT_WINDOW $1 | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
COMMAND_DOCK_MODE=`grep      COMMAND_DOCK_MODE      $1 | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
COMMAND_EXEC_PROGRAM=`grep   COMMAND_EXEC_PROGRAM   $1 | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
COMMAND_FULLSCREEN=`grep     COMMAND_FULLSCREEN     $1 | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
COMMAND_LOGIN_SHELL=`grep    COMMAND_LOGIN_SHELL    $1 | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
COMMAND_SET_TITLE=`grep      COMMAND_SET_TITLE      $1 | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
COMMAND_SHOW_HELP=`grep      COMMAND_SHOW_HELP      $1 | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
COMMAND_SHOW_OPTIONS=`grep   COMMAND_SHOW_OPTIONS   $1 | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`
COMMAND_SHOW_VERSION=`grep   COMMAND_SHOW_VERSION   $1 | tr -s ' ' ' ' | sed 's/^ //' | grep -v ^\/\/ | tail -n 1 | grep -v FALSE | grep -v 0`

head -n 10 misc/manpage.1 > misc/evilvte.1

if [ "$COMMAND_DOCK_MODE" != "" ]; then
  grep '^.\\" D ' misc/manpage.1 | sed 's/^.\\" D //' >> misc/evilvte.1
fi

if [ "$COMMAND_EXEC_PROGRAM" != "" ]; then
  grep '^.\\" E ' misc/manpage.1 | sed 's/^.\\" E //' >> misc/evilvte.1
fi

if [ "$COMMAND_FULLSCREEN" != "" ]; then
  grep '^.\\" F ' misc/manpage.1 | sed 's/^.\\" F //' >> misc/evilvte.1
fi

if [ "$COMMAND_SHOW_HELP" != "" ]; then
  grep '^.\\" H ' misc/manpage.1 | sed 's/^.\\" H //' >> misc/evilvte.1
fi

if [ "$COMMAND_LOGIN_SHELL" != "" ]; then
  grep '^.\\" L ' misc/manpage.1 | sed 's/^.\\" L //' >> misc/evilvte.1
fi

if [ "$COMMAND_SHOW_OPTIONS" != "" ]; then
  grep '^.\\" O ' misc/manpage.1 | sed 's/^.\\" O //' >> misc/evilvte.1
fi

if [ "$COMMAND_AT_ROOT_WINDOW" != "" ]; then
  grep '^.\\" R ' misc/manpage.1 | sed 's/^.\\" R //' >> misc/evilvte.1
fi

if [ "$COMMAND_SET_TITLE" != "" ]; then
  grep '^.\\" T ' misc/manpage.1 | sed 's/^.\\" T //' >> misc/evilvte.1
fi

if [ "$COMMAND_SHOW_VERSION" != "" ]; then
  grep '^.\\" V ' misc/manpage.1 | sed 's/^.\\" V //' >> misc/evilvte.1
fi

if [ "$COMMAND_TAB_NUMBERS" != "" ]; then
  grep '^.\\" 2 ' misc/manpage.1 | sed 's/^.\\" 2 //' >> misc/evilvte.1
fi

tail -n 4 misc/manpage.1 >> misc/evilvte.1

COMMAND_NUMBERS=`grep '^.B..-' misc/evilvte.1 | wc | awk '{print $1}'`
if [ "$COMMAND_NUMBERS" = "0" ]; then
  grep -v -i option misc/manpage.1 > misc/evilvte.1
  exit
fi
if [ "$COMMAND_NUMBERS" != "1" ]; then
  sed -e 's/option/options/' -e 's/OPTION/OPTIONS/' misc/evilvte.1 > misc/evilvte.2
  mv misc/evilvte.2 misc/evilvte.1
fi
