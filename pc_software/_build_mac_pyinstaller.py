from glob import glob
import os
import PyInstaller.__main__
import shutil
import sys

if 'darwin' not in sys.platform:
    print("this script is for macOS only!")
    exit()

def clean(additional=None):
	removethese = ['__pycache__','build','dist','*.spec']
	if additional:
		removethese.append(additional)
	for _object in removethese:
		target=glob(os.path.join('.', _object))
		for _target in target:
			try:
				if os.path.isdir(_target):
					shutil.rmtree(_target)
				else:
					os.remove(_target)
			except:
				print(f'Error deleting {_target}.')

THIS_VERSION = None
try:
	mainfile = open('duckypad_config.py')
	for line in mainfile:
		if "THIS_VERSION_NUMBER =" in line:
			THIS_VERSION = line.replace('\n', '').replace('\r', '').split("'")[-2]
	mainfile.close()
except Exception as e:
	print('build_windows exception:', e)
	exit()

if THIS_VERSION is None:
	print('could not find version number!')
	exit()

exe_file_name = f"duckypad_config_{THIS_VERSION.replace('.', '_')}_macOS_ARM"

# --noconsole
clean(additional='duckypad*.zip')
PyInstaller.__main__.run(['duckypad_config.py','--icon=_icon.icns', '--onefile', f"--name={exe_file_name}"])


output_folder_path = os.path.join('.', "dist")
new_folder_path = exe_file_name

print(output_folder_path)
print(new_folder_path)

os.rename(output_folder_path, new_folder_path)

f = open(os.path.join(new_folder_path, "run.sh"), "w")
f.write("echo\n")
f.write("echo Welcome to duckyPad!\n")
f.write("echo\n")
f.write("echo To Connect, Please Authenticate.\n")
f.write("echo\n")
f.write("echo More info: duckyPad.com\n")
f.write("echo\n")
f.write(f"sudo ./{exe_file_name}\n")
f.close()
os.system(f"chmod a+x {os.path.join(new_folder_path, "run.sh")}")

zip_file_name = exe_file_name
shutil.make_archive(exe_file_name, 'zip', new_folder_path)

clean()

