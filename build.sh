#/bin/bash
sudo apt-get install 	build-essential \
			cmake\
			libgl1-mesa-dev\
			freeglut3-dev\
			libjpeg62-dev\
			libpng12-dev\
			libxmu-dev\
			libxi-dev\

mkdir minuit-build
cd minuit-build
cmake ../minuit-dev
make


