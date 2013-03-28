#!/bin/bash

name=hd.png

unit_x=$1
unit_y=$2

l=100
n=0


function compute()
{
	width=`identify -format "%[fx:w]" $name`
	height=`identify -format "%[fx:h]" $name`

	xpart=`expr $width / $unit_x`
	ypart=`expr $height / $unit_y`

	tot=$(expr $unit_x \* $unit_y)
}

function draw()
{
	echo drawing
	mogrify \
			-draw "line 0,0,0,$l" \
			-draw "line 0,0,$l,0" \
			-draw "line $xpart,0,$(expr $xpart - $l),0" \
			-draw "line $(expr $xpart - 1),0,$(expr $xpart - 1),$l" \
			-draw "line 0,$(expr $ypart - 1),0,$(expr $ypart - $l)" \
			-draw "line 0,$(expr $ypart - 1),$l,$(expr $ypart - 1)" \
			-draw "line $(expr $xpart - 1),$(expr $ypart - 1),$(expr $xpart - $l),$(expr $ypart - 1)" \
			-draw "line $(expr $xpart - 1),$(expr $ypart - 1),$(expr $xpart - 1),$(expr $ypart - $l)" \
			+repage \
			"part-$n.png" 

	n=$(expr $n + 1)
}

# main

if [[ -e ./hd.png ]]
then
 	compute

	# crop
	convert $name 	-crop "$xpart"x"$ypart" \
			part.png

	# draw
	for i in $(seq $tot )
	do
		draw
		let i=i+1
	done
else
	echo "hd.png not found!"
fi




