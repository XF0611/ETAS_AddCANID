# Version information:
# BUILD_TYPE = PROD  : production release
# BUILD_TYPE = DEBUG : temporary debug build
# BUILD_TYPE = DEVEL : development release
LINK_TYPE           = 'Standalone' # 1=Standalone, 2=BOOT - Integration with FBL
BUILD_TYPE          = 'DEBUG'
if LINK_TYPE   == 'Standalone':
	PROJ_NAME           = 'ETAS_BIP_PT_ARCMW'
elif LINK_TYPE == 'BOOT':
	PROJ_NAME           = 'ETAS_BIP_PT_ARCMW_BOOT'
SW_VERSION_MSB      = 0
SW_VERSION_LSB      = 0


# Compiler Type Choice 
			# COMPILER_CHOICE = TASKING     : Tasking V4 Compiler
			# COMPILER_CHOICE = GNU         : GNU Hightec Compiler
			# COMPILER_CHOICE = WINDRIVER   : WINDRIVER Compiler
            # COMPILER_CHOICE = ARM         : ARM Compiler
            # COMPILER_CHOICE = ARMTI       : TI ARM C/C++ Compiler
            # COMPILER_CHOICE = ARCMW       : Synopsys ARCv2 Compiler for CalterahRhine
COMPILER_CHOICE = 'ARCMW'


# General configuration:--------------------------------------------------------------------------------------------
OUTPUT_DIR          = 'output'
LOG_DIR             = 'log' 	
COMPILER_TOOL       = 'Arcmw'#Must match the file name located in the site_tools directory
COMPILER_ROOT_LIB   = 'C:/ARC/MetaWare/arc/inc'
COMPILER_ROOT       = 'C:/ARC/MetaWare/arc/bin'


# CPU target--------------------------------------------------------------------------------------------------------
if COMPILER_CHOICE   == 'TASKING':
	CPU_TARGET          = ''
	CPU_USED            = ''

elif COMPILER_CHOICE == 'GNU':
	CPU_TARGET          = ''
	CPU_USED            = ''

elif COMPILER_CHOICE == 'WINDRIVER':
	CPU_TARGET          = ''
	CPU_USED            = ''

elif COMPILER_CHOICE == 'ARM':
	CPU_TARGET          = ''
	CPU_ARCHI           = ''

elif COMPILER_CHOICE == 'ARMTI':
	CPU_TARGET          = ''
	CPU_ARCHI           = ''

elif COMPILER_CHOICE == 'ARCMW':
	CPU_TARGET          = 'core4'
	CPU_ARCHI           = 'arcv2em'

else:
	CPU_TARGET   = ''
	CPU_ARCHI    = ''

# Compiler configuration:-------------------------------------------------------------------------------------------
if COMPILER_CHOICE   == 'TASKING':
	COMPILER_DEFINES        = [''] 
	COMPILER_FLAGS		    = ''

elif COMPILER_CHOICE == 'GNU':
	COMPILER_DEFINES        = [''] 
	COMPILER_FLAGS			= ''

elif COMPILER_CHOICE == 'WINDRIVER':
	COMPILER_DEFINES        = [''] 
	COMPILER_FLAGS			= ''

elif COMPILER_CHOICE == 'ARM':
	COMPILER_DEFINES        = [''] 
	COMPILER_FLAGS			= ''

elif COMPILER_CHOICE == 'ARMTI':
	COMPILER_DEFINES        = [''] 
	COMPILER_FLAGS			= ''

elif COMPILER_CHOICE == 'ARCMW':
	COMPILER_DEFINES        = ''
	COMPILER_FLAGS			= ' -I COMPILER_ROOT_LIB \
								-D__MW__ \
								-g \
								-arcv2em \
								-core4 \
								-Hnocopyr \
                                -Hnosdata \
                                -Hoff=uncached_in_own_section \
                                -Hrgf_num_banks=1 \
								-w'

# Assembler configuration:-------------------------------------------------------------------------------------------
if COMPILER_CHOICE   == 'TASKING':
	ASSEMBLER_DEFINES       = COMPILER_DEFINES
	ASSEMBLER_FLAGS         = ''

elif COMPILER_CHOICE == 'GNU':
	ASSEMBLER_DEFINES       = COMPILER_DEFINES
	ASSEMBLER_FLAGS			= COMPILER_FLAGS

