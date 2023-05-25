import scons_common
import os
import re
import StringIO

from SCons.Script import *

# SCons mandatory function for custom tools, sets up environment
# Implements: COR012
def generate(env):

	env['MEMFILESUFFIX'] 	= '.log'
######## Memory Usage BUILDER ########

	memory_usage_builder = SCons.Builder.Builder(action = memory_usage_info,
										single_source = 1,
										suffix = env.subst('$MEMFILESUFFIX'))
	env['BUILDERS']['Meminfo'] = memory_usage_builder
	
######## END Memory Usage BUILDER ########
	
# SCons mandatory function for custom tools
def exists(env):
    return True

def memory_usage_info(target, source, env):


	print "Analyzing Memory Usage From .map File"
	print ""
	
	filename = os.getcwd() + "\\output\\dbg\\" + env.subst('$TARGET_NAME') + ".map"
	print (filename)
	print ""
	print ""
	print ""
	if os.path.isfile(filename) == False:
		return
	
	rows = []
	rows.append(("Name","Address", "Size (byte)", "Used (byte)", "Percentage (Used)"))	
	
	line_info = get_line_info('flash_memory',filename)
	add_mem_usage(line_info, "flash_memory", rows)
	
	line_info = get_line_info('int_sram',filename)
	add_mem_usage(line_info, "int_sram", rows)
	print_table(rows)	
	
def get_line_info(title, filename):
	file = open(filename, "r")
	found = False
	for line in file.readlines():
		if 'MEMORY CONFIGURATION' in line:
			found=True

		if 'MEMORY usage analyzer' in line:
			found=True

		if found:
			if re.search(r'\b' + title + r'\b', line):
				file.close()
				return line
			if 'Linker script and memory map' in line:
				break
	file.close()
	return ''
	
def add_mem_usage(line_info, type, rows):
	total = 0
	used = 0
	remaining = 0
	per = 0
	
	buf = StringIO(line_info)
	
	while True:		
		line = buf.readline()
		if not line:
			break
		list_of_words = line.split('   ')
		Address		=	list_of_words[0]
		Size 		=	list_of_words[1]
		Used 		=	list_of_words[2].split('(') #str(list_of_words[2].replace('(',''))
		Used_byte	=	Used[0]
		Percentage	=	Used[1].replace(')','')
		Name 		= 	type
	rows.append((Name, Address, Size, Used_byte, str(Percentage) + " % of Full"))

def print_table(lines, separate_head=True):
	#Prints a formatted table given a 2 dimensional array
    #Count the column width
    widths = []
    for line in lines:
	    for i,size in enumerate([len(x) for x in line]):
				while i >= len(widths):
					widths.append(0)
				if size > widths[i]:
					widths[i] = size

	#Print line separator
    print("-"*(sum(widths)+3*(len(widths)-1)))
			
    #Generate the format string to pad the columns
    print_string = ""
    for i,width in enumerate(widths):
        print_string += "{" + str(i) + ":" + str(width) + "} | "
    if (len(print_string) == 0):
        return
    print_string = print_string[:-3]

    #Print the actual data
    for i,line in enumerate(lines):
        print(print_string.format(*line))
        if (i == 0 and separate_head):
            print("-"*(sum(widths)+3*(len(widths)-1)))
			
	#Print line separator
    print("-"*(sum(widths)+3*(len(widths)-1)))