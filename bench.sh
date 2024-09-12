#!/usr/bin/env bash
set -o errexit

if [ "$#" -ne 2 ]; then
	echo "Usage: [-r/-t/-a] file"
	exit
fi


FMT="si" # si: 1000B==1K, iec: 1024B==1K

hyperfine -N --setup "vmtouch -t '$2'" -L buf 1,2,4,8 "./h4x $1 {buf} '$2'" --export-json bench.json

COUNT=$(ls -l "$2" | awk '{print $5}')
JSON=($(jq -r '.results[] | .mean, .stddev, .median, .min, .max' bench.json))

MEAN=$(echo "scale=6; $COUNT/${JSON[0]}" | bc | numfmt --to=$FMT --suffix=B --format="%.3f")
#STDDEV=$(echo "scale=6; $COUNT/${JSON[1]}" | bc | numfmt --to=$FMT --suffix=B --format="%.3f")
MEDIAN=$(echo "scale=6; $COUNT/${JSON[2]}" | bc | numfmt --to=$FMT --suffix=B --format="%.3f")
MIN=$(echo "scale=6; $COUNT/${JSON[3]}" | bc | numfmt --to=$FMT --suffix=B --format="%.3f")
MAX=$(echo "scale=6; $COUNT/${JSON[4]}" | bc | numfmt --to=$FMT --suffix=B --format="%.3f")

printf "mean: %s/second (mediam: %s, min: %s, max: %s)\n" $MEAN $MEDIAN $MIN $MAX
