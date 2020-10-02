#include "Connector.h" 

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>

USING_NS_CC;

/*#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

TCHAR* GetThisPath(TCHAR* dest, size_t destSize)
{
  if (!dest) return NULL;

  DWORD length = GetModuleFileName(NULL, dest, destSize);
#if (NTDDI_VERSION >= NTDDI_WIN8)
  PathRemoveFileSpec(dest);
#else
  if (MAX_PATH > destSize) return NULL;
  PathRemoveFileSpec(dest);
#endif
  return dest;
}*/

STARTUPINFO sti = { 0 };
LPSTARTUPINFOA pstia = { NULL };
SECURITY_ATTRIBUTES sats = { 0 };
PROCESS_INFORMATION pi = { 0 };
HANDLE pipin_w, pipin_r, pipout_w, pipout_r;
BYTE buffer[2048];
DWORD writ, excode, read, available;

Connector::Connector()
{

}

Connector::~Connector()
{
}

bool Connector::init()
{
  if (!Node::init())
  {
    return false;
  }

  return true;
}

Connector* Connector::createConnector()
{
  Connector* pConnector = new(std::nothrow) Connector();

  if (pConnector && pConnector->init())
  {
    pConnector->autorelease();
    return pConnector;
  }
  else
  {
    delete pConnector;
    pConnector = nullptr;
    return nullptr;
  }
  return pConnector;
}

void Connector::ConnectToEngine(char* path)
{
  /*TCHAR path[1024];
  GetThisPath(path, 1024);
  std::string ex(path);
  bool stop = true;
  ex += fileName;*/

  char text[1024];
  strcpy(text, path);
  wchar_t wtext[1024];
  mbstowcs(wtext, text, strlen(text) + 1);//Plus null
  LPWSTR ptrPath = wtext;

  pipin_w = pipin_r = pipout_w = pipout_r = NULL;
  sats.nLength = sizeof(sats);
  sats.bInheritHandle = TRUE;
  sats.lpSecurityDescriptor = NULL;

  CreatePipe(&pipout_r, &pipout_w, &sats, 0);
  CreatePipe(&pipin_r, &pipin_w, &sats, 0);

  sti.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
  sti.wShowWindow = SW_HIDE;
  sti.hStdInput = pipin_r;
  sti.hStdOutput = pipout_w;
  sti.hStdError = pipout_w;

  CreateProcess(NULL, ptrPath, NULL, NULL, TRUE, 0, NULL, NULL, &sti, &pi);
}

std::string Connector::getNextMove(std::string position)
{
  std::string str;
  position = "position startpos moves " + position + "\ngo\n";

  WriteFile(pipin_w, position.c_str(), position.length(), &writ, NULL);
  Sleep(500);

  PeekNamedPipe(pipout_r, buffer, sizeof(buffer), &read, &available, NULL);
  do
  {
    ZeroMemory(buffer, sizeof(buffer));
    if (!ReadFile(pipout_r, buffer, sizeof(buffer), &read, NULL) || !read) break;
    buffer[read] = 0;
    str += (char*)buffer;
  } while (read >= sizeof(buffer));

  int n = str.find("bestmove");
  if (n != -1) return str.substr(n + 9, 4);

  return "error";
}


void Connector::CloseConnection()
{
  WriteFile(pipin_w, "quit\n", 5, &writ, NULL);
  if (pipin_w != NULL) CloseHandle(pipin_w);
  if (pipin_r != NULL) CloseHandle(pipin_r);
  if (pipout_w != NULL) CloseHandle(pipout_w);
  if (pipout_r != NULL) CloseHandle(pipout_r);
  if (pi.hProcess != NULL) CloseHandle(pi.hProcess);
  if (pi.hThread != NULL) CloseHandle(pi.hThread);
}