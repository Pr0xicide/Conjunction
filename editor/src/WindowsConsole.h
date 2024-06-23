#ifndef WINDOWS_CONSOLE_H_
#define WINDOWS_CONSOLE_H_

//
// Adapted from William Willing (http://www.williamwilling.com/blog/?p=74).
//

//
// Create a text console in a Windows GUI application.
// Redirect cin, cout, cerr, stdin, stdout, and stderr to the console.
// 
// CAUTION: Uses singleton pattern - not thread safe!
//          Best practice: show/hide console in main thread only.
//

#if _WIN32

class WindowsConsole {
	std::ofstream m_out;
	std::ofstream m_err;
	std::ifstream m_in;

	std::streambuf* m_old_cout;
	std::streambuf* m_old_cerr;
	std::streambuf* m_old_cin;

	// only one console per application is allowed
	static WindowsConsole* s_instance;

	WindowsConsole();
	WindowsConsole(const WindowsConsole&);
	WindowsConsole& operator=(const WindowsConsole&);
	~WindowsConsole();

public:

	static WindowsConsole* getInstance();

	void hide();
	void show();

    void toggle();

    bool isVisible();
};

#endif // _WIN32

#endif // include quard
