## PosterCalibrationSample

This sample uses an OpenCV plugin to find a specified poster texture in the PV cam view and aligns
a transform to that poster.

### Building:
- Step 1: Build the Plugin project (x86/Release) to create the plugin.
- Step 2: Copy the binaries from the "Release/Plugin" folder to the "Unity/Assets\Assets\Scripts\PosterAlign\Binaries"
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
it is useful to have a physical reference object (and matching virtual object) at an exact known distance 
from the calibration zone root (a couple meters is usually fine) so that the scale can be adjusted more 
exactly.  Scale is also saved/loaded with the anchor.

There are 2 calibration zones in the MultiZone sample, allowing you to align groups of objects in the scene
to two physical poster locations in the room.  
For testing, print out the 2 posters (or just show them on-screen), say "align zone 1", then look at
the poster/image from multiple angles.  The position will be refined as more position samples are taken.
Say "lock alignement" when the position settles into a stable position.  There is also a debug view showing 
which poster it is currently looking for, as well as the view of what the camera is seeing (with markers
showing detail points as well as a green box outlining the poster if found).

