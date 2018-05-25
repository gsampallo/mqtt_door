import paho.mqtt.client as mqtt
import time


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("SENSORPUERTA")
    

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    if(len(msg.payload) > 0):
        evento = str(msg.payload)
        if(evento.find("opened") > 1): 
            print("ALARMA!")
            client.publish("TIMBRE", "2")
        else:
            print(time.strftime("%d-%m-%Y %H:%M:%S", time.localtime())+" "+msg.topic+" "+str(msg.payload))

client = mqtt.Client()
client.username_pw_set("usuario", password="clave") 
client.on_connect = on_connect
client.on_message = on_message

client.connect("servidor", 1883, 60)

client.loop_forever()