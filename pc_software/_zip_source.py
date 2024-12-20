import os
import sys
import platform

THIS_VERSION = None
try:
    mainfile = open('duckypad_config.py')
    for line in mainfile:
        if "THIS_VERSION_NUMBER =" in line:
            THIS_VERSION = line.replace('\n', '').replace('\r', '').split("'")[-2]
    mainfile.close()
except Exception as e:
    print('zip_source exception:', e)
    exit()

if THIS_VERSION is None:
    print('could not find version number!')
    exit()

zip_file_name = f"duckypad_config_{THIS_VERSION}_source.zip"

os.system("rm -fv ./*.zip")

current_os = platform.system()

if current_os == "Windows":
    os.system(f"7z.exe a {zip_file_name} ./ -x!_* -x!setup.py")
elif current_os == "Darwin" or current_os == "Linux":
    os.system(f"zip -r {zip_file_name} ./ -x '_*' -x 'setup.py'")
else:
    print("Unsupported OS")