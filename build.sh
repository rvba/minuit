#/bin/bash

echo "m i n u i t "
echo "install dependencies ? "
read -r input
if [[ $input == "y" ]];then
	sudo apt-get install 	build-essential \
				cmake\
				libgl1-mesa-dev\
				freeglut3-dev\
				libjpeg62-dev\
				libpng12-dev\
				libxmu-dev\
				libxi-dev
fi

echo "bin name:"
read -r name
echo "build name:"
read -r input
cd ..
mkdir $input
cd $input
echo "cmaking..."
cmake ../$name
echo "making..."
make
echo "done."


