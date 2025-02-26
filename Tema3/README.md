#### Tema 3 SCD
#### Ionescu Cristina 

- tema nu este completa, am reusit sa fac doar:

1. Integrarea in stack a solutiilor open-source:

- serviciile sunt integrate intr-un stack de Docker Swarm
- am folosit imaginile de docker: 
    - Mosquitto - pt schimbul de mesaje intre dispozitive
    - InfluxDB - baza de date time-series pentru stocarea datelor IoT
    - Grafana - pentru vizualizarea datelor din InfluxDB
    - MQTT Adapter - adaptor pentru preluarea datelor MQTT si scrierea
    lor in InfluxDB (imagine personalizata)
- primele 3 folosesc imagini oficiale de Docker, a 3-a este personalizata

2. Dezvoltarea adaptorului si integrarea componentelor avand ca
rezultat stocarea valorilor în baza de date

- am implementat in python ("mqtt_to_influx.py") un adaptor care
conecteaza Mosquitto la InfluxDB.
- adaptorul se conecteaza la Mosquitto si asculta mesajele de pe topicul "UPB/#" si preia datele transmise de dispozitive
- mesajele sunt primite in format JSON, cele valide sunt prelucrate
- transforma mesajele intr-un format acceptat de InfluxDB si le scrie intr-un bucket-ul "iot_data"

- adaptorul include:
    - Exlipse Mosquitto (broker MQTT)
    - InfluxDB (baza de date)


3. Configurarea interfetei de vizualizare avand ca rezultat cel putin
un dashboard functional

- am reusit sa fac cat de cat ceva
- m-am chinuit mult sa fac volumele consistente pentru ca nu mi-am
dat seama ca sawrm-ul imi adauga scd3_ in fata la numele volumelor


4. Realizarea corecta a dashboard-urilor default

- asta nu este facuta

5. Separarea corecta a traficului intre containere

- asta este separarea
```
networks:
  mqtt_network:
    driver: overlay
    internal: true
  data_network:
    driver: overlay
    internal: true
```