#!/bin/bash
projPath=$1
folder="Logs"
name="testsLogged-"
inc=1
extension=".txt"

mkdir -p $folder

for f in $(ls $folder | grep "testsLogged"); do
	inc=$((inc+1))
done
path=$(echo "$folder/$name$inc$extension")

echo "Starting..."
./run.sh $projPath T-auto-tests/ > $path
echo "Finished!"
