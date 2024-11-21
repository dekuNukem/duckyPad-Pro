import os
import sys
import platform

zip_file_name = "duckypad_config_latest_source.zip"
os.system("rm -fv ./*.zip")

current_os = platform.system()

if current_os == "Windows":
    os.system(f"7z.exe a {zip_file_name} ./ -x!_* -x!setup.py")
elif current_os == "Darwin" or current_os == "Linux":
    os.system(f"zip {zip_file_name} ./ -x '_*' -x 'setup.py'")
else:
    print("Unsupported OS")