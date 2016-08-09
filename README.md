# piMotionSensor

# install wiring pi

git clone git://git.drogon.net/wiringPi
cd wiringPi
./build

# compile code
gcc -o piMotion pi_motion.c -lwiringPi

# move motionsensor to /etc/init.d
sudo cp motionsensor /etc/init.d
sudo chmod +x /etc/init.d/motionsensor
sudo update-rc.d motionsensor defaults

