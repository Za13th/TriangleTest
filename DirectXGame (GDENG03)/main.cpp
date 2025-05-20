#pragma comment(lib,"d3d11.lib") 
#include "AppWindow.h"


int main()
{
	AppWindow app;
	if (app.init())
	{
		while (app.isRun())
		{
			app.broadcast();
		}
	}

	return 0;
}