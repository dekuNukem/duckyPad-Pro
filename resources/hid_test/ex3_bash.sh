#!/bin/bash
# Example of interacting with duckyPad using (bash) shell
#
# This can serve as a starting point for users aiming for
# implementation of profile auto-switching using bash and
# some common utils (`xdotools`, `pgrep`..)
#
# refer to https://github.com/dekuNukem/duckyPad-profile-autoswitcher/blob/master/HID_details.md for details
#
function get_duckyPad {
  # find out which /dev/hidraw points to duckyPad
  hid=$(find /sys/class/hidraw -ls | sed -n '/0483:D11C/s/.*\///p')
  if [ -z "${hid}" ]
  then
    echo "duckyPad not found" >&2
    return 1
  fi
 echo "/dev/${hid}"
}


function pad {
  # pad payload to 64 bytes
  printf "%-64s" "$(echo -en $1)" | sed 's/ /\x0/g'
}


function hidwrite { 
  # write data ($1) to duckyPad ($2)
  if [ $# -lt 2 ]
  then
    echo "Must specify two arguments for hidwrite" >&2
    exit 1
  fi 
  exec 3<> "$2"
  pad "$1" >&3
  read -u3 -t1 output
  result=$(echo -n "$output" | od -j2 -An -N1 -i)
  [ -z "${result}" ] && result=0
  if [ "${result}" -eq 1 ]
  then
    echo "ERROR" >&2
    return 1
  elif [ "${result}" -eq 2 ]
  then
    echo "BUSY" >&2
    return 2
  fi 
}


function get_fw {
  # read installed firmware version
  read major minor < <(dp_info | awk '{print substr($0, 4, 2), substr($0, 2, 2)}')
  major=$(echo $((16#$major)))
  minor=$(echo $((16#$minor)))
  echo "${major}.${minor}"
}


function get_curr_profile {
  # read currently active profile
  read profile < <(dp_info | awk '{print substr($0, 24, 2)}')
  profile=$(echo $((16#$profile)))
  echo "$profile"
}


function dp_info {
  # Info (0x00)
  hid=$(get_duckyPad)
  [ -z "${hid}" ] && exit 1
  exec 3<> "$hid"
  pad $'\x5\x1\x0' >&3
  read -u3 -t1 output
  echo -n "${output}" | od -An -tx
}


function goto_profile {
  # Goto profile (0x01)
  profile=$(printf '%02X' $1)
  hid=$(get_duckyPad)
  [ -z "${hid}" ] && exit 1
  hidwrite $'\x5\x1\x1'"\x${profile}" "$hid"
}


function prev_profile {
  # Previous profile (0x02)
  hid=$(get_duckyPad)
  [ -z "${hid}" ] && exit 1
  hidwrite $'\x5\x1\x2' "$hid"
}


function next_profile {
  # Next profile (0x02)
  hid=$(get_duckyPad)
  [ -z "${hid}" ] && exit 1
  hidwrite $'\x5\x1\x3' "$hid"
}
 
 
echo "Retrieving info from duckyPad.." 
dp_info
echo "Firmware: $(get_fw), current_profile: $(get_curr_profile)"
sleep 2

echo "Switching to profile 1.."
goto_profile 1
sleep 2

echo "Switching to next profile.."
next_profile
sleep 2

echo "Switching to previous profile.."
prev_profile
sleep 2

echo "Switch to profile 3 if current window belongs to Firefox.."
xdotool getwindowfocus getwindowname | grep -q 'Mozilla Firefox$'
if [ $? -eq 0 ]
then
  goto_profile 3
  sleep 2
fi

echo "Switch to profile 5 if xterm process is running.."
pgrep -x xterm >/dev/null
if [ $? -eq 0 ]
then
  goto_profile 5
  sleep 2
fi
