import os
import re
import sys
import string
import SCons.Errors
import SCons.Util
import SCons.Action
import SCons.Scanner
import tempfile
import subprocess
import _subprocess
import datetime


from SCons.Script import *

def print_cmd_line(s, targets, sources, env):
    if ARGUMENTS.get('VERBOSE') == "1":
        sys.stdout.write(s)
    else:
        if len(sources) > 1:
            sys.stdout.write("Linking %s...\n" % (''.join([str(x) for x in targets])))
        else:
            file_extention = ("%s" % (''.join([str(x) for x in sources]))).split(".")[-1]
            if( file_extention == "c" ):
                sys.stdout.write("Compiling %s\n"% (' and '.join([str(x) for x in sources])))
            else:
                if( file_extention == "s" or file_extention == "S"):
                    sys.stdout.write("Assembling %s\n"%(' and '.join([str(x) for x in sources])))
                else:
                    if( file_extention == "elf"):
                        sys.stdout.write("Dumping %s to %s\n"% ( (''.join([str(x) for x in sources])), (''.join([str(x) for x in targets])) ))
                    else:
                        if( file_extention == "axf"):
                            sys.stdout.write("Dumping %s to %s\n"% ( (''.join([str(x) for x in sources])), (''.join([str(x) for x in targets])) ))

# Implements: PLF013
def setuplogging(env):
    DATETIMENOW = datetime.datetime.now().strftime("%Y_%m_%d_%H%M%S")
    CURRENTLOGDIR =  os.getcwd() + os.sep + env.subst('$LOG_DIR')

    if not os.path.exists(CURRENTLOGDIR):
        os.makedirs(CURRENTLOGDIR)

    CURRENTLOGFILE = CURRENTLOGDIR + os.sep + DATETIMENOW + '_currentlog.log'

    sys.stdout = Tee(CURRENTLOGFILE)
    sys.stderr = Tee(CURRENTLOGFILE)

# Implements: PLF013
class Tee(object):
    def __init__(self, logfile=None, mode='w'):
        self.stdout = sys.stdout
        sys.stdout = self
        if logfile:
            self.logfile = open(logfile, mode)
        else:
            self.file = None
    def write(self, s):
        if self.logfile:
            self.logfile.write(s)
        self.stdout.write(s)
    def close(self):
        if self.logfile:
            self.logfile.close()
        sys.stdout = self.stdout
    def flush(self):
        pass

def w32api_spawn(sh, escape, cmd, args, e_env):

	if cmd == "SHELL":
		return SCons.Platform.win32.spawn(sh,escape,args[1], args[1:],e_env)
	cmdline = cmd + ' ' + string.join(args[1:], ' ')
	startupinfo = subprocess.STARTUPINFO()
	startupinfo.dwFlags |= _subprocess.STARTF_USESHOWWINDOW
	proc = subprocess.Popen(
		cmdline,
		stdin=subprocess.PIPE,
		stdout=subprocess.PIPE,
		stderr=subprocess.STDOUT,
		startupinfo=startupinfo,
		shell = False,
		env = None
		)

	data, err = proc.communicate()


	print data
	rv = proc.wait()
	if rv:
		print "==================="
		print err
		print "==================="
	return rv

def w32api_cmdfilearg_spawn(sh, escape, cmd, args, e_env):
	if cmd == "SHELL":
		return w32api_spawn(sh, escape, cmd, args, e_env)
	cmdline = cmd +' '+string.join(args[1:], ' ')
	tmpfilename = ''
	rv = 0
	if len(cmdline) > 32766:
		tfile = tempfile.mkstemp('_args','scons_cmd_',r'c:\temp')
		tmpfilename = tfile[1]
		for n in range(1 ,len(args)):
			ta = args[n].replace("\\","\\\\")
	
			try:
				if ta[0] == '"' :
					ta = ta[1:-1]
			except:
				pass
			os.write(tfile[0],ta + '\n')
		os.close(tfile[0]);
		if cmd.lower().find('tasking')!=-1:
			cmdline = cmd + tmpfilename 
			rv = w32api_spawn(sh, escape, cmd, [ args[0] , '-f"' + tmpfilename + '"' ],e_env)
		elif cmd.lower().find('hightec')!=-1:
			cmdline = cmd +' @' + tmpfilename 
			rv = w32api_spawn(sh, escape, cmd, [ args[0] , '@"' + tmpfilename + '"' ],e_env)
		elif cmd.lower().find('arm')!=-1:
			cmdline = cmd + tmpfilename 
			rv = w32api_spawn(sh, escape, cmd, [ args[0] , '-@'+ tmpfilename],e_env)
		elif cmd.lower().find('arc')!=-1:
			cmdline = cmd +' @' + tmpfilename 
			rv = w32api_spawn(sh, escape, cmd, [ args[0] , '@"'+ tmpfilename + '"'],e_env)
	else:
		rv = w32api_spawn(sh, escape, cmd, args,e_env)
	if not rv and tmpfilename:
		os.remove(tmpfilename)
	return rv

def validate_environment(env):

    if not env.has_key('COMPILER_TOOL') or env['COMPILER_TOOL'] == '':
        print ('Compiler tool is undefined, aborting the build.')
        Exit(1)

    if not env.has_key('COMPILER_ROOT') or env['COMPILER_ROOT'] == '':
        print ('Compiler root path is undefined, aborting the build.')
        Exit(1)

    if not env.has_key('LINKER_FILE') or env['LINKER_FILE'] == '':
        print ('Linker file is undefined, aborting the build.')
        Exit(1)

    if not env.has_key('CPU_TARGET') or env['CPU_TARGET'] == '':
        print ('CPU target is undefined, aborting the build.')
        Exit(1)

    if not env.has_key('CPU_ARCHI') or env['CPU_ARCHI'] == '':
        print ('CPU architecture is undefined, aborting the build.')
        Exit(1)

def ncpus():
    if os.environ.has_key("NUMBER_OF_PROCESSORS"):
        ncpus = int(os.environ["NUMBER_OF_PROCESSORS"])
        if ncpus > 0:
            return ncpus
    return 1
    
def gen_link_objects_file(file, objs):
    f = open(file, 'w')
    for obj in objs:
        f.write(str(obj) + "\n")
    f.close()
    return file

def create_work_dir(path):
    "create a directory"
    if not os.path.exists(path):
        Execute(Mkdir(path))
    return path