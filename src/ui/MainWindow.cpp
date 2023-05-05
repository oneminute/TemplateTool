#include "MainWindow.h"

#include <QBoxLayout>
#include <QDir>
#include <QFileInfo>
#include <QLabel>
#include <QListWidgetItem>
#include <QMenu>
#include <QMessageBox>
#include <QProgressDialog>
#include <QRectF>

#include "ui_MainWindow.h"
#include "common.h"
#include "MatchResultItem.h"
#include "TemplateScene.h"
#include "TemplateListWidget.h"
#include "util/util.h"

using namespace HalconCpp;

class MainWindowPrivate
{
    Q_DECLARE_PUBLIC(MainWindow)

public:
    MainWindowPrivate(MainWindow* ptr)
        : q_ptr(ptr)
    {}

    // all actions
    QAction* actionLoadImage;
    QAction* actionLoadTemplate;

    QAction* actionSplit;

    // ui
    TemplateScene* scene;

    // iamge
    HImage image;
    HImage templateImage;
    HTuple templateId;

    MainWindow* q_ptr;
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , d_ptr(new MainWindowPrivate(this))
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
}

MainWindow::~MainWindow()
{
}

bool MainWindow::initialize()
{
    if (!initializeMenu())
        return false;

    if (!initializeToolBar())
        return false;

    if (!initializeStatusBar())
        return false;

    if (!initializeStatusBar())
        return false;

    if (!initializeDockPanel())
        return false;

    if (!initializeCanvas())
        return false;

    if (!initializeTemplates())
        return false;

    return true;
}

bool MainWindow::initializeMenu()
{
    Q_D(MainWindow);

    QMenu* menuFile = new QMenu(tr("&File"));
    d->actionLoadImage = menuFile->addAction(tr("Load Image"), this, &MainWindow::loadImage);
    d->actionLoadTemplate = menuFile->addAction(tr("Import Temaplte"), this, &MainWindow::importTemplate);

    QMenu* menuOperation = new QMenu(tr("&Operations"));
    d->actionSplit = menuOperation->addAction(tr("Matching"), this, &MainWindow::matchTemplate);
    d->actionSplit = menuOperation->addAction(tr("Split And Export"), this, &MainWindow::splitAndExport);

    m_ui->menubar->addMenu(menuFile);
    m_ui->menubar->addMenu(menuOperation);

    return true;
}

bool MainWindow::initializeToolBar()
{
    QMenu* menuFile = new QMenu();
    return true;
}

bool MainWindow::initializeStatusBar()
{
    return true;
}

bool MainWindow::initializeDockPanel()
{
    return true;
}

bool MainWindow::initializeCanvas()
{
    Q_D(MainWindow);
    d->scene = new TemplateScene;
    m_ui->graphicsView->setScene(d->scene);
    return true;
}

bool MainWindow::initializeTemplates()
{
    QDir dir("templates");
    QStringList nameFilters;
    nameFilters << "*.bmp" << "*.png" << "*.jpg" << "*.jpeg" << "*.tif" << "*.tiff";
    QFileInfoList infos = dir.entryInfoList(nameFilters, QDir::Files, QDir::Name);
    for (const QFileInfo& fileInfo : infos)
    {
        addTemplateWidget(fileInfo);
    }
    return false;
}

void MainWindow::addTemplateWidget(const QFileInfo& fileInfo)
{
    QImage image(fileInfo.absoluteFilePath());

    TemplateListWidget* tplWidget = new TemplateListWidget;
    tplWidget->setImage(image);
    tplWidget->setTitle(fileInfo.fileName());
    tplWidget->setFileInfo(fileInfo);
    
    QListWidgetItem* listItem = new QListWidgetItem;
    listItem->setSizeHint(tplWidget->sizeHint());
    m_ui->listWidgetTemplates->addItem(listItem);
    m_ui->listWidgetTemplates->setItemWidget(listItem, tplWidget);

    //m_ui->listWidgetTemplates->setItemSelected(listItem, true);
}

void MainWindow::outputInfo(const QString& info)
{
    m_ui->plainTextEditInfos->insertPlainText(info);
    m_ui->plainTextEditInfos->insertPlainText("\n");
}

void MainWindow::loadImage()
{
    Q_D(MainWindow);
    QString filename = TemplateToolApp::instance().getFilePath(tr("Load Image"), tr("Images (*.jpg *.jpeg *.tif *.bmp *.png *.svg *.ico)"));
    if (filename.isEmpty())
        return;
    try
    {
        ReadImage(&d->image, filename.toStdString().c_str());
        QImage image;
        hImage2QImage(d->image, image);
        d->scene->setImage(image);
    }
    catch (HOperatorException& exp)
    {
        int code = exp.ErrorCode();
        QString msg = QString::fromLatin1(exp.ErrorMessage().TextA());
        qLogD << code << ": " << msg;
    }
}

