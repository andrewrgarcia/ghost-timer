#!/bin/bash

read -p "Enter hours: " hours
read -p "Enter minutes: " minutes
read -p "Enter seconds: " seconds

total_seconds=$(( hours * 3600 + minutes * 60 + seconds ))

if [[ $total_seconds -le 0 ]]; then
    echo "Invalid time duration!"
    exit 1
fi

{
    while [[ $total_seconds -ge 0 ]]; do
        hh=$(printf "%02d" $(( total_seconds / 3600 )))
        mm=$(printf "%02d" $(( (total_seconds % 3600) / 60 )))
        ss=$(printf "%02d" $(( total_seconds % 60 )))

        echo "$hh:$mm:$ss" | osd_cat --pos=top --align=right --offset=50 \
            --font="-misc-fixed-bold-r-normal--34-*-*-*-*-*-iso8859-1" \
            --color=white --shadow=2 --outline=2 --delay=9999 &  # Keeps it on-screen

        sleep 1
        total_seconds=$(( total_seconds - 1 ))

        # Kill previous instance only the latest is visible
        pkill -f "osd_cat"
    done

    echo "Time's up!" | osd_cat --pos=top --align=right --offset=50 \
        --font="-misc-fixed-bold-r-normal--34-*-*-*-*-*-iso8859-1" \
        --color=red --shadow=2 --outline=2 --delay=5

} &  # Runs in the background

