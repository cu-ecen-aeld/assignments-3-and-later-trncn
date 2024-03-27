#!/bin/bash

# Check if required arguments are provided
if [ $# -ne 2 ]; then
    echo "Error: Two arguments are required."
    exit 1
fi

writefile=$1
writestr=$2

# Create directory if it doesn't exist
mkdir -p "$(dirname "$writefile")"

# Write content to file
echo "$writestr" > "$writefile"

# Check if file was created successfully
if [ $? -ne 0 ]; then
    echo "Error: Failed to create file $writefile"
    exit 1
fi

exit 0