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

int activatePropertyInPropertyEditor(QMainWindow *mainwindow
									 , const QString &propertyName)
{
	PropertyEditorView *dock =
			mainwindow->findChild<PropertyEditorView *>("propertyEditor");
	QtTreePropertyBrowser *tb = dock->findChild<QtTreePropertyBrowser *>();
	QTreeWidget *treeWidget = tb->findChild<QTreeWidget *>();
	qDebug() << treeWidget;
	QList<QTreeWidgetItem *> items =
			treeWidget->findItems(propertyName, Qt::MatchExactly);
	qDebug() << items.at(0);
	qDebug() << treeWidget->findChildren<QWidget *>();
	Q_ASSERT(items.size() == 1);
	QWidget *iw = treeWidget->itemWidget(items.at(0), 0);
	qDebug() << iw;
	QTest::mouseClick(iw, Qt::LeftButton);
	// TODO

	return 0;
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

int clickObjectOnScene(QMainWindow *window, const QString &name) {
	qReal::EditorView *view = qrtestlib::getCurrentEditorView(window);
	Q_ASSERT(view != nullptr);

	QList<QGraphicsItem *> l = view->items();
	foreach (QGraphicsItem *i, l) {
		qReal::Label *label = dynamic_cast<qReal::Label *>(i);
		if (label != nullptr) {
			if (label->toPlainText() == name) {
				QPoint corner = label->parentItem()->scenePos().toPoint();
				QPoint relCenter = label->parentItem()->boundingRect().center().toPoint();
				QTest::mouseClick(view->viewport(), Qt::LeftButton, 0, corner + relCenter);

				QTest::qWait(2000);
			}
		}
	}
	return 0;
}

int renameObjectOnScene(QMainWindow *window, const QString &currentName
						, const QString &newName)
{
	qReal::EditorView *view = qrtestlib::getCurrentEditorView(window);
	Q_ASSERT(view != nullptr);

	QList<QGraphicsItem *> l = view->items();
	foreach (QGraphicsItem *i, l) {
		qReal::Label *label = dynamic_cast<qReal::Label *>(i);
		if (label != nullptr) {
			if (label->toPlainText() == currentName) {
				QPoint relLabel = label->scenePos().toPoint();
				QTest::mouseDClick(view->viewport(), Qt::LeftButton, 0, relLabel);
				QTest::keyClicks(nullptr, newName, Qt::NoModifier, 200);
				QTest::keyClick((QWidget *) nullptr, Qt::Key_Enter
								, Qt::NoModifier, 200);
			}
		}
	}
	return 0;
}

} // end namespace qrtestlib
