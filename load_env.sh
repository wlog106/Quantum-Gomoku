#!/bin/bash

if [ ! -f .env ]; then
    echo ".env file not found in current directory."
fi

while IFS='=' read -r key value; do
    export $key=$value
done < .env