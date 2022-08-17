# raygame - custom physics

This is a sample C++ project setup with [raylib][raylib] for Visual Studio 2019.

This sample demonstrates how to create a real-time physics system with support
for collision detection and basic collision resolution.

> **Note:** This project does _not_ use physac. All physics-related code is
> entirely implemented in this project.

It is primarily intended for use by students in the Game Programming course at
the Seattle Campus of the Academy of Interactive Entertainment.

[raylib]:https://github.com/raysan5/raylib
[raysan]:https://github.com/raysan5

## Repository Structure

```
raygame/      Source code and assets for building the custom solver
thirdparty/   Headers and lib files for third-party libraries
  $LIB_NAME/  Each library gets its own folder
    lib/      If needed, lib files are in a sub-folder named by the $(ProgramShortName) for its architecture
      x64/    64-bit MSVC
      x86/    32-bit MSVC
```

## Building

This project supports by **Visual Studio 2019** or newer.

**Property**          | **Value**
----------------------|--------------------------------
Windows SDK Version   | 10.0 (latest installed version)
Platform Toolset      | Visual Studio 2019 (v142)
C++ Language Standard | ISO C++14 Standard (std:c++14)

When building, please make sure that your installation of Visual Studio has
the necessary components to compile this project, per the table above. If
you get any errors, consult the table above and refer to [Microsoft Docs][msft-modify]
to learn how to configure your installation appropriately.

[msft-modify]:https://docs.microsoft.com/en-us/visualstudio/install/modify-visual-studio?view=vs-2019

1. Clone the repository and open the solution in Visual Studio.
2. Build and run the "raygame" project.

## License

MIT License - Copyright (c) 2018-2020 Academy of Interactive Entertainment

For more information, see the [license][lic] file.

Third party works are attributed under [thirdparty.md][3p].

[lic]:license.md
[3p]:thirdparty.md
[raylib]:https://github.com/raysan5/raylib
