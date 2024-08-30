import time
import platform

p = platform.system()

if p == 'Windows':
    import wmi
    import win32process
    import pygetwindow as gw
elif p == 'Darwin':
    from AppKit import NSWorkspace
    import Quartz
elif p == 'Linux':
    from ewmh import EWMH
    import psutil
    import Xlib
    NET_WM_NAME = Xlib.display.Display().intern_atom('_NET_WM_NAME')

def get_active_window():
    if p == 'Windows':
        return win_get_active_window()
    elif p == 'Darwin':
        return darwin_get_active_window()
    elif p == 'Linux':
        return linux_get_active_window()
    raise 'Platform %s not supported' % p

def get_list_of_all_windows():
    if p == 'Windows':
        return win_get_list_of_all_windows()
    elif p == 'Darwin':
        return darwin_get_list_of_all_windows()
    elif p == 'Linux':
        return linux_get_list_of_all_windows()
    raise 'Platform %s not supported' % p

def linux_get_list_of_all_windows():
    ret = set()
    ewmh = EWMH()
    for window in ewmh.getClientList():
        try:
            win_pid = ewmh.getWmPid(window)
        except TypeError:
            win_pid = False
        if win_pid:
            app = psutil.Process(win_pid).name()
        else:
            app = 'Unknown'
        wm_name = window.get_wm_name()
        if not wm_name:
            wm_name = window.get_full_property(NET_WM_NAME, 0).value
        if not wm_name:
            wm_name = 'class:{}'.format(window.get_wm_class()[0])
        if isinstance(wm_name, bytes):
            wm_name = wm_name.decode('utf-8')
        ret.add((app, wm_name))
    return ret

def linux_get_active_window():
    ret = set()
    ewmh = EWMH()
    active_window = ewmh.getActiveWindow()
    if not active_window:
        return '', ''
    try:
        win_pid = ewmh.getWmPid(active_window)
    except TypeError:
        win_pid = False
    except Xlib.error.XResourceError:
        return '', ''
    wm_name = active_window.get_wm_name()
    if not wm_name:
        wm_name = active_window.get_full_property(NET_WM_NAME, 0).value
    if not wm_name:
        wm_name = 'class:{}'.format(active_window.get_wm_class()[0])
    if isinstance(wm_name, bytes):
        wm_name = wm_name.decode('utf-8')
    if win_pid:
        active_app = psutil.Process(win_pid).name()
    else:
        return '', wm_name
    return (active_app, wm_name)

def darwin_get_active_window():
    windows = Quartz.CGWindowListCopyWindowInfo(
        Quartz.kCGWindowListExcludeDesktopElements | Quartz.kCGWindowListOptionOnScreenOnly, Quartz.kCGNullWindowID)
    for window in windows:
        if window[Quartz.kCGWindowLayer] == 0:
            return window[Quartz.kCGWindowOwnerName], window.get(Quartz.kCGWindowName, 'unknown')
    return '', ''

def darwin_get_list_of_all_windows():
    apps = []
    windows = Quartz.CGWindowListCopyWindowInfo(
        Quartz.kCGWindowListExcludeDesktopElements | Quartz.kCGWindowListOptionOnScreenOnly, Quartz.kCGNullWindowID)

    for window in windows:
        apps.append((window[Quartz.kCGWindowOwnerName],
                    window.get(Quartz.kCGWindowName, 'unknown')))
    apps = list(set(apps))
    apps = sorted(apps, key=lambda x: x[0])
    return apps

def win_get_app_name(hwnd):
    """Get application name given hwnd."""
    c = wmi.WMI()
    try:
        _, pid = win32process.GetWindowThreadProcessId(hwnd)
        for p in c.query('SELECT Name FROM Win32_Process WHERE ProcessId = %s' % str(pid)):
            exe = p.Name.rsplit('.', 1)[0]
            break
    except:
        return 'unknown'
    else:
        return exe

def win_get_list_of_all_windows():
    ret = set()
    for item in gw.getAllWindows():
        ret.add((win_get_app_name(item._hWnd), item.title))
    ret = sorted(list(ret), key=lambda x: x[0])
    return ret

def win_get_active_window():
    active_window = gw.getActiveWindow()
    if active_window is None:
        return '', ''
    return (win_get_app_name(active_window._hWnd), active_window.title)

# print(get_list_of_all_windows())
