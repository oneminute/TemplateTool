// TeamplteTool.cpp : Defines the entry point for the application.
//

#include "TemplateTool.h"

#include <QFileDialog>

#include "ui/MainWindow.h"

TemplateToolApp::TemplateToolApp(int& argc, char** argv)
    : QApplication(argc, argv)
    , m_mainWindow(new MainWindow)
{

}

bool TemplateToolApp::initialize()
{
    m_mainWindow->initialize();
    m_mainWindow->showNormal();
    qInstallMessageHandler(TemplateToolApp::handleLogOutput);
    return true;
}

void TemplateToolApp::destroy()
{
}

QString TemplateToolApp::getFilePath(const QString& title, const QString& filter)
{
    QString filename = QFileDialog::getOpenFileName(m_mainWindow.data(), title, m_lastDir, filter);
    if (filename.isEmpty() || filename.isNull())
        return QString();

    QFileInfo fileInfo(filename);
    m_lastDir = fileInfo.absoluteDir().absolutePath();
    return filename;
}

QString TemplateToolApp::getPath(const QString& title)
{
    QString dirName = QFileDialog::getExistingDirectory(m_mainWindow.data(), title, m_lastDir, QFileDialog::DontResolveSymlinks);
    if (dirName.isEmpty() || dirName.isNull())
        return QString();

    QDir dir(dirName);
    m_lastDir = dir.absolutePath();
    return dir.absolutePath();
}

TemplateToolApp& TemplateToolApp::instance()
{
    TemplateToolApp* app = static_cast<TemplateToolApp*>(QCoreApplication::instance());
    return *app;
}

void TemplateToolApp::handleLogOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    instance().m_mainWindow->outputInfo(msg);
}

