# Quadcopter flight controller software and construction
* Take a look at the project report [pdf](Programska_in_strojna_oprema_za_komunikacijo_in_vodenje_quadcoptera.pdf) and [videos](videos) of me testing the quadcopter.

## This picture shows the main conceptual angle-speed control loop on the vehicle.
 Throttle and Roll potentiometers are shown only for ease of understanding, they are not normally installed on the vehicle, but on the remote control unit. The flight controller regulates the speed of rotation of the vehicle and not the absolute angle. This makes it harder to steer the vehicle because the driver needs to adjust the speed of the rotation of the vehicle and with that the angle of the vehicle. For example, the joystick potentiometers are in their neutral positions and the vehicle is flying horizontally to the ground. The driver wants to steer to the left with moderate speed. This means that the vehicle needs to be tilted slightly to the left to move itself to the left. The driver turns the roll joystick to the left and thus begins to increase the speed of rotation. When the driver sees that the vehicle is almost at the desired angle, he pulls the joystick back to neutral position, meaning the desired speed of rotation is zero. The vehicle is now at a certain angle and the driver is sending it commands to rotate at speed zero. The vehicle tries to keep the speed of rotation at zero, thus stays at that certain angle and moves to the left.
![](images/ControlLoopDrawing.jpg)
![](images/FrameDrawing.jpg)
![](images/FramePhoto1.jpg)
![](images/FramePhoto2.jpg)
![](images/MotorPhoto.jpg)
![](images/McuEscReceiverGyroPhoto.jpg) 
![](images/Receiver.png) 
![](images/responseZoom .png)       
![](images/Schematic_dron1.png)       
![](images/PWMminmax.png)        
![](images/I2CcommunicationDescribed.png)        
![](images/TransmissionProtocol.png)        
![](images/ProgramBlockDiagram.png)        

