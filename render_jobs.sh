#!/bin/bash

FILES=scenes/jobs/*.brhan
RENDER_TIMES=""

# Render each scene
for file in $FILES
do
	echo "Rendering:" $file
	START_TIME=$(date +%s)
	./build/BrhanRenderer $file
	END_TIME=$(date +%s)
	ELAPSED_TIME="$(( $END_TIME-$START_TIME ))"
	RENDER_TIMES="$RENDER_TIMES $ELAPSED_TIME"
done

echo "All render jobs finished successfully"

# Print render times
FILES_ARRAY=($FILES)
RENDER_TIMES_ARRAY=($RENDER_TIMES)
i="0"
for file in $FILES
do
	RENDER_TIME_MINUTES="$(( ${RENDER_TIMES_ARRAY[$i]}/"60" ))"
	echo ${FILES_ARRAY[$i]} ":" ${RENDER_TIMES_ARRAY[$i]} "second(s) =" $RENDER_TIME_MINUTES "minute(s)" 
	i="$(( $i+"1" ))"
done
