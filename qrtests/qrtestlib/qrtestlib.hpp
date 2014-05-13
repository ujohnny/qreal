#pragma once

class QMainWindow;
class QString;
class QMenu;

namespace qReal {
class EditorView;

namespace gui {
	class DraggableElement;
	class TabWidget;
}
}

namespace qrtestlib {

QMenu * getMenu(QMainWindow *mainWindow, QString const &menuName);

int activateActionOnToolbar(QMainWindow *mainWindow
		, QString const &toolbarName
		, QString const &actionName);

int activatePropertyInPropertyEditor(QMainWindow *mainwindow
									 , QString const &propertyName);

int clickObjectOnScene(QMainWindow *window
					   , QString const &name);

int renameObjectOnScene(QMainWindow *window
						, QString const &currentName
						, QString const &newName);

int activateMenu(QMainWindow *mainWindow, QString const &menuName);

int activateActionInMenu(QMainWindow *mainWindow, QString const &menuName
						 , QString const &actionName);

qReal::gui::DraggableElement * getElementFromPalette(QMainWindow *mainWindow
													 , QString const &elementName);

qReal::gui::TabWidget * getTabWidget(QMainWindow *mainWindow);

qReal::EditorView * getCurrentEditorView(QMainWindow *mainWindow);

void findElementOnScene(QMainWindow *mainWindow);

} // end namespace qrtestlib
