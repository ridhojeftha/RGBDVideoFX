RGBDVideoFX
===========




===========

Libraries:

openFrameworks

openFrameworks is distributed under the [MIT License](https://en.wikipedia.org/wiki/MIT_License). This gives everyone the freedoms to use openFrameworks in any context: commercial or non-commercial, public or private, open or closed source.

---

Copyright (c) 2004- openFrameworks Community

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

---

openFrameworks also ships with a mix of the following libraries, depending on your platform, which may have different licenses.

* [OpenGL](http://www.opengl.org/), [GLEW](http://glew.sourceforge.net/), [GLUT](http://www.opengl.org/resources/libraries/glut/), [libtess2](https://code.google.com/p/libtess2/) and [cairo](http://cairographics.org/) for graphics
* [rtAudio](http://www.music.mcgill.ca/~gary/rtaudio/), [PortAudio](http://www.portaudio.com/) or [FMOD](http://www.fmod.org/)  and [Kiss FFT](http://kissfft.sourceforge.net/) for audio input, output and analysis
* [FreeType](http://freetype.sourceforge.net/index2.html) for fonts
* [FreeImage](http://freeimage.sourceforge.net/) for image saving and loading
* [Quicktime](http://developer.apple.com/quicktime/), [Unicap](http://unicap-imaging.org/), [GStreamer](http://gstreamer.freedesktop.org/) and [videoInput](https://github.com/ofTheo/videoInput) for video playback and grabbing
* [Poco](http://pocoproject.org/) for a variety of utilities

openFrameworks README


1) installing Codeblocks:
--------------------------------------------------------------------------------

follow the steps here:
http://www.openframeworks.cc/setup/setup/codeblocks

** be sure to follow step e ** 

download this zip:

http://www.openframeworks.cc/content/files/codeblocks_additions.zip
and add the includes / libs to the corresponding includes / libs folder in mingw.


2) workspaces
--------------------------------------------------------------------------------

In 0.061 we've moved to a "compiled" openframeworks.  This means, you should open up the workspace
that comes with each project: 

imageSaverExample.cbp
imageSaverExample.workspace 	<---------- load this

the workspaces load both the imageSaverExample project as well as the openframeworksLib project, and 
will recompile the openframeworks library as needed.    

a few points:

a) it's easy to open up multiple projects into the workspace.  remember to close the workspace before working 
with a new project.  If you accidently open up other projects into the workspace, just don't save it. 

b) within each workspace, it's easy to highlight which project you want to build by clicking it.  
choose the "project", ie, imageSaverExample as opposed to the openframeworks lib.



3) if you want to make a new project:
--------------------------------------------------------------------------------

a) copy the folder inside of apps and paste in the same directory

ie (copy "emptyExample" and paste "copy of emptyExample")

b) rename the folder, and inside the folder, rename the .cbp and .workspace

c) open the .cbp file with a text editor.  Change the occurance of the old name to the new name:

line 5: <Option title="emptyExample" />

for example, if I copied emptyexample and wanted to call it "coolExample" would look like:

line 5: <Option title="coolExample" />

in the workspace file, change the name of .cbp file:

line 4: <Project filename="emptyExample.cbp" active="1" />

to whatever you've renamed the cbp to.



4) codeblocks wizard
--------------------------------------------------------------------------------

From 007 there's a codeblocks wizard to create projects, you can find instructions
on how to install it in scripts/win_cb/codeblocks_wizard.
	
	


as always if you have issues, check or post on the forum:  openframeworks.cc/forum

have fun! 




