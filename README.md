# Console3D
A 3D Software rendering engine for the Windows command prompt written in C++.

Console3D is a software 3D rendering engine that uses the Windows command prompt as an output display, being capable of rendering full 3D models in the Windows Console. It's made in C++ and uses no external libraries other than the ones Windows makes available for the command prompt functions.

#Features
Some of the features the engine has:

  -Geodesic sphere generation  
  -Plane generation  
  -Model loading (.obj)  
  -Vertex normals generation  
  -Free look camera (can not hide the mouse cursor in the console though)  
  -Directional Lights  
  -Point Lights  
  -Spot Lights  
  -Diffuse Shading  
  -Parallel Rasterization with OpenMP  
  -Back face culling  
  -Quaternion Rotation  
  -Shadowing using Shadow Maps  

#Resolution
The font size used is 4x6 Raster Fonts, but in reality the "pixels" are 4x3.
This is because in order to draw on the command prompt, Console3D takes advantage of a half block character, 220(▄), thus multiplying the vertical resolution by 2 and dividing the font height by 2.

#Colors
There are only 16 colors in the Windows command prompt, but it is possible to change the value of each color, therefore Console3D is capable of changing the color palette.
A grey scale palette is used for lighting but any 16 colors are possible.

#Videos
Old Video: https://www.youtube.com/watch?v=tfO59BS7vt0  
New Video: https://www.youtube.com/watch?v=tWKfFGAjppI  

#Dithering
**The current version is not capable of dithering.**

I've performed several experiments with the dithering characters 176(░), 177(▒) and 178(▓). In theory we can expand our color
space using the 177 character because it creates a perfect grid, so it is possible to blend colors in a single character.

However the results in my experiments were less than ideal, not only is the vertical resolution gain sacrificed, the end result is not that great, here is a comparison:

[![ScreenShot](https://i.imgur.com/BoQOuOv.gif)](https://i.imgur.com/BoQOuOv.gif)  
[![ScreenShot](https://i.imgur.com/57Qeyg9.gif)](https://i.imgur.com/57Qeyg9.gif)  

If you think there is a better way to achieve more color depth please contact me, the 16 color limit is the biggest limitation in the engine so far.
