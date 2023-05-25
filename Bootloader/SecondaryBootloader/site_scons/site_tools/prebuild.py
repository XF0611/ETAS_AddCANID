import glob
import os,re
from SCons.Script import *
import SCons.Errors
import SCons.Util
import SCons.Action
import SCons.Scanner

def cleanHeaderDir( pathDir ):
    if not os.path.exists(pathDir):
        os.makedirs(pathDir)
    else:
        files=glob.glob('%s/*.h' % pathDir )
        for file in files:
            os.remove(file)

def createSymlinkHeader(  inclusionDes,inclusionSrc ):
    os.chdir(inclusionSrc)
    listFile = glob.glob( "*.h" )
    for f in listFile:
        flag = touchHeader(f, inclusionDes, inclusionSrc )
        # if flag == 1:
            # return False
    return True

def touchHeader( fileName, pathDir, srcPath ):
    pathfile = os.path.join(pathDir,fileName)

    if not os.path.isfile( pathfile ):
        f = open( pathfile, "w+" )
        f.write( '''#include "%s"\n'''% os.path.join(os.path.relpath( srcPath,pathDir), fileName) )
        f.close
        return 0
    else:
        f = open( pathfile, "r" )
        string =  re.sub(r'#include ','''''',f.read())
        if( ARGUMENTS.get('VERBOSE') == "1" ):
            print "\n\r[infor]Duplicated file %s in %s." % (fileName, srcPath)
            print "\t was previously located in %s." %  string
        f.close
        return 1

def createHotfixSource( pathDir, filename ):
    #read path file Rte.c
    pathfile_in = os.path.join(pathDir,filename)
    if not os.path.exists("output\\hotfix_src"):
        os.makedirs("output\\hotfix_src")
    pathfile_out = os.path.join( "output\\hotfix_src",filename)
    f_in = open( pathfile_in, "r")
    f_out = open( pathfile_out, "w+")
    original_content= f_in.read()
    if filename == "Rte.c":
        fixed_content = re.sub( r'(VAR\(Rte_EventType\, RTE_DATA\).*WaitingEv).*;',r'\1 = RTE_WOWP_EVENTS;', original_content )
    elif filename== "Rte_Lib.c":
        fixed_content = re.sub( r'(if \( 0 < --datum->count \))', '''if ( 0 < datum->count)
   {
	   datum->count--;
   }
   if ( 0 < datum->count )''', original_content )
        fixed_content = re.sub( r'(if \( \(TickType\)0 != timeout \))', r'/* \1 */', fixed_content)
        fixed_content = re.sub( r'if \( E_OK != SetRelAlarm\( alarm, timeout, 0uL \) \)', 'if ( E_OK != SetRelAlarm( alarm, 10, 0uL ) )', fixed_content)
    f_out.write( fixed_content )
    f_in.close
    f_out.close
    
    return 0
    
