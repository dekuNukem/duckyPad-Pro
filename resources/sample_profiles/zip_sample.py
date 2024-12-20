import os
import shutil
import sys

for file in os.listdir('.'):
    if file.endswith('.zip'):
        os.remove(file)
        print(f"Removed {file}")

current_os = sys.platform

if current_os.startswith('win'):
    shutil.make_archive('sample_profiles', 'zip', './sample_profiles')
    shutil.make_archive('keymaps', 'zip', './sample_profiles/keymaps')
elif current_os.startswith('darwin') or current_os.startswith('linux'):
    shutil.make_archive('sample_profiles', 'zip', './sample_profiles')
    shutil.make_archive('keymaps', 'zip', './sample_profiles/keymaps')
else:
    print("Unsupported OS")

