================================================== ==============================
              MFC Library: CoreToolRS3 project overview
================================================== ==============================

Create the CoreToolRS3 application using the application wizard. This application
The program explains how to use MFC and how to write an application.

This file contains an overview of each of the files that make up the CoreToolRS3 application.
There is.

CoreToolRS3.vcproj
    This is the main project file for a VC++ project created using the application wizard.
    This file contains information about the Visual C++ version that generated the file and the platform selected in the application wizard,
    It includes information such as configuration, project features, and more.

CoreToolRS3.h
    This is the application's main header file. This file is related to other projects such as Resource.h
    It contains specific headers and the CCoreToolRS3App application class is declared.

CoreToolRS3.cpp
    This is the source file for the main application that contains the CCoreToolRS3App application class.

CoreToolRS3.rc
    A file listing all the Microsoft Windows resources used by the program.
    This file contains icons, bitmaps, and cursors stored in the RES subdirectory.
    You can also edit this file directly in Microsoft Visual C++. User's project
    Resources are contained in 1042.

res\CoreToolRS3.ico
    An icon file used as an icon for an application.
    This icon is included in the main resource file, CoreToolRS3.rc.

res\CoreToolRS3.rc2
    A file that contains resources that cannot be edited in Microsoft Visual C++.
    All resources that cannot be edited in the resource editor are contained in this file.
////////////////////////////////////////////// //////////////////////////

This is for the main frame window.
    The project includes standard MFC interfaces.
MainFrm.h, MainFrm.cpp
    The file containing the CMainFrame frame class.
    The frame class is derived from CFrameWnd and controls all SDI frame functions.
res\Toolbar.bmp
    A bitmap file used when creating tiled images on a toolbar.
    The initial toolbar and status bar are organized in the CMainFrame class.
    Edit the toolbar bitmap using the resource editor and open the CoreToolRS3.rc
    Update the IDR_MAINFRAME TOOLBAR array to add it to the toolbar button.
////////////////////////////////////////////// //////////////////////////

////////////////////////////////////////////// //////////////////////////

Other features

ActiveX control
    You can use ActiveX controls in your application.
////////////////////////////////////////////// //////////////////////////

Other standard files

StdAfx.h, StdAfx.cpp
    Precompiled header file (PCH) CoreToolRS3.pch and precompiled
    This is the file used when building the format file StdAfx.obj.

Resource.h
    Standard header file defining new resource IDs.
    Microsoft Visual C++ reads and updates this file.

////////////////////////////////////////////// //////////////////////////

Other information

AppWizard should be added or customized using "TODO:"
Represents the source code.

The application uses MFC for the shared DLL, and the application's language is the operating system's
If the language is different, in the Win\System directory of the Microsoft Visual C++ CD-ROM
MFC70XXX.DLL, a local resource, in the system or system32 directory of the computer.
You need to copy it and then rename it to MFCLOC.DLL. "XXX" stands for that language
It is an abbreviation. For example, MFC70DEU.DLL contains resources translated to German.
If you don't do this, some UI elements in your application will be
Will remain.

////////////////////////////////////////////// //////////////////////////