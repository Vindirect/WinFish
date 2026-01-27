#include "WinFishApp.h"

using namespace Sexy;

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#else
int main() 
{
#endif
	WinFishApp* aTheApp = new WinFishApp();

	aTheApp->Init();
	aTheApp->Start();
	aTheApp->Shutdown();

	delete aTheApp;
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}