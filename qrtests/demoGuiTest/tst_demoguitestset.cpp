#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <qrealApplication.h>
#include <mainwindow/mainWindow.h>
#include <thirdparty/windowsmodernstyle.h>
#include <mainwindow/palette/draggableElement.h>
#include <mainwindow/palette/paletteTree.h>
#include <mainwindow/tabWidget.h>

#include <qrtests/qrtestlib/qrtestlib.hpp>

class DemoGuiTestSet : public QObject
{
	Q_OBJECT

public:
	DemoGuiTestSet() {}

private Q_SLOTS:
	void initTestCase() {}
	void cleanupTestCase() {}
	void testShowGrid();
private:
	void testOpenSave();
	void testNewDiagram();
	void testNewProject();
};

void DemoGuiTestSet::testNewDiagram()
{
	MainWindow w;
	QTest::qWaitForWindowExposed(&w);
	int res = qrtestlib::activateActionOnToolbar(&w, "fileToolbar", "actionNew_Diagram");
	QVERIFY(res == 0);

	QTest::qWait(1000);
	qReal::gui::TabWidget *tw = qrtestlib::getTabWidget(&w);
	QVERIFY(dynamic_cast<qReal::EditorView *>(tw->currentWidget()) != nullptr);
}

void DemoGuiTestSet::testNewProject() {
	MainWindow w;
	QTest::qWaitForWindowExposed(&w);
	int res = qrtestlib::activateActionOnToolbar(&w, "fileToolbar", "actionNewProject");
	QVERIFY(res == 0);

	QTest::qWait(1000);
	qReal::EditorView *view = qrtestlib::getCurrentEditorView(&w);
	QVERIFY(view != nullptr);
}

void DemoGuiTestSet::testOpenSave() {
	MainWindow w("testsave.qrs");
	QTest::qWaitForWindowExposed(&w);
	QTest::qWait(1000);
	qReal::EditorView *view = qrtestlib::getCurrentEditorView(&w);
	QVERIFY(view != nullptr);
}

void DemoGuiTestSet::testShowGrid() {
	MainWindow w("testsave.qrs");
	QTest::qWaitForWindowExposed(&w);
	QTest::qWait(1000);
	qReal::EditorView *view = qrtestlib::getCurrentEditorView(&w);
	QVERIFY(view != nullptr);

	bool before = SettingsManager::value("ShowGrid").toBool();
	int res = qrtestlib::activateActionInMenu(&w, "menuSettings"
											  , "actionShow_grid");
	QVERIFY(res == 0);

	bool after = SettingsManager::value("ShowGrid").toBool();
	QVERIFY(before == !after);

	qReal::EditorView *editorView = qrtestlib::getCurrentEditorView(&w);
	QVERIFY(after == editorView->editorViewScene()->getNeedDrawGrid());
}

int main(int argc, char **argv) {
	QRealApplication app(argc, argv);
	app.setAttribute(Qt::AA_Use96Dpi, true);
	QTEST_DISABLE_KEYPAD_NAVIGATION
	DemoGuiTestSet tc;
	return QTest::qExec(&tc, argc, argv);
}

#include "tst_demoguitestset.moc"
