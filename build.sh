#!/bin/bash

cd content;

set -e

files=$(find . -type f \
    ! -name "*.h" \
    ! -name "*.c" \
    ! -name "*.sh" \
    ! -name "*.png")

total=$(echo "$files" | wc -l)
current=0

for file in $files
do
    current=$((current + 1))

    out="${file%.*}.h"

    name="$(
        echo "$file" |
        sed 's|^\./||' |
        tr '/.-' '_'
    )"

    echo "packing $file to $out ($current/$total)"

    xxd -i -n "$name" "$file" > "$out"
done


images=$(find . -name "*.png")

img_total=$(echo "$images" | wc -l)
img_current=0

for img in $images
do
    img_current=$((img_current + 1))

    raw="${img%.png}.raw"
    header="${img%.png}.h"

    echo "converting $img ($img_current/$img_total)"

    convert "$img" RGBA:"$raw"

    name="$(
        echo "$raw" |
        sed 's|^\./||' |
        tr '/.-' '_'
    )"

    xxd -i -n "$name" "$raw" > "$header"

    rm "$raw"
done

images=$(find . -name "*.jpg")

img_total=$(echo "$images" | wc -l)
img_current=0

for img in $images
do
    img_current=$((img_current + 1))

    raw="${img%.jpg}.raw"
    header="${img%.jpg}.h"

    echo "converting $img ($img_current/$img_total)"

    convert "$img" RGBA:"$raw"

    name="$(
        echo "$raw" |
        sed 's|^\./||' |
        tr '/.-' '_'
    )"

    xxd -i -n "$name" "$raw" > "$header"

    rm "$raw"
done

cd ..; gcc engine/main.c include/glad.c -o bin -lGL -lglfw -lm -flto -Os -funroll-loops -fexpensive-optimizations -pipe -s
