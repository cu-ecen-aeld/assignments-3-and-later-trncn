#!/bin/sh

# Check if required arguments are provided
if [ $# -ne 2 ]; then
    echo "Error: Two arguments are required."
    exit 1
fi

filesdir=$1
searchstr=$2

# Check if filesdir exists and is a directory
if [ ! -d "$filesdir" ]; then
    echo "Error: $filesdir is not a directory."
    exit 1
fi

# Find matching lines in filesdir
matching_lines=$(grep -r "$searchstr" "$filesdir")

# Count the number of files and matching lines
num_files=$(find "$filesdir" -type f | wc -l)
num_matching_lines=$(echo "$matching_lines" | wc -l)

# Print the results
echo "The number of files are $num_files and the number of matching lines are $num_matching_lines"
