#include <QtWidgets/QApplication>
#include <string>


#include "bencoding.h"
#include "umc_window.h"


using namespace std;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	UMCWindow mainWindow;

	mainWindow.show();
	mainWindow.activateWindow();

	return a.exec();
}
