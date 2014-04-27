#include "qrtestlib.hpp"

#include <QString>
#include <QtWidgets>
#include <QtTest>

#include <qrgui/mainwindow/palette/draggableElement.h>
#include <qrgui/mainwindow/tabWidget.h>
#include <qrgui/mainwindow/palette/paletteTree.h>

namespace qrtestlib {

int activateActionOnToolbar(QMainWindow *mainWindow
							 , QString const &toolbarName
							 , QString const &actionName)
{
	QToolBar *tb = mainWindow->findChild<QToolBar *>(toolbarName);
	QList<QAction *> actions = tb->actions();
	foreach (QAction *action, actions) {
		if (action->objectName() == actionName) {
			QWidget *actionWidget = tb->widgetForAction(action);
			QTest::mouseClick(actionWidget, Qt::LeftButton);
			return 0;
		}
	}
	return 1;
}

qReal::gui::DraggableElement * getElementFromPalette(QMainWindow *mainWindow
													, QString const &elementName)
{
	qReal::gui::PaletteTree *palette = mainWindow->findChild<qReal::gui::PaletteTree *>();
	QList<qReal::gui::DraggableElement *> elements =
			palette->findChildren<qReal::gui::DraggableElement *>();
	foreach (qReal::gui::DraggableElement * el, elements) {
		if (el->id().toString() == elementName) {
			return el;
		}
	}
	return nullptr;
}

qReal::gui::TabWidget * getTabWidget(QMainWindow *mainWindow) {
	return mainWindow->findChild<qReal::gui::TabWidget *>();
}

qReal::EditorView * getCurrentEditorView(QMainWindow *mainWindow) {
	qReal::gui::TabWidget *tab = getTabWidget(mainWindow);
	return dynamic_cast<qReal::EditorView *>(tab->currentWidget());
}

void findElementOnScene(QMainWindow *mainWindow) {
	qReal::EditorView *view = getCurrentEditorView(mainWindow);
	// TODO
}

} // end namespace qrtestlib
