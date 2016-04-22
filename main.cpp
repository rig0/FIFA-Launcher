#define WINVER 0x0500
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <shellapi.h>
#include <tlhelp32.h>

using namespace std;

bool IsRunning(LPCSTR pName);
void Start(LPCTSTR pName, LPCTSTR pArgs);
void Kill(LPCTSTR pName);

int main()
{
    //---To run silently remove "//" from the beginning of the following line---//
    //FreeConsole();

    //---Closing Origin and relaunching as child process is required in order for Steam overlay to work---//
    cout << "\nTerminating 'Origin.exe' if running... \n" << endl;

    if (IsRunning("Origin") == true)
        Kill("Origin.exe");

    cout << "Launching 'Origin.exe' as child process... \n" << endl;

    Start("C:/PROGRA~2/Origin/Origin.exe","-StartClientMinimized");

    cout << "Making sure Origin Launched... \n" << endl;

    for (int i = 50; IsRunning("Origin") == false; i--)
    {
        Sleep(500);
        if ( i == 0 )
        {
            cout << "Timed out." << endl;
            Sleep (3000);
            return 0;
        }
    }

    Sleep(7000); //<---Adjust depending on how long it takes your PC to launch origin.---//

    cout << "Launching 'fifaconfig.exe'... \n" << endl;

    Start("origin://launchgame/Origin.OFR.50.0000781",NULL);

    cout << "Waiting for 'fifaconfig.exe'... \n" << endl;

    for (int i = 50; IsRunning("FIFA 16") == false; i--)
    {
        Sleep(500);
        if ( i == 0 )
        {
            cout << "Timed out." << endl;
            Sleep (3000);
            return 0;
        }
    }

    cout << "Sending ENTER key to 'fifaconfig.exe'... \n" << endl;

    HWND config_hwnd;
    config_hwnd = FindWindow(NULL, "FIFA 16");
    SetForegroundWindow(config_hwnd);
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    ip.ki.wVk = 0x0D;
    ip.ki.dwFlags = 0;
    SendInput(1, &ip, sizeof(INPUT));
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));

    cout << "Waiting 10 seconds to close 'fifaconfig.exe'... \n" << endl;

    Sleep(10000);

    cout << "Closing 'fifaconfig.exe'... \n" << endl;

    DWORD config_pid;
    GetWindowThreadProcessId(config_hwnd, &config_pid);
    HANDLE config_hndl;
    config_hndl = OpenProcess(PROCESS_ALL_ACCESS, FALSE, config_pid);
    TerminateProcess(config_hndl, 0);

    cout << "Waiting for FIFA to close... \n" << endl;

    while (IsRunning("FIFA 16")){
        Sleep(500);
    }

    cout << "Waiting 5 seconds to close Origin... \n" << endl;

    Sleep(5000);

    Kill("Origin.exe");

    cout << "All Done" << endl;

    return 0;
}

/*--------------- F U N C T I O N S ---------------*/

bool IsRunning(LPCSTR pName){
    HWND hwnd;
    hwnd = FindWindow(NULL,pName);
    if (hwnd != 0)
        return true;
        else return false;
}

void Start(LPCTSTR pName, LPCTSTR pArgs){
    ShellExecute(NULL,"open",pName,pArgs,NULL,SW_SHOW);
}

void Kill(LPCTSTR pName){
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(snapshot, &entry) == TRUE){
        while (Process32Next(snapshot, &entry) == TRUE){
            if (stricmp(entry.szExeFile, pName ) == 0){
            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
            TerminateProcess(hProcess, 0 );}}
    }CloseHandle(snapshot);
}
