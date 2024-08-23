import os
import sys
import hashlib
import shutil

def is_file_different(file1, file2):
    hash1 = hashlib.md5(open(file1,'rb').read()).hexdigest()
    hash2 = hashlib.md5(open(file2,'rb').read()).hexdigest()
    return hash1 != hash2

def compare(old_path, new_path):
    old_files = set([x for x in os.listdir(old_path)])
    new_files = set([x for x in os.listdir(new_path)])

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

    return files_to_add, files_to_delete, files_with_difference, files_in_both
    
"""

Files in new but not old: add to duckypad

Files in old but not new: delete from duckypad

Files in both but different content: delete from duckypad then write new version

"""

def delete_path(path):
    if os.path.exists(path) is False:
        return
    if 'dpp_config.txt' in path:
        return
    print("deleting", path)
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

def copy_file_if_exist(from_path, to_path):
    if os.path.exists(from_path):
        print(f"\tcopying from {from_path} to {to_path}")
        shutil.copy2(from_path, to_path)

def duckypad_file_sync(sd_dir, modified_dir):
    # top level dirs
    top_level_item_to_add, top_level_item_to_delete, top_level_item_with_difference, common_dirs = compare(sd_dir, modified_dir)
    # print("top_level_item_to_add:", top_level_item_to_add)
    # print("top_level_item_to_delete:", top_level_item_to_delete)
    # print("top_level_item_with_difference:", top_level_item_with_difference)
    # print("common_dirs:", common_dirs)
    top_level_to_copy = top_level_item_to_add | top_level_item_with_difference
    top_level_to_remove = top_level_item_to_delete | top_level_to_copy

    # print('----------------')
    # print("top_level_to_copy", top_level_to_copy)
    # print("top_level_to_remove", top_level_to_remove)

    for item in top_level_to_remove:
        item_path = os.path.join(sd_dir, item)
        # print("removing...", item_path)
        delete_path(item_path)
    
    for item in top_level_to_copy:
        to_copy_source_path = os.path.join(modified_dir, item)
        to_copy_destination_path = os.path.join(sd_dir, item)
        # print("to_copy_source_path:", to_copy_source_path)
        # print("to_copy_destination_path:", to_copy_destination_path)
        # nothing on top level is worth copying
        if os.path.isfile(to_copy_source_path):
            continue
        # this is a dir, create a new dir first
        # print("creating dir:", to_copy_destination_path)
        ensure_dir(to_copy_destination_path)
        source_subdir_file_list = [os.path.join(to_copy_source_path, x) for x in os.listdir(to_copy_source_path)]
        source_subdir_file_list = [d for d in source_subdir_file_list if os.path.isfile(d)]
        for source_file in source_subdir_file_list:
            print(f"copying {source_file} to {to_copy_destination_path}")
            shutil.copy2(source_file, to_copy_destination_path)

    for common_dir_name in common_dirs:
        files_to_add, files_to_delete, files_with_difference, common_files = compare(os.path.join(sd_dir, common_dir_name), os.path.join(modified_dir, common_dir_name))
        # print('===========', common_dir_name, '===========')
        # print("files_to_add", files_to_add)
        # print("files_to_delete", files_to_delete)
        # print("files_with_difference", files_with_difference)
        # print("common_files", common_files)

        subdir_file_to_remove = files_to_delete | files_with_difference
        subdir_file_to_remove.add("state.sps")
        subdir_file_to_remove.add("state_dpp.sps")

        for item in list(subdir_file_to_remove):
            if item.startswith("key"):
                subdir_file_to_remove.add(item.replace('.txt', '.dsb'))

        # print("subdir_file_to_remove:", subdir_file_to_remove)

        for item in subdir_file_to_remove:
            to_delete_path = os.path.join(sd_dir, common_dir_name)
            to_delete_path = os.path.join(to_delete_path, item)
            # print("\tdeleting...", to_delete_path)
            delete_path(to_delete_path)

        subdir_file_to_copy = files_to_add | files_with_difference

        for item in list(subdir_file_to_copy):
            if item.startswith("key"):
                subdir_file_to_copy.add(item.replace('.txt', '.dsb'))

        # print("subdir_file_to_copy:", subdir_file_to_copy)

        for item in subdir_file_to_copy:
            copy_from_path = os.path.join(modified_dir, common_dir_name)
            copy_from_path = os.path.join(copy_from_path, item)
            copy_to_path = os.path.join(sd_dir, common_dir_name)
            copy_to_path = os.path.join(copy_to_path, item)
            copy_file_if_exist(copy_from_path, copy_to_path)

# if len(sys.argv) != 3:
#     print(__file__, "sd_dir modified_dir")
#     exit()

# sd_dir = sys.argv[1]
# modified_dir = sys.argv[2]

# duckypad_file_sync(sd_dir, modified_dir)