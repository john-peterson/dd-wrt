#!/usr/bin/env python
######################################################################
##
##  Generate parameter dictionary from param/loadparm.c
##
##  Copyright (C) Gerald Carter		       2004.
##
##  This program is free software; you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation; either version 3 of the License, or
##  (at your option) any later version.
##
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with this program; if not, see <http://www.gnu.org/licenses/>.
##
######################################################################

import re, string, sys, commands

HEADER = """######################################################################
##
## autogenerated file of smb.conf parameters
## generate_parm_table <..../param/loadparm.c>
##
##  Copyright (C) Gerald Carter		       2004.
##
##  This program is free software; you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation; either version 3 of the License, or
##  (at your option) any later version.
##
##  This program is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU General Public License for more details.
##
##  You should have received a copy of the GNU General Public License
##  along with this program; if not, see <http://www.gnu.org/licenses/>.
##
######################################################################

from SambaParm import SambaParmString, SambaParmBool, SambaParmBoolRev

## boolean defines for parm_table
P_LOCAL    = 0
P_GLOBAL   = 1

"""

FOOTER = """##### end of smbparm.y ##########################################
#################################################################"""

TESTPARM = "/usr/bin/testparm"

## fields in Samba's parameter table
displayName = 0
type        = 1
scope       = 2
variable    = 3
flags       = 6

parm_table  = {}
var_table   = {}
def_values  = {}
obj_table = {
	'P_BOOL'    : 'SambaParmBool',
	'P_BOOLREV' : 'SambaParmBoolRev',
	'P_STRING'  : 'SambaParmString',
	'P_USTRING' : 'SambaParmString',
	'P_GSTRING' : 'SambaParmString',
	'P_LIST'    : 'SambaParmString',
	'P_ENUM'    : 'SambaParmString',
	'P_CHAR'    : 'SambaParmString',
	'P_OCTAL'   : 'SambaParmString',
	'P_INTEGER' : 'SambaParmString',
}

######################################################################
##                        BEGIN MAIN CODE                           ##
######################################################################

## First thing is to build the dictionary of parmeter names  ##
## based on the output from testparm                         ##

cmd = "/usr/bin/testparm -s -v /dev/null"
( status, testparm_output ) = commands.getstatusoutput( cmd )
if status:
	sys.stderr.write( "Failed to execute testparm!\n%s\n" % testparm_output )


## break the output into a list ##

lines = string.split( testparm_output, "\n" )

## loop through list -- parameters in testparm output have ##
## whitespace at the beginning of the line                 ##

pattern = re.compile( "^\s+" )
for input_str in lines:
	if not pattern.search( input_str ):
		continue
	input_str = string.strip( input_str )
	parts = string.split( input_str, "=" )
	parts[0] = string.strip( parts[0] )
	parts[1] = string.strip( parts[1] )
	key = string.upper( string.join(string.split(parts[0]), "") )
	new = parts[1].replace('\\', '\\\\')
	def_values[key] = new

## open loadparm.c and get the entire list of parameters ##
## including synonums                                    ##
		
if len(sys.argv) != 2:
	print "Usage: %s <.../param/loadparm.c>" % ( sys.argv[0] )
	sys.exit( 1 )
	
try:
	fconfig = open( sys.argv[1], "r" )
except IOError:
	print "%s does not exist!" % sys.argv[1]
	sys.exit (1)

## Loop through loadparm.c --  all parameters are either ##
## P_LOCAL or P_GLOBAL                                   ##

synonyms = []
pattern = re.compile( '{".*P_[GL]' )
while True:
	input_str= fconfig.readline()
	if  len(input_str) == 0 :
		break
	input_str= string.strip(input_str)
	
	## see if we have a patch for a parameter definition ##
	
	parm = []
	if pattern.search( input_str) :
	
		## strip the surrounding '{.*},' ##
		
		input_str= input_str[1:-2]
		parm = string.split(input_str, ",")
		
		## strip the ""'s and upper case ##
		
		name = (string.strip(parm[displayName])[1:-1])
		key = string.upper( string.join(string.split(name), "") )
		var_name = string.strip( parm[variable] )
		

		## try to catch synonyms -- if the parameter was not reported ##
		## by testparm, then save it and come back after we will out  ##
		## the variable list                                          ##
		
		if not def_values.has_key( key ):
			synonyms.append( input_str)
			continue
		
			
		var_table[var_name] = key 
		
		parmType = string.strip(parm[type])
		
		parm_table[key] = [ name , string.strip(parm[type]), string.strip(parm[scope]), def_values[key] ]

## Deal with any synonyms ##

for input_str in synonyms:
		parm = string.split(input_str, ",")
		name = (string.strip(parm[displayName])[1:-1])
		key = string.upper( string.join(string.split(name), "") )
		var_name = string.strip( parm[variable] )
		
		## if there's no pre-existing key, then testparm doesn't know about it
		if not var_table.has_key( var_name ):
			continue
			
		## just make a copy
		parm_table[key] = parm_table[var_table[var_name]][:]
		# parm_table[key][1] = parm[1]
		parm_table[key][1] = string.strip(parm[1])

##                      ##	
## print out smbparm.py ##
##                      ##	

try:
	smbparm = open ( "smbparm.py", "w" )
except IOError:
	print "Cannot write to smbparm.py"
	sys.exit( 1 )
	
smbparm.write( HEADER )
smbparm.write( "parm_table = {\n" )

for x in parm_table.keys():
	key = "\"%s\"" % x
	smbparm.write("\t%-25s: (\"%s\", %s, %s, \"%s\"),\n" % ( key, parm_table[x][0], 
		obj_table[parm_table[x][1]], parm_table[x][2], parm_table[x][3] ))

smbparm.write( "}\n" )

smbparm.write( FOOTER )
smbparm.write( "\n" )

sys.exit(0)


##                  ##
## cut-n-paste area ##
##                  ##

for x in parm_table.keys():
	if def_values.has_key( x ):
		parm_table[x].append( def_values[x] )
	else:
		parm_table[x].append( "" )
