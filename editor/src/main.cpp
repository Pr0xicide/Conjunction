#include "stdafx.h"

#include "Game.h"

#if _WIN32
#include "WindowsConsole.h"
#endif

/**
 * This function will report any errors.
 */
void ReportError(const std::string& title, const std::string& msg)
{
    // print to stderr
    std::cerr << "*** " << title << ": " << msg << std::endl;

    // on Windows, also pop up a message box
#if _WIN32
    MessageBox(NULL, msg.c_str(), title.c_str(), MB_OK | MB_ICONERROR | MB_TASKMODAL);
#endif
}


#if _WIN32
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main()
#endif
{
    try {

#if _WIN32
        //WindowsConsole::getInstance()->show();
#endif

        // Create an instance of the game.
        capstone::Game game;

        // Start the game.
        game.go();

    } 
	
	// Exception handling.
	catch (const Ogre::Exception& oe) {
        ReportError("Ogre Exception", oe.getFullDescription());
    } 
	
	catch (const std::exception& e) {
        ReportError("Exception", e.what());
    } 
	
	catch (...) {
        ReportError("Unexpected Exception", "Something horrible has happened!");
    }

#if _WIN32
    // if the conole is visible, prevent it from closing automatically
    if (WindowsConsole::getInstance()->isVisible())
        system("pause");
#endif
}
