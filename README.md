# FHEM-MQTT-ESP8266_Power_Consumption


```
# keller ------------------------------------------------
#
# --------strom_debug -----------------------------
#
define strom_debug dummy
#attr strom_debug room keller
#attr strom_debug webCmd on:off
#
#define mqtt_strom_debug MQTT_BRIDGE strom_debug
#attr mqtt_strom_debug room keller
#attr mqtt_strom_debug IODev mqtt
#attr mqtt_strom_debug publishState fhem/keller/debug/set
#attr mqtt_strom_debug stateFormat transmission-state
#
# --------strom_1 -----------------------------
#
define strom_1 dummy
attr strom_1 room keller
attr strom_1 webCmd on:off
#
define mqtt_strom_1 MQTT_BRIDGE strom_1
attr mqtt_strom_1 IODev mqtt
attr mqtt_strom_1 room keller
attr mqtt_strom_1 stateFormat transmission-state
attr mqtt_strom_1 subscribeSet fhem/keller/strom/zaehler1
#
define CN_strom1 HourCounter strom_1:on strom_1:off
attr CN_strom1 room keller
#
define CN_strom1.kwh HourCounter CN_strom1:75
attr CN_strom1.kwh room keller
#
define n_CN_strom1 notify CN_strom1:75 set CN_strom1 clear
#
#define CN.Strom1.File FileLog ./log/CN.Strom1-%Y.log (CN\.Strom1:.*)
# CN.Strom1.File  room keller
#
# --------strom_1_verbrauch -----------------------------
#
define strom_1_verbrauch dummy
attr strom_1_verbrauch room keller
#
define m_strom_1_verbrauch notify strom_1:on {\
   my $schaltzeit = \
   ReadingsVal("CN_strom1","pauseTimeEdge","0")+ReadingsVal("CN_strom1","pulseTimeEdge","0");;\
   my $momentanverbrauch = int(3600000/75/$schaltzeit);;\
   fhem("set strom_1_verbrauch $momentanverbrauch");;\
}
#
define fileLog_strom_1_verbrauch FileLog ./log/strom_1_verbrauch-%Y-%m-%d.log strom_1_verbrauch
attr fileLog_strom_1_verbrauch logtype text
attr fileLog_strom_1_verbrauch room keller
#
# --------strom_2 -----------------------------
#
define strom_2 dummy
attr strom_2 room keller
attr strom_2 webCmd on:off
#
define mqtt_strom_2 MQTT_BRIDGE strom_2
attr mqtt_strom_2 IODev mqtt
attr mqtt_strom_2 room keller
attr mqtt_strom_2 stateFormat transmission-state
attr mqtt_strom_2 subscribeSet fhem/keller/strom/zaehler2
#
define CN_strom2 HourCounter strom_2:on strom_2:off
attr CN_strom2 room keller
#
define CN_strom2_kwh HourCounter CN_strom2:150
attr CN_strom2_kwh room keller
#
define n_CN_strom2 notify CN_strom2:150 set CN_strom2 clear
#
# --------strom_2_verbrauch -----------------------------
#
define strom_2_verbrauch dummy
attr strom_2_verbrauch room keller
#
define m_strom_2_verbrauch notify strom_2:on {\
   my $schaltzeit = \
   ReadingsVal("CN_strom2","pauseTimeEdge","0")+ReadingsVal("CN_strom2","pulseTimeEdge","0");;\
   my $momentanverbrauch = int(3600000/150/$schaltzeit);;\
   fhem("set strom_2_verbrauch $momentanverbrauch");;\
}
#
define fileLog_strom_2_verbrauch FileLog ./log/strom_2_verbrauch-%Y-%m-%d.log strom_2_verbrauch
attr fileLog_strom_2_verbrauch logtype text
attr fileLog_strom_2_verbrauch room keller
define SVG_fileLog_strom_1_verbrauch_1 SVG fileLog_strom_1_verbrauch:SVG_fileLog_strom_1_verbrauch_1:CURRENT
attr SVG_fileLog_strom_1_verbrauch_1 room keller
```
