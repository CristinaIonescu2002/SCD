#!/bin/bash

set -e

if ! docker info | grep -q "Swarm: active"; then
    echo "Start Docker Swarm..."
    docker swarm init
fi

export SCD_DVP=/var/lib/docker/volumes

echo "Construim imaginea pentru mqtt_adaptor..."
docker build -t scd3/mqtt_adaptor ./mqtt_adaptor

echo "Deploy app..."
docker stack deploy -c stack.yml scd3

echo "App started!"
echo "Accesează Grafana la http://localhost:3000 cu utilizatorul 'asistent' și parola 'grafanaSCD2024'."
