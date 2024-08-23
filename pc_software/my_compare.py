import os
import sys
import hashlib
import shutil

duckypad_file_whitelist = [
    "profile",
    "config",
    "key",
]

duckypad_file_blacklist = [
    "keymaps",
]

def is_duckypad_file(name):
    for item in duckypad_file_blacklist:
        if item in name:
            return False
    for item in duckypad_file_whitelist:
        if item in name:
            return True
    return False

def is_file_different(file1, file2):
    hash1 = hashlib.md5(open(file1,'rb').read()).hexdigest()
    hash2 = hashlib.md5(open(file2,'rb').read()).hexdigest()
    return hash1 != hash2

def compare(old_path, new_path):
    old_files = set([x for x in os.listdir(old_path) if is_duckypad_file(x)])
    new_files = set([x for x in os.listdir(new_path) if is_duckypad_file(x)])

    # files in new but not old
    files_to_add = new_files - old_files
    # files in old but not new
    files_to_delete = old_files - new_files

    files_in_both = new_files.intersection(old_files)
    files_with_difference = set()

    for item in files_in_both:
        new_full_path = os.path.join(new_path, item)
        old_full_path = os.path.join(old_path, item)
        if os.path.isdir(new_full_path):
            continue
        if is_file_different(old_full_path, new_full_path):
            files_with_difference.add(item)

    common_dirs = [x for x in files_in_both if os.path.isdir(os.path.join(new_path, x))]

    return files_to_add, files_to_delete, files_with_difference, set(common_dirs)
    # print("files_to_add:", files_to_add)
    # print("files_to_delete:", files_to_delete)
    # print("files_with_difference:", files_with_difference)
    
"""

Files in new but not old: add to duckypad

Files in old but not new: delete from duckypad

Files in both but different content: delete from duckypad then write new version

"""

def delete_path(path):
    if os.path.exists(path) is False:
        return
    if os.path.isfile(path):
        os.remove(path)
    elif os.path.isdir(path):
        shutil.rmtree(path)

def get_file_content(file_path):
    this_file = open(file_path, 'rb')
    content = this_file.read()
    this_file.close()
    return content

def ensure_dir(dir_path):
    if not os.path.exists(dir_path):
        os.makedirs(dir_path)

def duckypad_file_sync(sd_dir, modified_dir):
    files_to_add, files_to_delete, files_with_difference, common_dirs = compare(sd_dir, modified_dir)
    print("file to add:", files_to_add)
    print("file to delete:", files_to_delete)
    print("file differs:", files_with_difference)
    print("unchanged:", common_dirs)
    top_level_to_copy = files_to_add | files_with_difference
    top_level_to_remove = files_to_delete | top_level_to_copy

    print('----------------')
    print("top_level_to_copy", top_level_to_copy)
    print("top_level_to_remove", top_level_to_remove)

    for item in top_level_to_remove:
        item_path = os.path.join(sd_dir, item)
        print("removing...", item_path)
        delete_path(item_path)
    
    for item in top_level_to_copy:
        to_copy_source_path = os.path.join(modified_dir, item)
        to_copy_destination_path = os.path.join(sd_dir, item)
        print("to_copy_source_path:", to_copy_source_path)
        print("to_copy_destination_path:", to_copy_destination_path)
        if os.path.isfile(to_copy_source_path):
            print("writing...", to_copy_source_path)
            # duckypad_open_file_for_writing(item)
            # duckypad_write_file(get_file_content(to_copy_source_path))
            # duckypad_close_file()
            continue
        # this is a dir, create a new dir first
        print("creating dir:", to_copy_destination_path)
        ensure_dir(to_copy_destination_path)
        source_subdir_file_list = [os.path.join(to_copy_source_path, x) for x in os.listdir(to_copy_source_path)]
        source_subdir_file_list = [d for d in source_subdir_file_list if os.path.isfile(d)]
        for source_file in source_subdir_file_list:
            print(f"copying {source_file} to {to_copy_destination_path}")
            shutil.copy2(source_file, to_copy_destination_path)

if len(sys.argv) != 3:
    print(__file__, "sd_dir modified_dir")
    exit()

sd_dir = sys.argv[1]
modified_dir = sys.argv[2]

# files_to_add, files_to_delete, files_with_difference, common_dirs = compare(sd_dir, modified_dir)



duckypad_file_sync(sd_dir, modified_dir)