#include "qrtestlib.hpp"

#include <QString>
#include <QtWidgets>
#include <QtTest>

#include <qrgui/mainwindow/palette/draggableElement.h>
#include <qrgui/mainwindow/tabWidget.h>
#include <qrgui/mainwindow/palette/paletteTree.h>

namespace qrtestlib {

QMenu * getMenu(QMainWindow *mainWindow, QString const &menuName) {
	// assuming that menubar is exclusive
	QMenuBar *menubar = mainWindow->findChild<QMenuBar *>();
	QMenu *menu = menubar->findChild<QMenu *>(menuName);
	return menu;
}

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

int activateMenu(QMainWindow *mainWindow, QMenu *menu) {
	qDebug() << menu->title().at(1).toLatin1();
	if (menu != nullptr) {
		QTest::keyClick(mainWindow, menu->title().at(1).toLatin1(), Qt::AltModifier);
		return 0;
	}
	return 1;
}

int activateActionInMenu(QMainWindow *mainWindow, QString const &menuName
						 , QString const &actionName)
{
	QMenu *menu = getMenu(mainWindow, menuName);
	activateMenu(mainWindow, menu);

	QTest::qWait(1000);
	QList<QAction *> actions = menu->actions();
	foreach (QAction *action, actions) {
		if (action->objectName() == actionName) {
			QTest::keyClick(menu, Qt::Key_Enter);
			return 0;
		}
		QTest::qWait(1000);
		QTest::keyClick(menu, Qt::Key_Down);
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
