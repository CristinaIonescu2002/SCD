docker run -d --name mosquitto-broker -p 1883:1883 \
-v ~/mqtt/mosquitto.conf:/mosquitto/config/mosquitto.conf \
eclipse-mosquitto
