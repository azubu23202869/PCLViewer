#include "pclviewer.h"

int main(int argc, char *argv[])
{
    // QTextCodec* code = QTextCodec::codecForName("GB2312");
    vtkOutputWindow::SetGlobalWarningDisplay(0);
    QApplication a(argc, argv);
    PCLViewer w;
    w.show();
    return a.exec();
}
