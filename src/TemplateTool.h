// TeamplteTool.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include "common.h"

#include <iostream>

#include <QApplication>

class MainWindow;

class TemplateToolApp : public QApplication
{
    Q_OBJECT
public:
    explicit TemplateToolApp(int &argc, char **argv);

    bool initialize();
    void destroy();

    QString getFilePath(const QString& title, const QString& filter);

    QString getPath(const QString& title);

    static TemplateToolApp& instance();

    static void handleLogOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg);

public slots:

private:
    QSharedPointer<MainWindow> m_mainWindow;

    QString m_lastDir;
};