elif COMPILER_CHOICE == 'WINDRIVER':
	ASSEMBLER_DEFINES       = ''
	ASSEMBLER_FLAGS			= ''

elif COMPILER_CHOICE == 'ARM':
	ASSEMBLER_DEFINES       = COMPILER_DEFINES
	ASSEMBLER_FLAGS			= COMPILER_FLAGS

elif COMPILER_CHOICE == 'ARMTI':
	ASSEMBLER_DEFINES       = COMPILER_DEFINES
	ASSEMBLER_FLAGS			= COMPILER_FLAGS

elif COMPILER_CHOICE == 'ARCMW':
	ASSEMBLER_DEFINES       = ''
	if LINK_TYPE   == 'Standalone':
		ASSEMBLER_FLAGS			= '-D__MW__ \
									-Hasopt=-g \
									-arcv2em \
									-core4 \
									-Hnocopyr'
	elif LINK_TYPE == 'BOOT':
		ASSEMBLER_FLAGS			= '-D__MW__ \
									-D_LOAD_XIP_TEXT=1 \
									-Hasopt=-g \
									-arcv2em \
									-core4 \
									-Hnocopyr'		

# Libraries to include:----------------------------------------------------------------------------------------------
if COMPILER_CHOICE   == 'TASKING':
	ADD_LIBRARIES        = ['']
elif COMPILER_CHOICE == 'GNU':
	ADD_LIBRARIES        = ['']
elif COMPILER_CHOICE == 'WINDRIVER':
	ADD_LIBRARIES        = ['']
elif COMPILER_CHOICE == 'ARM':
	ADD_LIBRARIES        = ['']
elif COMPILER_CHOICE == 'ARMTI':
	ADD_LIBRARIES        = ['']
elif COMPILER_CHOICE == 'ARCMW':
	ADD_LIBRARIES        = ['']
else:
	ADD_LIBRARIES    = []

LIBRARY_PATHS        = []

OBJECT_FILE          = []


# Linker configuration:----------------------------------------------------------------------------------------------

#No optimization		 	-OCLTXY --optimize=0 -O0
#Default optimization 		-OcLtxy --optimize=1 -O1
#All optimizations 	 		-Ocltxy --optimize=2 -O2

if COMPILER_CHOICE   == 'TASKING':
	LINKER_FLAGS      = ''
	LINKER_FILE       = ''
elif COMPILER_CHOICE == 'GNU':  
	LINKER_FLAGS      = ''
	LINKER_FILE       = ''
elif COMPILER_CHOICE == 'WINDRIVER':
	LINKER_FLAGS      = ''
	LINKER_FILE       = ''
elif COMPILER_CHOICE == 'ARM':
	LINKER_FLAGS      = ''
	LINKER_FILE       = ''
elif COMPILER_CHOICE == 'ARMTI':  
	LINKER_FLAGS      = ''
	LINKER_FILE       = ''
elif COMPILER_CHOICE == 'ARCMW':  
	LINKER_FLAGS      = '-e_start \
						-core4 \
						-arcv2em \
						-Hldopt=-q \
						-Hldopt=-Cglobals \
						-Hldopt=-Cfunctions \
						-Hldopt=-Csections \
						-Hldopt=-Csize \
						-Hldopt=-Cunmangle'
if LINK_TYPE   == 'Standalone':
	LINKER_FILE       = '.\BasicSoftware\integration\linker\ETAS_BIP_PT_ARCMW.ld'
elif LINK_TYPE == 'BOOT':
	LINKER_FILE       = '.\BasicSoftware\integration\linker\ETAS_BIP_PT_ARCMW_BOOT.ld'

#centralized header directory::----------------------------------------------------------------------------------------------
if COMPILER_CHOICE   == 'TASKING':
	CEN_HEADER_DIR   = 'output\inc'

elif COMPILER_CHOICE == 'GNU':
	CEN_HEADER_DIR   = 'output\inc'

elif COMPILER_CHOICE == 'WINDRIVER':
	CEN_HEADER_DIR   = 'output\inc'

elif COMPILER_CHOICE == 'ARM':
	CEN_HEADER_DIR   = 'output\inc'

elif COMPILER_CHOICE == 'ARMTI':
	CEN_HEADER_DIR   = 'output\inc'

elif COMPILER_CHOICE == 'ARCMW':
	CEN_HEADER_DIR   = 'output\inc'
