#!/bin/bash

if [ ! -f server/.env ]; then
    echo ".env file not found, please ask your system administrator for it."
fi

while IFS='=' read -r key value; do
    export $key=$value
done < server/.env