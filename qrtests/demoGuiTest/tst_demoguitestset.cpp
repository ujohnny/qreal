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
	void testSceneElement();
private:
	void testPropertyEditor();
	void testGesture();
	void testShowGrid();
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

void DemoGuiTestSet::testSceneElement() {
	MainWindow w("testsave.qrs");
	QTest::qWaitForWindowExposed(&w);
	QTest::qWait(1000);
	qrtestlib::clickObjectOnScene(&w, "Enum");
	qrtestlib::renameObjectOnScene(&w, "Enum", "ASFF");
	// some validation from models is needed here

	Id id = qrtestlib::getElementIdFromFriendlyId(&w, "ASFF");
	qDebug() << id;

	qrtestlib::validateElementPropertyInModel(&w, id, "name", "ASFF");
}

void DemoGuiTestSet::testGesture() {
	MainWindow w;
	QTest::qWaitForWindowExposed(&w);
	int res = qrtestlib::activateActionOnToolbar(&w, "fileToolbar", "actionNew_Diagram");
	QVERIFY(res == 0);

	QTest::qWait(1000);
	qReal::gui::TabWidget *tw = qrtestlib::getTabWidget(&w);
	QVERIFY(dynamic_cast<qReal::EditorView *>(tw->currentWidget()) != nullptr);

	qReal::EditorView *view = qrtestlib::getCurrentEditorView(&w);
	QVERIFY(view != nullptr);

	QTest::mousePress(view->viewport(), Qt::RightButton, 0, QPoint(100, 200));
	QTest::mouseMove(view->viewport(), QPoint(100, 100), 100);
	QTest::mouseMove(view->viewport(), QPoint(200, 100), 100);
	QTest::mouseMove(view->viewport(), QPoint(200, 200), 100);
	QTest::mouseMove(view->viewport(), QPoint(100, 200), 100);
	QTest::mouseRelease(view->viewport(), Qt::RightButton, 0, QPoint(100, 200), 100);
	QTest::qWait(5000);

}

void DemoGuiTestSet::testPropertyEditor() {
	MainWindow w("testsave.qrs");
	QTest::qWaitForWindowExposed(&w);
	QTest::qWait(1000);
	qrtestlib::clickObjectOnScene(&w, "Enum");
	qrtestlib::setPropertyInPropertyEditor(&w, "displayedName", "new name");
}

int main(int argc, char **argv) {
	QRealApplication app(argc, argv);
	app.setAttribute(Qt::AA_Use96Dpi, true);
	QTEST_DISABLE_KEYPAD_NAVIGATION
	DemoGuiTestSet tc;
	return QTest::qExec(&tc, argc, argv);
}

#include "tst_demoguitestset.moc"
