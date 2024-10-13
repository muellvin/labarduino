#!/bin/bash

# Define the serial device and output file
SERIAL_DEVICE="/dev/ttyACM0"
OUTPUT_FILE="output.txt"

# Ensure the serial device exists
if [ ! -e "$SERIAL_DEVICE" ]; then
  echo "Error: Serial device $SERIAL_DEVICE not found!"
  exit 1
fi

# Read from the serial device, print to terminal, and save to output file
while read -r line; do
  echo "$line"
done < "$SERIAL_DEVICE" | tee "$OUTPUT_FILE"
