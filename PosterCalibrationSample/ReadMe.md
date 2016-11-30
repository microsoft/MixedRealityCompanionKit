## PosterCalibrationSample

Often times it is helpful to be able to align a hologram to a real world object, or be able to align multiple HoloLens devices to a common set of world coordinates (so everyone sees the same holograms in the same place).  For instance, in your Unity scene, you can add a "poster" to which you want to anchor your scene (perhaps a gameboard or such) – adding holograms on/around it.  Then you can print out the poster, lay it on a table and run the calibration/alignment tool.  This will move the holographic version of the poster to align with the physical version of the poster - moving all of the linked holograms to the correct alignment.  If desired, you can choose to hide the virtual poster after calibration, leaving the holograms in place.  When alignment is complete, a world anchor will be placed for the root transform, eliminating the need to recalibrate unless the poster is moved.

A "poster" is really just an image - a 2D texture applied to a quad in the scene.  Some images will work better than others for alignment purposes. In general, you'll want to choose an image with lots of key points - usually sharp angles with high contrast – it will take some trial and error to find images that work the best.  Also, the image should be asymmetric in some way so that the recognizer can determine the correct rotation of the poster.

This sample uses an OpenCV plugin to find a specified poster texture in the PV cam view and aligns a transform to that poster.  Other objects can be anchored to a common transform to align them to a known real-world location.  The poster will usually be aligned to within a couple of centimeters, and you can then use a manual alignment mechanism if finer alignment is needed.


### Building:
- Step 1: Build the Plugin project (x86/Release) to create the plugin.
- Step 2: Copy the binaries from the "Release/Plugin" folder to the "Unity/Assets/Scripts/PosterAlign/Binaries"
folder.
- Step 3: Build and deploy to the HoloLens.

### How to use:
The Unity "PosterAlign" folder has most of the code necessary for calibration, with the rest of the
files serving as supporting functionality (camera/voice/control, etc).

The ZoneCalibrationManager handles starting/stopping the zone calibration (via voice commands 
specified in Calibration.cs), and saving anchors for the zone.  The anchor will automatically be 
loaded the next time the scene/zone is loaded.
While the poster calibration can get the object close to the real location (usually within a couple 
centimeters), it is useful to have an additional mechanism to fine tune the placement/scale, so the 
sample also includes some voice commands (i.e. "move X plus", "scale up").  If exact scale is important, 
it is also useful to have a physical reference object (and matching virtual object) at an exact known distance 
from the calibration zone root (a couple meters is usually fine) so that the scale can be adjusted more 
exactly.  Scale is also saved/loaded with the anchor.

There are 2 calibration zones in the MultiZone sample, allowing you to align groups of objects in the scene
to two physical poster locations in the room.  
For testing, print out the 2 posters (or just show them on-screen), say "align zone 1", then look at
the poster/image from multiple angles.  The position will be refined as more position samples are taken.
Say "lock alignement" when the position settles into a stable position.  There is also a debug view showing 
which poster it is currently looking for, as well as the view of what the camera is seeing (with markers
showing detail points as well as a green box outlining the poster if found).

