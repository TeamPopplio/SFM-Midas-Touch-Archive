# Source Filmmaker: Midas Touch

### Usage
1. Download the DLL from the releases page:
    Click [here](https://github.com/TeamPopplio/SFM-Midas-Touch/releases) to visit the releases page.

2. Place the DLL into the correct folder:
    From the topmost `Source Filmmaker` folder, navigate to `game` and then `usermod`. Place the DLL into `usermod`.

3. Launch Source Filmmaker.
4. Press F11 and go to `Options > Keyboard > Advanced`, check `Enable developer console`, then press OK.
5. Press the tilde key (<kbd>~</kbd>) and enter `plugin_load midas` into the developer console.

   Add `plugin_load midas` to `cfg/autoexec.cfg` to load Midas Touch automatically.

   *Loading Midas Touch more than once will crash the game!*

### Building
You will need Visual Studio 2019 and [git](https://git-scm.com).

1. Open Visual Studio 2019. Click on Tools → Get Tools and Features... from the top bar of the window. 
This should open the Visual Studio Installer in another window. From the Workload tab, install "Desktop development with C++". From the Individual Components tab, install:
	- MSVC v141 - VS 2017 C++ x64/x86 build tools
	- C++ Windows XP Support for VS 2017 (v141) tools

1. Run the following in cmd:
    ```
    git clone --recurse-submodules https://github.com/TeamPopplio/SFM-Midas-Touch.git
    
    SFM-Midas-Touch\midas.sln
    ```

1. If Windows asks which program to open `midas.sln` with, choose Visual Studio.

   If Visual Studio asks to retarget projects, press OK.

   Once Visual Studio is open, right click `libMinHook`, click `Properties`, change `Platform Toolset` to the one corresponding to your Visual Studio version, and press OK.

1. Click `Build > Build Solution`.

   `midas.dll` will be in `SFM-Midas-Touch\<Build Configuration>`

### References
Midas Touch is a fork of [SourcePauseTool](https://github.com/YaLTeR/SourcePauseTool).