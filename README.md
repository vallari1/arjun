# Arjun - Mecanum bot


For testing, 
1. Put pin values in .ino file
2. Upload code to mega from laptop, and then connect to Raspberry Pi.
3. Connect the dongle for joystick / connect through bluetooth. (bluetoothctl :( )
4. To heck if joystick is working and to know button numbers: (make sure it is in redgear mode)
   `sudo apt install joystick jstest-gtk evtest`

Workspace:
```
mkdir -p ~/arjun/src
cd ~/arjun/src
git clone https://github.com/vallari1/arjun.git
cd .. 
colcon build --symlink-install
source install/setup.bash
ros2 launch mobile_project_pkg mec_bot.launch.py 
```
Then. open a new terminal:
```
 python3 src/arjun_bot/src/bridge.py
```


