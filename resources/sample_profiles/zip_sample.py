import os
import sys
import platform

os.system("rm -fv ./*.zip")
current_os = platform.system()

if current_os == "Windows":
    os.system("7z a -r sample_profiles.zip ./sample_profiles/*")
elif current_os == "Darwin" or current_os == "Linux":
    os.system("zip -rv sample_profiles.zip ./sample_profiles/*")
else:
    print("Unsupported OS")