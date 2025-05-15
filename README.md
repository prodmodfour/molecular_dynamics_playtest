Dependencies:
    Qt6 configured for VTK
    Qt6 Charts
    VTK 9.3.1 or later

Build tips:
    After building with cmake, either use winqdeploy to add the necessary DLLs to the build folder or copy them directly from the include directory.
    
    In the build command, add --config Release if you don't want debug logging enabled (better performance). --config Debug otherwise.

    