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

sh_content = f"""
echo
echo ---------------
echo Welcome to duckyPad!
echo
echo To Connect, Please Authenticate.
echo
echo If Blocked:
echo "    Go to Settings > Privacy & Security"
echo "    Scroll down, click \"Allow Anyway\""
echo
echo More info: duckyPad.com
echo ---------------
echo
sudo ./{exe_file_name}
"""

with open(os.path.join(new_folder_path, "run.sh"), "w") as f:
	f.write(sh_content)
os.system(f"chmod a+x {os.path.join(new_folder_path, "run.sh")}")

readme_content = """

Launching this app on macOS:

https://dekunukem.github.io/duckyPad-Pro/doc/linux_macos_notes.html

Full User Manual:

https://dekunukem.github.io/duckyPad-Pro/doc/getting_started.html

"""

with open(os.path.join(new_folder_path, "README.txt"), "w") as f:
	f.write(readme_content)

zip_file_name = exe_file_name
shutil.make_archive(exe_file_name, 'zip', new_folder_path)

clean()

