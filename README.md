# Console3D
C++ 3D Software rendering engine for the Windows command prompt.

Console3D is a software 3D rendering engine that uses the Windows command prompt as an output display, thus rendering full 3D models in the Windows Console. It's made in C++ and uses no external libraries other than the ones Windows makes available for the command prompt functions.

#Features
Some of the features the engine has:

  -Geodesic sphere generation  
  -Plane generation  
  -Model loading (.obj)  
  -Vertex normals generation  
  -Free look camera (can not hide the mouse cursor in the console though)  
  -Direction lighting  
  -Point lighting  

#Resolution
The resolution (font size used) is 4x6 Raster Fonts, but in reallity the "pixels" are 4x3.
This is because in order to draw on the command prompt, Console3D uses only 3 types of characters: a bottom half block, a top half block and a full block.
Because of this the vertical resolution is divided by 2.

#Colors
There are only 16 colors in the Windows command prompt, but it is possible to change the value of each color, therefore Console3D is capable of changing the color palette.
A grey scale palette is used for lighting but any 16 colors are possible.

Video: https://www.youtube.com/watch?v=tfO59BS7vt0
