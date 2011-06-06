#!/bin/bash
#
# Create package for delivery
#

TGT="/tmp/yacm-1.0"
rm -rf $TGT
mkdir -p $TGT/src
mkdir -p $TGT/doc
mkdir -p $TGT/resources

cp -a src/* $TGT/src/
cp -a doc/exported/* $TGT/doc/
cp -a resources/* $TGT/resources/
cp -a Doxyfile Makefile README $TGT/

