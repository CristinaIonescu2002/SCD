#!/bin/bash

set -e  # Opreste scriptul la prima eroare

STACK_NAME="scd3"

echo "Oprirea si curatarea serviciilor definite in $STACK_NAME..."

if docker stack ls | grep -q "$STACK_NAME"; then
    echo "Eliminam stack-ul $STACK_NAME..."
    docker stack rm $STACK_NAME
    echo "Asteptam ca serviciile sa fie eliminate..."
    sleep 10
else
    echo "Stack-ul $STACK_NAME nu este activ."
fi

echo "Eliminam containerele asociate..."
for container in $(docker ps -a --filter "name=${STACK_NAME}_" --format "{{.ID}}"); do
    echo "Eliminam containerul $container..."
    docker rm -f $container || true
done

echo "Eliminam retelele asociate stack-ului..."
for network in $(docker network ls --filter "label=com.docker.compose.project=${STACK_NAME}" --format "{{.Name}}"); do
    echo "Eliminam reteaua $network..."
    docker network rm $network || true
done

echo "GATA"
