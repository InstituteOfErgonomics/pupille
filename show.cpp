#include "stdafx.h"

#include "globals.h"

#include "Form1.h"

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
using namespace System::Threading;

using namespace show;


[STAThreadAttribute]
int main(array<System::String ^> ^args)
{

	// Aktivieren visueller Effekte von Windows XP, bevor Steuerelemente erstellt werden
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Hauptfenster erstellen und ausführen
    Form^  f = gcnew Form1(); 
	Application::Run(f);


	return 0;
}

