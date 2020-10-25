# Giant-Wheel
### Abstract 
    In our Computer Graphics project we have aimed at creating virtual
    Giant Wheel in Amusement Park. Giant Wheel is visualized using circular 
    rings. Rotation effects are provided for Giant Wheel. The whole giant wheel 
    is placed inside a texture mapped cube. There is also first person movement
    where the viewer can move around anywhere in the scene using up, down, 
    left and right keys for movement on ground and keys ‘w’ and ‘s’ for moving 
    up and down in 3D. The mouse context menu helps the user in selecting 
    various options,which can be obtained using right click. The colours of 
    various objects can be changed as well as background and ground can also 
    be changed. The user can control various camera positions like Free 
    Movement and inside both of the Giant Wheels. Objects like disc, cube, 
    sphere and cylinders are used to implement our giant wheel with an object
    and bench inside it.
    
## Functionality
* #####Free Movement -- We have enabled first person movement in our project where viewer can move anywhere using keyboard keys
    * Key Up-Moves ahead horizontally
    * Key Down-Move behind horizontally
    * Key Left-Rotates in left direction horizontally
    * Key Right-Rotates in right direction horizontally
    * "W" - Moves up vertically
    * "S" - Movesdownvertically.(Itwon’tmovedownaftera
            particular point as technically we can’t go inside the ground.
* #####Changing the Features of Giant Wheel
    By right clicking on screen,and choosing giant wheel option, we are able
    to change some features of giant wheel like -
    * ######To start or stop rotation of giant wheel: 
        If giant wheel is stationary that it will start moving and will 
        become stationary if was earlier moving.
    * ######Changing the number of trolleys: 
        We can choos enumber of wagons on giant wheel as 12 or 6. 
        Initially there are 12 wagons but can be made 6 by clicking this 
        option and again 12 by again clicking this option. Similarly, we can change the no. of wagons in 2nd wheel by 
        8 and 4.
    * ######Changing the direction of spin:
    We can choose direction of rotation of our wheel. If it is stationary 
    initially then it will start moving else it will move in opposite direction from which 
    it was moving.
    * ######Changing the color:
    We have 5 color choices for wagon and giant wheel stand to choose from 
    Change Color option.
    * ######Changing the Background and Ground:
    We have two background scenery choices which can be changed from
    change background option, and same goes for ground color
    as well.
    * ######Changing the speed:
    Increasing speed,as speed of wagon can be increased up to 3 folds. It will
    start moving if was stationary.Decreasing speed, as we can decrease the 
    speeds in three folds just like we increased in opposite manner. It will
    stop moving if decreased further.
* #####Mouse Movement
By clicking and moving our mouse the wheel will start if was earlier 
stationary. Else its speed will increase in the direction of movement. 
If we move mouse in opposite direction of the movement of wheel than 
the direction of rotation will change.
* #####Object Inside Giant Wheel
We have put an Object resembling like a human wearing red t-shirt
inside each wagon. The human like object is sitting on a bench.
* #####Displaying Speed
We are showing the speed of giant wheel on screen which will change as we change speed. 
It is showing positive speed in clockwise direction and negative in
anti-clockwise direction.
* #####Opening And Closing Of Gates
Initially as the giant wheel is in stop position its gates are open, but 
as soon as we start our giant wheel its gates are closed. So whenever the
 giant wheel is moving its gates are closed and opened when stopped. 
 Whichever way we use to start or stop our wheel i.e. start/stop wheel or 
 increase/decrease speed or through mouse movement or rotate wheel in opposite 
 direction the gates will be opened when stationary and closed otherwise.
* #####Position Inside Giant Wheel
We can choose to view from the Giant Wheels also. The menu available on the right click, 
offers this feature to sit in the Giant wheel, and view. We can also choose to
sit in different Giant wheels.

##How to run
```1. g++ final.cpp -lGL -lGLU -lglut -w```

You may encounter following error on MacOS
```
final.cpp:2:9: fatal error: 'GL/glut.h' file not found
#include<GL/glut.h>
        ^~~~~~~~~~~
1 error generated.
```
For that, you may need to download the library, from official sources.
