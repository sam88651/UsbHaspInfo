# UsbHaspInfo
Supplimental tool for UsbHasp discovered in Internet

What does it do?
  It shows info about locally attached USB HASP keys. Output in a format of Nagios plugin.
  
Requirements:
  Due to the fact that Linux HASP4 API is old enough and any longer unsupported by vendor only 32-bits Linux API library is available. So, to run it on x64 system you've gotta install i386 support. Do somethig like this:
  
  sudo dpkg --add-architecture i386
  sudo apt-get update
  sudo apt-get install libc6:i386 libncurses5:i386 libstdc++6:i386

Also the API mentioned above works via licence manager API. Hasplm and aksusbd drivers must be installed on a computer usbhaspinfo runs.

Usage:
  usbhaspinfo <-c path_to_nethasp.ini>
 If no -c option given nethasp.ini file read from /etc/haspd
 
Example output:
  HASP OK: HASP4 Time (XXXXXXXX network 50 0) | SN_XXXXXXXX=0;35;45
  
Where XXXXXXXX - key's serial, next type (local/network), number of available licenses, number of licenses used. Nagios performance data - serial, number of licenses taken, warning threshold, error threshold.


