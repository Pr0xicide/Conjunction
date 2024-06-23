#include "stdafx.h"

#if _WIN32

#include "WindowsConsole.h"

WindowsConsole* WindowsConsole::s_instance = NULL;

WindowsConsole* WindowsConsole::getInstance()
{
	if (!WindowsConsole::s_instance)
		s_instance = new WindowsConsole();
	return s_instance;
}

WindowsConsole::WindowsConsole()
{
	//show();
}

WindowsConsole::~WindowsConsole()
{
	hide();
}

void WindowsConsole::show()
{
    if (isVisible())
        return;

	// create a console window
	AllocConsole();

	// redirect std::cout to our console window
	m_old_cout = std::cout.rdbuf();
	m_out.open("CONOUT$");
	std::cout.rdbuf(m_out.rdbuf());

	// redirect std::cerr to our console window
	m_old_cerr = std::cerr.rdbuf();
	m_err.open("CONOUT$");
	std::cerr.rdbuf(m_err.rdbuf());

	// redirect std::cin to our console window
	m_old_cin = std::cin.rdbuf();
	m_in.open("CONIN$");
	std::cin.rdbuf(m_in.rdbuf());

	// redirect C streams
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	freopen("CONIN$", "r", stdin);
}

void WindowsConsole::hide()
{
    if (!isVisible())
        return;

	// reset the standard streams
	std::cin.rdbuf(m_old_cin);
	std::cerr.rdbuf(m_old_cerr);
	std::cout.rdbuf(m_old_cout);

	// reset C streams??

	// remove the console window
	FreeConsole();
}

void WindowsConsole::toggle()
{
    if (isVisible())
        hide();
    else
        show();
}

bool WindowsConsole::isVisible()
{
    return GetConsoleWindow() != NULL;
}

#endif // _WIN32
