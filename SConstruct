########## BEGIN SCONSTRUCT ##########

import os,sys
from scons_common import ncpus

sys.path.append(Dir('#').abspath + "\\site_scons\\site_tools")


# Ensure tool versions
EnsurePythonVersion(2,7)
EnsureSConsVersion(3,0,0)

# Settings file to be read
settings_file = 'settings.py'

# Add one customized command to generate list of include path which is used for generate configuration for QAC
AddOption('--util',action='store_true')
# Configure number of CPU to compile the source code.
SetOption('num_jobs', ncpus())


# Load all the variables into the environment
if not os.path.exists(settings_file):
    print ('Could not find file ' + settings_file + ' in current directory.')
else:
    vars = Variables(settings_file)

    # Add variables to environment along with some help text
    vars.Add('OUTPUT_DIR',          'Path to build all target files.', 'output')
    vars.Add('LOG_DIR',             'Path to store all log files.', 'log')
    vars.Add('CPU_TARGET',          'CPU to build for.  Must match what the compiler is expecting as \'cpu\' option.')
    vars.Add('CPU_ARCHI',           'CPU to build for.  Must match what the compiler is expecting as \'cpu\' option.')
    vars.Add('COMPILER_TOOL',       'Custom SCons tool to be used for building.  Must match the file name located in the site_tools directory.')
    vars.Add('COMPILER_ROOT',       'Path to compiler executables.')
    vars.Add('COMPILER_FLAGS',      'Compiler flags to use for building.')
    vars.Add('COMPILER_DEFINES',    'Compiler defines to use for building.')
    vars.Add('ASSEMBLER_FLAGS',     'Assembler flags to use for building.')
    vars.Add('ASSEMBLER_DEFINES',   'Assembler defines to use for building.')
    vars.Add('LINKER_FLAGS',        'Linker flags to be used for building.')
    vars.Add('LINKER_FILE',         'Path to linker file to be used during linking.')
    vars.Add('ADD_LIBRARIES',       'Names of additional libraries to be included during linking.  These do not need the file extension.')
    vars.Add('LIBRARY_PATHS',       'Paths to look for the additional libraries.')
    vars.Add('BUILD_TYPE',          'Type of build to be done.  Options are: DEBUG, PROD, DEVEL')
    vars.Add('PROJ_NAME',           'Name of the project to be built.')
    vars.Add('OBJECT_FILE',         'path to object files to be added during linking.')
    vars.Add('CEN_HEADER_DIR',      'path to centralized header file of project.')
    vars.Add('LIBRARY_SEARCH_PATH', 'Path to search a specific library')
    vars.Add('CPU_USED', 	    'CPU Used.')
    # Initialize the default SCons environment
    env = Environment(variables = vars)

    # Generate variables help text
    env.Help(vars.GenerateHelpText(env))

    # to add the checking of SIMULATION BUILD from input command line
    simbuild = ARGUMENTS.get('SIMBUILD', 0)
    if int(simbuild):
        env.Append(COMPILER_DEFINES = '__EPM_TRIGGER_SIMULATION=__EPM_TSIM_ON')
        print ("************************************************************************************************")
        print ("*               Enter ENGINE SIMULATION BUILD mode                                            *")
        print ("* Compiler Preprocessor Macro option: %s *" %env['COMPILER_DEFINES'])
        print ("************************************************************************************************")
