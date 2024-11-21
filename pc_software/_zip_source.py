import os
import sys

zip_file_name = "duckypad_config_latest_source.zip"
os.system("rm -fv ./*.zip")
os.system(f"7z.exe a {zip_file_name} ./ -x!_* -x!setup.py")
