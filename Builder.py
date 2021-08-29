import os
import json
import time
import subprocess


project_dir = os.path.abspath(os.path.dirname(__file__))

with open(os.path.join("Config.json"), "r") as json_file:
	file = json.load(json_file)

##-----Json file tokens------
Includes = file["Includes"]
Lib = file["Library"]
Libs = file["Libs"]
Type = file["Type"]
LibBuild = file["LibBuild"]
Bit = file["Bit"]
C_Cpp_file_paths = file["C/Cpp_files"]
Output_file = file["Output_file"]

################################### Compiling ########################################33

if Type == "Cpp":
	_compile = "g++ "
elif Type == "C":
	_compile = "gcc "

# Adding the build bit (32 | 64)
if Bit == "32":
	_compile += "-m32 "
elif Bit == "64":
	_compile += "-m64 "

# Adding include folder
for i in Includes:
	_compile += f"\"-I{i}\" "

_compile += "-c "

#Compiling the cpp files
C_Cpp_files = []

for i in C_Cpp_file_paths:
	src_files = os.listdir(i)
	
	for j in src_files:
		if ".h" not in j:
			C_Cpp_files.append(i+"/"+j)

for i in C_Cpp_files:
	_compile += f"\"{i}\" " 

print(_compile)

_subprocess = subprocess.Popen(_compile, shell=True, stdout=subprocess.PIPE)
subprocess_return = _subprocess.stdout.read()


# Creating the build dir
if not os.path.exists("Build"):
	os.mkdir("Build")


################################### Building Exe file ########################################33
if not LibBuild:
	if Type == "Cpp":
		_type = "g++"
	elif Type == "C":
		_type = "gcc" 

	_build = f"{_type} "

	# Adding the build bit (32 | 64)
	if Bit == "32":
		_build += "-m32 "
	elif Bit == "64":
		_build += "-m64 "

	_build += f"-o \"Build/{Output_file}\" "

	# Taking the .o files
	obj_files = []
	files = os.listdir(project_dir)

	for j in files:
		if ".o" in j:
			obj_files.append(j)

	# Adding .o files
	for i in obj_files:
		_build += f"{i} "

	# Adding library folder
	for i in Lib:
		_build += f"\"-L{i}\" "

	# Adding libraries
	for i in Libs:
		_build += f"{i} "

	print(_build)
	_subprocess = subprocess.Popen(_build, shell=True, stdout=subprocess.PIPE)
	subprocess_return = _subprocess.stdout.read()

################################## Building a lib file ###########################
else:
	#-- Extracting the external static libs
	libExtract = "ar -x "

	for path in Lib:
		total_libs = os.listdir(path)
		for i in Libs:
			if i in total_libs:
				libExtract += f"{path}/{i} "

	print(libExtract)
	_subprocess = subprocess.Popen(libExtract, shell=True, stdout=subprocess.PIPE)
	subprocess_return = _subprocess.stdout.read()

	#--- Building the lib
	libBuild = "ar ru"
	
	# Adding the lib name
	libBuild += f" \"Build/{Output_file}\" "

	# Taking the .o files
	obj_files = []
	files = os.listdir(project_dir)

	for j in files:
		if ".o" in j:
			obj_files.append(j)

	# Adding .o files
	for i in obj_files:
		libBuild += f"{i} "

	print(libBuild)
	_subprocess = subprocess.Popen(libBuild, shell=True, stdout=subprocess.PIPE)
	subprocess_return = _subprocess.stdout.read()

################################### Cleaning ########################################
for i in obj_files:
	if os.path.isfile(i):
		os.remove(f"{project_dir}/{i}")

# Opening the exe file

if not LibBuild:
	os.chdir("Build")
	os.system(Output_file)