if (not GetOption('help') ) and (not GetOption('util') ):

	import scons_common
	import prebuild
	
	
	# Setup logging of output
	scons_common.setuplogging(env)
	
	# Validate the mandatory values of the environment
	scons_common.validate_environment(env)
	
	
	# Build target/project name
	if env['BUILD_TYPE'] == 'PROD':
		env['PROJ_PREFIX'] = 'P'
		#env['TARGET_NAME'] = env.subst('$PROJ_PREFIX') + '_' + env.subst('$PROJ_NAME') + '_V' + env.subst('$SW_VERSION_MSB') + '_' + env.subst('$SW_VERSION_LSB') + '_' + env.subst('$DATE_TIME')
	elif env['BUILD_TYPE'] == 'DEVEL':
		env['PROJ_PREFIX'] = 'R'
		#env['TARGET_NAME'] = env.subst('$PROJ_PREFIX') + '_' + env.subst('$PROJ_NAME') + '_V' + env.subst('$SW_VERSION_MSB') + '_' + env.subst('$SW_VERSION_LSB') + '_' + env.subst('$DATE_TIME')
	else:
		env['PROJ_PREFIX'] = 'D'
		env['TARGET_NAME'] = env.subst('$PROJ_PREFIX') + '_' + env.subst('$PROJ_NAME')


	############ START SCONSCRIPT ############
	#clone the evironment and setup compiler specific enivronment
	env = env.Clone(tools=[env['COMPILER_TOOL']])
	env = env.Clone(tools=[env['COMPILER_TOOL'], 'memory_usage'])
	
	#remove all psuedo header file
	prebuild.cleanHeaderDir(env['CEN_HEADER_DIR'])
	
	# Storage of all objects to be linked
	objects = []
	
	# Setup build directories for sub-systems
	asw_build_dir         = os.path.join(env.subst('$OUTPUT_DIR'), 'objs', 'asw')
	bsw_build_dir         = os.path.join(env.subst('$OUTPUT_DIR'), 'objs', 'basicSoftware', 'src', 'bsw')
	rte_build_dir         = os.path.join(env.subst('$OUTPUT_DIR'), 'objs', 'basicSoftware', 'src', 'rte')
	os_build_dir          = os.path.join(env.subst('$OUTPUT_DIR'), 'objs', 'basicSoftware', 'src', 'os')
	cobra_build_dir       = os.path.join(env.subst('$OUTPUT_DIR'), 'objs', 'basicSoftware', 'src', 'cobra')
	infra_build_dir       = os.path.join(env.subst('$OUTPUT_DIR'), 'objs', 'basicSoftware', 'integration', 'src')
	mcal_build_dir        = os.path.join(env.subst('$OUTPUT_DIR'), 'objs', 'basicSoftware', 'integration','mcal')
	cdd_build_dir         = os.path.join(env.subst('$OUTPUT_DIR'), 'objs', 'cdd')
	ecuabs_build_dir      = os.path.join(env.subst('$OUTPUT_DIR'), 'objs', 'ecuAbstraction', 'IoHwAbs')


	# Specify which sub-systems to build
	asw_path            = os.path.join('ASW/SConscript')
	bsw_path            = os.path.join('BasicSoftware/src/bsw/SConscript')
	rte_path            = os.path.join('BasicSoftware/src/rte/SConscript')
	os_path             = os.path.join('BasicSoftware/src/os/SConscript')
	cobra_path          = os.path.join('BasicSoftware/src/cobra/SConscript')
	infra_path          = os.path.join('BasicSoftware/integration/src/SConscript')
	mcal_path           = os.path.join('BasicSoftware/integration/mcal/SConscript')
	cdd_path            = os.path.join('CDD/SConscript')
	ecuabs_path         = os.path.join('EcuAbstraction/SConscript')    


	# Compile each subsystem's SConscript file and collect all objects
	objects += SConscript(asw_path, exports='env', variant_dir=asw_build_dir, duplicate=0)
	objects += SConscript(bsw_path, exports='env', variant_dir=bsw_build_dir, duplicate=0)
	objects += SConscript(rte_path, exports='env', variant_dir=rte_build_dir, duplicate=0)
	objects += SConscript(os_path, exports='env', variant_dir=os_build_dir, duplicate=0)
	objects += SConscript(cobra_path, exports='env', variant_dir=cobra_build_dir, duplicate=0)
	objects += SConscript(infra_path, exports='env', variant_dir=infra_build_dir, duplicate=0)
	objects += SConscript(mcal_path, exports='env', variant_dir=mcal_build_dir, duplicate=0)
	objects += SConscript(cdd_path, exports='env', variant_dir=cdd_build_dir, duplicate=0)
	objects += SConscript(ecuabs_path, exports='env', variant_dir=ecuabs_build_dir, duplicate=0)    



#-------------------------------------------------------------------------------------------------------
#	T_dir				= os.path.join(env.subst('$OUTPUT_DIR'), 'dbg')
#	T_path				= scons_common.create_work_dir(T_dir)
#	T_objs_file 		= os.path.join(T_path, env.subst('$TARGET_NAME')) + ".objectlist"
#	T_objects_path	= scons_common.gen_link_objects_file(T_objs_file, objects)
#
#	env.Append(LINKCOM=' @'+ T_objects_path)
#	env.Append(LINKERGENMAPFILE= " -m31 > " + T_path + os.sep + env.subst('$TARGET_NAME') + ".map")
#	env.Append(LINKCMDFILE= env.subst('$LINKER_FILE'))
#	exe_dir		=os.path.join(env.subst('$OUTPUT_DIR'), 'dbg', env.subst('$TARGET_NAME'))
#-------------------------------------------------------------------------------------------------------
	
	# Setup output path for debug, hex and A2L files
	dbg_path    = os.path.join(env.subst('$OUTPUT_DIR'), 'dbg', env.subst('$TARGET_NAME'))
	hex_path    = os.path.join(env.subst('$OUTPUT_DIR'), 'hex', env.subst('$TARGET_NAME'))
	s19_path    = os.path.join(env.subst('$OUTPUT_DIR'), 's19', env.subst('$TARGET_NAME'))
	bin_path    = os.path.join(env.subst('$OUTPUT_DIR'), 'bin', env.subst('$TARGET_NAME'))
	mem_path    = os.path.join(env.subst('$OUTPUT_DIR'), 'dbg', env.subst('$TARGET_NAME')+'.map')
	
	# Build final debug, elf
	dbg_exe     = env.Program(dbg_path, objects)
	
	
	# Setup explicit dependency of elf file to linker script
	env.Depends(dbg_exe, env['LINKER_FILE'])
	# Setup explicit dependency of elf file to static library file of OS
	env.Depends(dbg_exe, env['LIBRARY_PATHS'])
	
	hex_exe     = env.Hex(hex_path, dbg_exe)
	s19_exe     = env.S19(s19_path, dbg_exe)
	bin_exe     = env.Bin(bin_path, dbg_exe)
	
	#meminfo     = env.Meminfo(mem_path, dbg_exe)
	############ END SCONSCRIPT ############
	
	########## END SCONSTRUCT ##########
