import os
import time
import sys
import psutil
import concurrent.futures

def format_and_copy(item, sample_profile_path):
    """Worker function to process a single drive."""
    mountpoint = item.mountpoint
    print(f"Starting work on removable drive: {mountpoint}")
    
    # Wipe the drive
    os.system(f"del /s /q {mountpoint}*.* && rmdir /s /q {mountpoint}")
    
    # Copy the new files
    os.system(f"xcopy {sample_profile_path} {mountpoint} /e /i /y")
    
    print(f"Finished updating: {mountpoint}")

def main():
    # Find all FAT-formatted removable drives
    removable_drives = [x for x in psutil.disk_partitions() if ('removable' in x.opts.lower() and 'fat' in x.fstype.lower())]
    
    if not removable_drives:
        print("No removable drives found")
        sys.exit()

    sample_profile_path = "C:\\Users\\allen\\Desktop\\repos\\duckyPad2\\resources\\sample_profiles\\sample_profiles\\"

    print(f"Found {len(removable_drives)} drive(s). Processing in parallel...")

    # Use ThreadPoolExecutor to run the tasks concurrently
    with concurrent.futures.ThreadPoolExecutor() as executor:
        # Create a list of futures (tasks)
        futures = []
        for item in removable_drives:
            # Submit the task to the thread pool
            future = executor.submit(format_and_copy, item, sample_profile_path)
            futures.append(future)
            
        # Wait for all threads to complete
        concurrent.futures.wait(futures)

    print("All drives successfully updated!")

if __name__ == "__main__":
    main()