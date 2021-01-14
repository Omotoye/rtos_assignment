# Real-time Operating Systems Assignment
#### Name: Omotoye Shamsudeen Adekoya
#### Student ID: 5066348
[Click here to see the Assignment Specifications](assignment_specifications.md) 
## How to Compile and Run 
---
Type __make__ in the Drivers directory, this will compile all the object files required for the loadable module, eg .ko 
```bash
make
```
To load in the module super user permission is required. To get there, simple type the command below and enter your password
```bash
sudo su
```
You should see something of this sort depending on where the file is place in your system
```bash
root@Omotoye-VirtualBox:/home/omotoye/Desktop/rtos_assignment/Driver# 
```
After getting to this stage, enter the command below to load the module code into the kernel
```bash
insmod ./character_device_driver.ko
```
You can find the module in the __/dev__ and the name of the device is **_my_device_**.
To access the driver with the multithreaded app, simply compile the app with the command below
```bash
gcc multithreaded_app.c -pthread 
```
*__You can only compile the app in the working directory of the multithreaded_app.c file__*

After the compilation you should get an __a.out__ file. Run the app with the command below 
```bash
./a.out
```
After the execution has ended, go back to the driver working directory with super user privilages and enter the command below to see the message log in the kernel log
```bash
dmesg
```
After use enter the command below to remove the driver module
```bash
rmmod ./character_device_driver.ko
```
