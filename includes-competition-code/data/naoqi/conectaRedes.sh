killall wpa_supplicant
wpa_supplicant -B -iwlan0 -c /home/nao/naoqi/puredebatata.conf -Dnl80211
dhclient wlan0
ifconfig wlan0 10.0.98.1 netmask 255.255.0.0 up
