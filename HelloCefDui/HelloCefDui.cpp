//HelloCefDui.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "HelloCefDui.h"
#include <Windows.h>

#include "include/base/cef_scoped_ptr.h"
#include "include/cef_command_line.h"
#include "include/cef_sandbox_win.h"

#include "simple_app.h"
#include "MainDuiFrame.h"
#include "TestWindow.h"
#include "UILib.h" /*必须放在最后一行include*/

using namespace DuiLib;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	
	CPaintManagerUI::SetInstance(hInstance);
	CDuiString path = CPaintManagerUI::GetInstancePath() + _T("..\\..\\HelloCefDui\\res");
	CPaintManagerUI::SetResourcePath(path);
	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) { return 0; }

	CefEnableHighDPISupport();

	void* sandbox_info = NULL;
#if defined(CEF_USE_SANDBOX)
	// Manage the life span of the sandbox information object. This is necessary
	// for sandbox support on Windows. See cef_sandbox_win.h for complete details.
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif

	CefMainArgs main_args(hInstance);
	CefRefPtr<SimpleApp> app(new SimpleApp);
	int exit_code = CefExecuteProcess(main_args, app, sandbox_info);
	if (exit_code >= 0) {
		// The sub-process has completed so return here.
		return exit_code;
	}

	CefSettings settings;
	CefSettingsTraits::init(&settings);
	settings.single_process = true;
	settings.ignore_certificate_errors = true;
	settings.log_severity = LOGSEVERITY_ERROR;
#if !defined(CEF_USE_SANDBOX)
	settings.no_sandbox = true;
#endif
	settings.multi_threaded_message_loop = true;

	CefInitialize(main_args, settings, app.get(), sandbox_info);

	MainDuiFrame pFrame;
	pFrame.Create(NULL, _T("XiaMi"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame.CenterWindow();
	pFrame.ShowWindow();

	if (!settings.multi_threaded_message_loop)
	{
		CefRunMessageLoop();
	}
	else
	{
		CPaintManagerUI::MessageLoop();
	}
	
	delete pFrame;
	CefShutdown();
	::CoUninitialize();
	return 0;
} 
