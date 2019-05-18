#include "dspicabootable.h"
#include <QApplication>
#include <QFileInfo>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Jontisoft");
    a.setOrganizationDomain("jontio.zapto.org");
    a.setApplicationName("dsPICaBootable");
    a.setApplicationVersion("1");

    //if we find a hex file in the apps dir then with the same base name then use this as the one to load
    QStringList args=QCoreApplication::arguments();
    QFileInfo fileinfo(args[0]);
    QString path=fileinfo.absolutePath();
    QString hexfilename=path+"/"+fileinfo.completeBaseName()+".hex";
    QFile file(hexfilename);
    if(file.exists())
    {
        QSettings settings;
        settings.setValue("lineEdit_filename",hexfilename);
    }

    dspicabootable w;
    w.show();

    return a.exec();
}
