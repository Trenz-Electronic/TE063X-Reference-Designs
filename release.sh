#!/bin/sh
# Projects to release
REPOSITORY=TE063X-Reference-Designs
# Numbers of projects to release (list like '1 2 3 4')
PROJECT_LIST='1'
# Project 1 description
# List of folders requred to release (First will be used as name)
NAME1='reference-TE0630 TE-EDK-IP'
# Tool used to build
TOOL1='EDK-13.2'
# Project Version
VER1='1.0'

# Project 2 description
NAME2=''
TOOL2=''
VER2=''

# Project 3 description
NAME3=''
TOOL3=''
VER3=''

############# Do not edit below this line !!!
SUBVER=`git rev-parse --short HEAD`

if [ -z "$PROJECT_LIST" ]; then exit ; fi
cd ..
for l in $PROJECT_LIST
do
	namelist=NAME$l
	tool=TOOL$l
	ver=VER$l
	name=${!namelist}
	set $name
	for proj in ${!namelist}
	do
		zip -rq $1-${!tool}-v${!ver}.${SUBVER}.zip ${REPOSITORY}/${proj}
	done
done
cd ${REPOSITORY}

