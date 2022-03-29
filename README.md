# Source Filmmaker: Midas Touch

### Usage
1. Download the `midas.zip` file from the releases page:
    Click [here](https://github.com/TeamPopplio/SFM-Midas-Touch/releases) to visit the releases page.

2. Extract the zip content into the correct folder:
    From the topmost `Source Filmmaker` folder, navigate to `game` and then `usermod`. Extract `addons` into `usermod`.

3. Launch Source Filmmaker.
4. Check the console for errors, if any.
5. If there are no errors, you're good to go!

### Building
You will need Visual Studio 2019, [git](https://git-scm.com), and [Qt 4.8.3 for Visual Studio 2010](https://download.qt.io/archive/qt/4.8/4.8.3/) to build the project.

1. Open Visual Studio 2019. Click on Tools → Get Tools and Features... from the top bar of the window. 
This should open the Visual Studio Installer in another window. From the Workload tab, install "Desktop development with C++". From the Individual Components tab, install:
	- MSVC v141 - VS 2017 C++ x64/x86 build tools
	- C++ Windows XP Support for VS 2017 (v141) tools

    While inside of Visual Studio, also install the [Qt Visual Studio Tools](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools-19123) extension.

1. From the top bar of the window, click on Extensions → Qt VS Tools → Qt Versions.

1. Add `C:\Qt\4.8.3` as a version path with the name `Qt_4.8.3`.

1. Run the following in a command prompt:
    ```
    git clone --recurse-submodules https://github.com/TeamPopplio/SFM-Midas-Touch.git
    
    SFM-Midas-Touch\midas.sln
    ```

1. If Windows asks which program to open `midas.sln` with, choose Visual Studio.

   If Visual Studio asks to retarget projects, press OK.

   Once Visual Studio is open, right click `libMinHook`, click `Properties`, change `Platform Toolset` to the one corresponding to your Visual Studio version, and press OK.

1. Ensure that the build target is set to `Release`.

1. Click `Build > Build Solution`.

   `midas.dll` will be in `SFM-Midas-Touch\Release`

   This is a binary that can be loaded within SFM using the `plugin_load` command.

### References
Midas Touch is based on [SourcePauseTool](https://github.com/YaLTeR/SourcePauseTool).