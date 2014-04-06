#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <qrealApplication.h>
#include <mainwindow/mainWindow.h>
#include <thirdparty/windowsmodernstyle.h>
#include <mainwindow/palette/draggableElement.h>
#include <mainwindow/palette/paletteTree.h>
#include <mainwindow/tabWidget.h>
#include <thread>

class DemoGuiTestTest : public QObject
{
    Q_OBJECT

public:
    DemoGuiTestTest();

private Q_SLOTS:
    void testCase1();

private slots:
    void wait();
};

DemoGuiTestTest::DemoGuiTestTest()
{
}

void DemoGuiTestTest::wait() {
    QList<QWidget *> list = qApp->allWidgets();
    foreach (QWidget *w, list) {
        if (dynamic_cast<qReal::SuggestToCreateDiagramWidget *>(w) != nullptr) {
            //QTest::keyClick(w1, Qt::Key_Enter);
            QTest::qWaitForWindowExposed(w);
            qDebug() << "ping" ;

            qDebug() << w->children();
            QListWidget *widget =  w->findChild<QListWidget *>();
            QListWidgetItem *item = widget->currentItem();
            QModelIndex i = widget->currentIndex();
            qDebug() << i;
            QWidget *itemWidget = widget->itemWidget(item);
            qDebug() << itemWidget->objectName();

            QTest::mouseDClick(itemWidget, Qt::LeftButton);

            //QTest::mouseDClick(w1->findChild<QListWidget *>()->currentItem(), Qt::LeftButton);
        }
    }
}

void DemoGuiTestTest::testCase1()
{
    MainWindow w;

    QTest::qWaitForWindowExposed(&w);
    QToolBar *wi = w.findChild<QToolBar *>("fileToolbar");
    QList<QAction *> la = wi->actions();
    qDebug() << la;
    QAction *n;
    foreach (QAction *ac, la) {
        if (ac->objectName() == "actionNewProject") {
            n = ac;
            QWidget *w2 = wi->widgetForAction(n);
            qDebug() << w2;
            //QTimer::singleShot(3000, this, SLOT(wait()));

            QTest::mouseClick(w2, Qt::LeftButton);
            break;
        }
    }
    QTest::qWait(1000);
    qReal::gui::PaletteTree *pt = w.findChild<qReal::gui::PaletteTree *>();
    QList<qReal::gui::DraggableElement *> ld = pt->findChildren<qReal::gui::DraggableElement *>();

    qReal::gui::TabWidget *tw = w.findChild<qReal::gui::TabWidget *>();
    qReal::EditorView *ew = tw->currentWidget();

    QTest::qWait(1000);
    foreach (qReal::gui::DraggableElement *de, ld) {
        if (de->id().toString() == "qrm:/MetaEditor/MetaEditor/MetaEntityNode") {
            qDebug() << de;
            QTest::mouseMove(ew);
            QTest::mousePress(de, Qt::LeftButton);
//            QTest::mouseMove(tw->currentWidget(), QPoint(100, 100));
//            QTest::mouseRelease(tw->currentWidget(), Qt::LeftButton);
        }
    }

    QTest::qWait(5000);
}

int main(int argc, char **argv) {
    QRealApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    QTEST_DISABLE_KEYPAD_NAVIGATION
    DemoGuiTestTest tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "tst_demoguitesttest.moc"
