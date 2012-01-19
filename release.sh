#!/bin/sh

TOOL=EDK-13.2
VER=1.0
SUBVER=`git rev-parse --short HEAD`

REPO=TE063X-Reference-Designs
PROJECT=reference-TE0630
SUBPROJECTS='TE-EDK-IP'

cd ..
tar -cf ${PROJECT}-${TOOL}-v${VER}.${SUBVER}.tar ${REPO}/${PROJECT} 
tar -rf ${PROJECT}-${TOOL}-v${VER}.${SUBVER}.tar ${REPO}/${SUBPROJECTS}
gzip -f ${PROJECT}-${TOOL}-v${VER}.${SUBVER}.tar
cd ${REPO}