void MainWindow::importTemplate()
{
    Q_D(MainWindow);
    QString filename = TemplateToolApp::instance().getFilePath(tr("Load Image"), tr("Images (*.jpg *.jpeg *.tif *.bmp *.png *.svg *.ico)"));
    if (filename.isEmpty())
        return;

    QFileInfo fileInfo(filename);
}

void MainWindow::matchTemplate()
{
    Q_D(MainWindow);
    if (m_ui->listWidgetTemplates->selectedItems().isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("No template selected!"));
        return;
    }

    if (!d->image.IsInitialized())
    {
        QMessageBox::warning(this, tr("Warning"), tr("No image to match!"));
        return;
    }

    QListWidgetItem* item = m_ui->listWidgetTemplates->selectedItems().first();
    TemplateListWidget* widget = qobject_cast<TemplateListWidget*>(m_ui->listWidgetTemplates->itemWidget(item));
    ReadImage(&d->templateImage, widget->fileInfo().absoluteFilePath().toStdString().c_str());
    
    //HImage r = d->image.CopyImage();
    //HImage g = d->image.CopyImage();
    //HImage b = d->image.CopyImage();

    CreateShapeModel(d->templateImage, 1, 0, 5, "auto", "auto", "use_polarity", "auto", "auto", &d->templateId);

    HTuple rows, columns;
    HTuple angles;
    HTuple scaleRs, scaleCs, scores;
    FindAnisoShapeModel(d->image, d->templateId,
        DegToRad(m_ui->doubleSpinBoxAngleStart->value()),
        DegToRad(m_ui->doubleSpinBoxAngleExtent->value()),
        1, 1,
        1, 1,
        m_ui->doubleSpinBoxMinScore->value(),
        0,
        m_ui->doubleSpinBoxMaxOverlap->value(),
        "least_squares",
        0, 0.9, &rows, &columns, &angles, &scaleRs, &scaleCs, &scores);
    int count = rows.Length();
    if (count == 0)
    {
        QMessageBox::warning(this, tr("Warning"), tr("No targets found."));
    }

    d->scene->clearResults();

    //QProgressDialog progress("进度", "取消", 0, count, this);
    //progress.setWindowModality(Qt::WindowModal);
    qLogD << "found " << rows.Length() << " matched items.";

    for (int i = 0; i < rows.Length(); i++)
    {
        //progress.setValue(i);
        //qLogD << columns[i].D() << ", " << rows[i].D();
        HTuple c1 = columns[i] - d->templateImage.Width() / 2;
        HTuple r1 = rows[i] - d->templateImage.Height() / 2;
        HTuple c2 = columns[i] + d->templateImage.Width() / 2;
        HTuple r2 = rows[i] + d->templateImage.Height() / 2;
        QRectF bounding(QPointF(c1, r1), QPointF(c2, r2));

        MatchResultItem* resultItem = new MatchResultItem;
        resultItem->setScore(scores[i]);
        resultItem->setAngleDegrees(angles[i]);
        resultItem->setBoundingRect(bounding);
        resultItem->setScoreThreshold(m_ui->doubleSpinBoxMinScoreThreshold->value());
        d->scene->addMatchResultItem(resultItem);

        
        //if (progress.wasCanceled())
            //break;
    }
}

void MainWindow::splitAndExport()
{
    Q_D(MainWindow);

    QString dirName = TemplateToolApp::instance().getPath(tr("Split"));
    if (dirName.isEmpty())
        return;

    QDir dir(dirName);

    int i = 0;
    for (MatchResultItem* item : d->scene->results())
    {
        QRectF qRect = item->boundingRect();
        HObject rect;
        GenRectangle1(&rect,
            qRect.topLeft().y(), qRect.topLeft().x(),
            qRect.bottomRight().y(), qRect.bottomRight().x());
        HImage littleImage;
        try
        {
            CropRectangle1(d->image, &littleImage,
                qRect.topLeft().y(), qRect.topLeft().x(),
                qRect.bottomRight().y(), qRect.bottomRight().x());
            QString filename = QString("%1.bmp").arg(i);
            filename = dir.absoluteFilePath(filename);
            WriteImage(littleImage, "bmp", 0, filename.toStdString().c_str());
            i++;
        }
        catch (HOperatorException& e)
        {
            QString errorMsg = QString::fromLatin1(e.ErrorMessage().Text());
            qLogD << "Error: " << e.ErrorCode() << ", message: " << errorMsg;
        }
    }
    ////progress.setValue(count);

    //HImage rgb;
    //Compose3(r, g, b, &rgb);
    //WriteImage(rgb, "bmp", 0, "result.bmp");
}

void MainWindow::onTemplateListRowChanged(int currentRow)
{
    Q_D(MainWindow);

}
