import paho.mqtt.client as mqtt
import threading

BROKER_HOST = "mqtt.dfilip.xyz"
BROKER_PORT = 1883
TOPIC_SUBSCRIBE = "scd/chat/#"
NUME = "IonescuCristina-341C3"
TOPIC_PUBLISH = f"scd/chat/{NUME}" 


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectat cu succes la broker!")
        client.subscribe(TOPIC_SUBSCRIBE)
        print(f"Abonat la topicul: {TOPIC_SUBSCRIBE}")
    else:
        print(f"Conectare esuata cu codul: {rc}")

def on_message(client, userdata, msg):
    topic = msg.topic
    payload = msg.payload.decode("utf-8")
    print(f"[{topic}] {payload}")

def publish_messages(client):
    try:
        while True:
            message = input("Mesaj: ")
            if message.strip():
                client.publish(TOPIC_PUBLISH, message)
    except KeyboardInterrupt:
        print("\nIesire din aplicatie...")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER_HOST, BROKER_PORT, 60)

client.loop_start()

publish_messages(client)

client.loop_stop()
client.disconnect()
