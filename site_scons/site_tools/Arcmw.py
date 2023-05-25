import scons_common

from SCons.Script import *

# SCons mandatory function for custom tools, sets up environment
def generate(env):

######## ENVIRONMENT ########

	env['CPPDEFPREFIX'] 	= '-D'
	env['INCPREFIX']		= '-I'
	env['INCSUFFIX']		= ''
	env['OBJSUFFIX'] 		= '.o'
	env['DEPFILESUFFIX'] 	= '.d'
	env['MAPFILESUFFIX'] 	= '.map'
	env['LIBSUFFIX'] 		= '.a'
	env['PROGSUFFIX']		= '.elf'
	env['LIBLINKPREFIX']	= ''
	env['LIBDIRPREFIX']		= ''
	env['HEXFILESUFFIX'] 	= '.hex'
	env['S19FILESUFFIX'] 	= '.s19'
	env['BINFILESUFFIX'] 	= '.bin'
	env['ASMSUFFIXES'] 		= ['.ppc', '.s', '.S']
	env['CPPSUFFIXES'] 		= ['.c','.C','.h','.H']
	env['SPAWN'] 			= scons_common.w32api_cmdfilearg_spawn
	env['PRINT_CMD_LINE_FUNC'] = scons_common.print_cmd_line

######## END ENVIRONMENT ########
	
######## COMPILER ########
	
	# Implements: COR003, COR006, PLF001
	env['CC'] 				= env['COMPILER_ROOT'] + os.sep + 'ccac.exe'
	env['TARGETCPUOPT'] 	= env['CPU_TARGET']
	env['TARGETARCHIOPT'] 	= env['CPU_ARCHI']
	env['CPPDEFINES']   	= env['COMPILER_DEFINES']
	env['CCFLAGS'] 			= env['COMPILER_FLAGS']

	# Compiler command line
	env['CCCOM'] 			= '$CC -c $SOURCE $CCFLAGS $CPPDEFINES -o $TARGET -I$CEN_HEADER_DIR'
	
######## END COMPILER ########

######## ASSEMBLER ########
	# Implements: COR004, COR008, PLF002
	env['AS']				= env['COMPILER_ROOT'] + os.sep + 'ccac.exe'
	env['ASFLAGS']  		= env['ASSEMBLER_FLAGS']
	
	# Assembler command line
	env['ASCOM']			= '$AS -o $TARGET -c $SOURCES $ASCFLAGS $ASFLAGS $_ASCOMCOM $_ASCPPCOMCOM'
	env['ASPPFLAGS'] 		= env['ASSEMBLER_FLAGS']
	env['ASCPPDEFINES']		= env['ASSEMBLER_DEFINES']
	env['_ASCPPDEFFLAGS'] 	= '${_defines(CPPDEFPREFIX, ASCPPDEFINES,CPPDEFSUFFIX, __env__)}'
	env['_ASCPPCOMCOM']    	=  '$ASCPPFLAGS $_ASCPPDEFFLAGS $_ASCPPINCFLAGS $_CPPINCFLAGS'
	
	# Assembler command line
	env['ASPPCOM']			= '$AS -o $TARGET -c $SOURCES $ASPPFLAGS $ASCFLAGS $_ASCPPCOMCOM'
######## END ASSEMBLER ########

######## ARCHIVER ########
	env['AR']				= env['COMPILER_ROOT'] + os.sep + 'armar.exe'
	env['ARFLAGS']			= env['COMPILER_FLAGS']
	
	# Archiver command line
	env['ARCOM']			= '$AR rv -o $TARGET -c $SOURCES $ASPPFLAGS $ASCFLAGS $_ASCPPCOMCOM'
######## END ARCHIVER ########

######## LINKER ########

	# Implements: COR005, COR007, PLF003
	env['LINK'] 			= env['COMPILER_ROOT'] + os.sep + 'ccac.exe'
	env['LINKFLAGS']		= env['LINKER_FLAGS']
	env['LINKCMDFILE'] 		= env['LINKER_FILE']
	env['LIBS']				= env['ADD_LIBRARIES']
	env['LIBPATH']			= env['LIBRARY_PATHS']
	env['LINKERGENMAPFILE'] = '-Hldopt=-Coutput=' + os.path.join(env.subst('$OUTPUT_DIR'), 'dbg', env.subst('$TARGET_NAME')) + env['MAPFILESUFFIX']
	env['_LIBDIRFLAGS'] 	= '${_concat(LIBDIRPREFIX, LIBPATH, LIBDIRSUFFIX, __env__, RDirs, TARGET, SOURCE)}'
	env['_LIBFLAGS']		= '${_concat(LIBLINKPREFIX, LIBS, LIBLINKSUFFIX, __env__)}'
	
	# Linker command line
	env['LINKCOM']  		= '$LINK $LINKFLAGS $SOURCES $_LIBDIRFLAGS $_LIBFLAGS -w $LINKCMDFILE -o $TARGET $LINKERGENMAPFILE'
######## END LINKER ########
######## OBJECT BUILDER ########
	# Implements: COR003, COR004
	cs = SCons.Scanner.ClassicCPP("CScanner", "$CPPSUFFIXES", "CPPPATH", \
					'^[ \t]*#[ \t]*(?:include|import)[ \t]*(<|")([^>"]+)(>|")') 

	obj_builder_cc = SCons.Builder.Builder(action = '$CCCOM',src_suffix = env.subst('$CPPSUFFIXES'), \
										suffix = env.subst('$OBJSUFFIX'),
										source_scanner = cs)

	obj_builder_as = SCons.Builder.Builder(action = '$ASPPCOM',src_suffix = env.subst('$ASMSUFFIXES'), \
										suffix = env.subst('$OBJSUFFIX'),
										source_scanner = cs)
										
	env['BUILDERS']['Object']	= obj_builder_cc
	env['BUILDERS']['Object_s']	= obj_builder_as
######## END OBJECT BUILDER ########	
######## HEX BUILDER ########

	# Implements: COR011
	env['HEXCMD'] 			= './site_scons/site_tools/objcopy.exe'

	hex_builder = SCons.Builder.Builder(action = '$HEXCMD -O ihex "$SOURCE" \
																	"$TARGET"',
										single_source = 1,
										suffix = env.subst('$HEXFILESUFFIX'))
										
	env['BUILDERS']['Hex']	= hex_builder

######## END HEX BUILDER ########

######## S19 BUILDER ########

	#Implements: COR011
	env['S19CMD'] 			= './site_scons/site_tools/objcopy.exe'

	s19_builder = SCons.Builder.Builder(action = '$S19CMD -O srec "$SOURCE" \
																	"$TARGET"',
										single_source = 1,
										suffix = env.subst('$S19FILESUFFIX'))
	env['BUILDERS']['S19']	= s19_builder
######## END S19 BUILDER ########

######## BINNER ########

	env['BINCMD'] 			= env['COMPILER_ROOT'] + os.sep + 'elf2bin.exe'

	bin_builder = SCons.Builder.Builder(action = '$BINCMD "$SOURCE" \
																"$TARGET"',
										single_source = 1,
										suffix = env.subst('$BINFILESUFFIX'))
	env['BUILDERS']['Bin']	= bin_builder
######## END BINNER ########

######## END OBJECT BUILDER ########

# SCons mandatory function for custom tools
def exists(env):
    return True

if __name__ == '__main__':
	main()	
