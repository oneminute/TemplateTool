#include "common.h"

#include "TemplateTool.h"

//#ifdef _MSC_VER
//#    ifdef NDEBUG
//#        pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
//#    else
//#        pragma comment(linker, "/SUBSYSTEM:CONSOLE")
//#    endif
//#endif
#        pragma comment(linker, "/SUBSYSTEM:CONSOLE")

int main(int argc, char* argv[])
{
    TemplateToolApp* app = new TemplateToolApp(argc, argv);

    qRegisterMetaType<QList<QPointF>>("QList<QPointF>");

    if (!app->initialize())
    {
        return -1;
    }

    int ret = app->exec();

    app->destroy();
    delete app;

    return ret;
}
