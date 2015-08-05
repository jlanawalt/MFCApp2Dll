This project documents the process of converting an MFC application to a DLL by simply exporting a wrapper around calling theApp's init & run. As source for how to do this I have drawn upon the ideas from two post answers. One in social.msdn.microsoft.com titled "Converting MFC application to DLL and running it from another process" by Jesse Jiang and the other in computer-programming-forum.com titled "Converting MFC MDI App to DLL (Experts, please)" by Mario L. Muell. Their solutions are reproduced below in case the forum posts get deleted or moved.

I have recreated each step as outlined by Jesse to the best of my understanding, then documented and mixed in Mario's solution to get a working simple dialog based MFC application converted into a DLL and called by another program. Each step is a commit so that others can reproduce the steps, see what changed, and what function (or errors) exist at each step.

The solution is just a proof of concept and has issues. For example, I wouldn't want a production program really using a console app with the console window showing.

The solution was created using Microsoft Visual Studio 2012 Professional.

Please feel free to add to this. It would be neat to roll in some tips and best practices for this simple type of conversion, or caution on troubles one may run into if they try to use this as a soltuion instead of moving the control class(es) into the DLL(s).

Jacob Anawalt



Converting MFC application to DLL and running it from another process
https://social.msdn.microsoft.com/Forums/vstudio/en-US/32b5d881-db7e-4d43-bb59-d84b038e2b70/converting-mfc-application-to-dll-and-running-it-from-another-process?forum=vcgeneral

Hello,
 
I think the better way is to put the control class into DLL and remove the message loop out of DLLs.
 
But if you want to put all the MFC application into a DLL. It's also fine. Please follow these steps
 
Create a MFC application, for example I create a simple dialog application called MFCindll.
Change the application type as DLL, Right Click your project->Properties->Configuration Properties-> General->Configuration Type as Dynamic Library (.dll)
Export all the classes and a outside call function like this
// in the MFCindll.h
class __declspec( dllexport ) CMFCindllApp;
extern "C" __declspec( dllexport ) CMFCindllApp* RunMainframe();
class __declspec( dllexport ) CMFCindllDlg;

//in the MFCindll.cpp
CMFCindllApp* RunMainframe()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState()) ;
	CMFCindllApp* myapp=new CMFCindllApp();
	myapp->InitInstance();
	myapp->Run();
	return myapp;
}

Create a application to call this DLL, I suggest you to create a console application with MFC support
You can load the DLL in static or dynamic way as you want, or just set project dependencies: Right Click your project->Properties->Common Properties->Framework and References->include your front project into this.
At last, call the outside call function like this.
CMFCindllApp* cmyapp=RunMainframe();

 
I hope my suggestions can help you to solve this problem.
 
Best regards,
Jesse
Jesse Jiang [MSFT]

...I forgot that we need delete the CWinApp theApp; in the console application, because there only one theApp in your project. It is a global value, there is the one in MFC DLL,  and we need to delete the one in console application...

Best regards,
Jesse
Jesse Jiang [MSFT]



Converting MFC MDI App to DLL (Experts, please)
http://computer-programming-forum.com/82-mfc/9d616bc86ce69a91.htm

I was able to figure the problem out. For those people wanting to 
convert an MFC application to DLL, I am posting this article as my 
contribution to the newsgroups. 
To convert an MFC MDI app to DLL, these steps worked for me: 
1. Create an empty DLL project with AppWizard 
2. Copy your original MFC application over to this project 
3. Remove Enable3dControls() from InitInstance() 
4. Mask SW_SHOW to the m_nCmdShow in the ShowWindow() member function 
of m_pFrameWnd 
5. Create a member variable called m_bReady in your customized CWinApp 
6. In constructor, initialize m_bReady to FALSE 
7. Add the following to the very beginning of your InitInstance() 
  if (!m_bReady) return TRUE; 
8. Create an exported function in your DLL to syncrhonize resources 
and show the main frame when you want it to: 
void StartFrame() 
{ 
  AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
  theApp.m_bReady = TRUE; 
  theApp.InitInstance(); 
  theApp.Run(); 
Quote:
} 

9. Finally, in your calling EXE create a thread using this function: 
  typedef void (*PROC_START) (void); 
  HMODULE hDLL = LoadLibrary("myappdll"); 
  PROC_START procStart = 
(PROC_START)GetProcAddress(hDLL,"StartFrame"); 
  procStart(); 
  FreeLibrary(hDLL); 
10. WARNING: some people say to make _AFXDLL to _USRDLL, but I keep 
using _AFXDLL because it already defines _USRDLL and I am sharing the 
MFC DLL which requires this definition. 
I hope I'm able to save someone else very much time with this. 
Mario Mueller 
