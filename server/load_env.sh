#!/bin/bash

if [ ! -f .env ]; then
    echo ".env file not found, please ask your system administrator for it."
    exit 1
fi

while IFS='=' read -r key value; do
    export "$key=$value"
done < .env