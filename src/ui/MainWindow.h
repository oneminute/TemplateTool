#pragma once

#include <QFileInfo>
#include <QMainWindow>

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

namespace Ui
{
    class MainWindow;
}

class MainWindowPrivate;
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    bool initialize();
    bool initializeMenu();
    bool initializeToolBar();
    bool initializeStatusBar();
    bool initializeDockPanel();
    bool initializeCanvas();

    bool initializeTemplates();

    void addTemplateWidget(const QFileInfo& fileInfo);

    void outputInfo(const QString& info);

public slots:
    void loadImage();
    void importTemplate();
    void matchTemplate();
    void splitAndExport();

protected slots:
    void onTemplateListRowChanged(int currentRow);

private:
    QSharedPointer<MainWindowPrivate> d_ptr;
    QSharedPointer<Ui::MainWindow> m_ui;

    Q_DISABLE_COPY(MainWindow)
    Q_DECLARE_PRIVATE(MainWindow)
};
