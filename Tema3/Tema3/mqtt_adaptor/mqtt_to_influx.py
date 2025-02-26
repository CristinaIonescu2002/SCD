import os
import json
from datetime import datetime
import paho.mqtt.client as mqtt
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS

MQTT_BROKER = "mqtt_broker"
MQTT_PORT = 1883
MQTT_TOPIC = "#"

INFLUX_HOST = "http://influxdb:8086"
INFLUX_TOKEN = os.getenv("INFLUX_TOKEN", "cris1234")
INFLUX_ORG = os.getenv("INFLUX_ORG", "my-org")
INFLUX_BUCKET = os.getenv("INFLUX_BUCKET", "iot_data")

DEBUG = os.getenv("DEBUG_DATA_FLOW", "false").lower() == "true"

import logging
logging.basicConfig(level=logging.DEBUG if DEBUG else logging.INFO)

try:
    influx_client = InfluxDBClient(url=INFLUX_HOST, token=INFLUX_TOKEN, org=INFLUX_ORG)
    write_api = influx_client.write_api(write_options=SYNCHRONOUS)
    logging.info("Connected to InfluxDB.")
except Exception as e:
    logging.error(f"Failed to connect to InfluxDB: {e}")
    raise e

def parse_message(topic, payload):
    try:
        logging.debug(f"Parsing message: {payload}")
        data = json.loads(payload)
        timestamp = data.pop("timestamp", datetime.utcnow().isoformat())

        try:
            datetime.fromisoformat(timestamp.replace("Z", "+00:00"))
        except ValueError as e:
            logging.error(f"Invalid timestamp: {timestamp}. Error: {e}")
            return []

        points = []
        for key, value in data.items():
            if isinstance(value, (int, float)):
                point = (
                    Point(topic.replace("/", "."))
                    .field(key, value)
                    .time(timestamp, WritePrecision.NS)
                    .tag("location", topic.split("/")[0])
                    .tag("topic", topic.split("/")[1])
                )
                points.append(point)
                logging.debug(f"Generated point: {point.to_line_protocol()}")
        return points
    except Exception as e:
        logging.error(f"Error parsing message: {e}")
        return []

def on_message(client, userdata, msg):
    logging.info(f"Received message on topic {msg.topic}: {msg.payload.decode()}")
    points = parse_message(msg.topic, msg.payload.decode())
    if points:
        try:
            logging.info(f"Writing points: {[p.to_line_protocol() for p in points]}")
            write_api.write(bucket=INFLUX_BUCKET, org=INFLUX_ORG, record=points)
            logging.info(f"Successfully wrote {len(points)} points to InfluxDB")
        except Exception as e:
            logging.error(f"Failed to write points to InfluxDB: {e}")
    else:
        logging.warning("No points generated from message.")

mqtt_client = mqtt.Client()
mqtt_client.on_message = on_message
try:
    mqtt_client.connect(MQTT_BROKER, MQTT_PORT)
    logging.info(f"Connected to MQTT broker at {MQTT_BROKER}:{MQTT_PORT}.")
except Exception as e:
    logging.error(f"Failed to connect to MQTT broker: {e}")
    raise e

mqtt_client.subscribe(MQTT_TOPIC)

logging.info("MQTT to InfluxDB Adaptor started...")
mqtt_client.loop_forever()
