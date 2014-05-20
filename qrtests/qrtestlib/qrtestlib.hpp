#pragma once

class QMainWindow;
class QString;
class QMenu;

namespace qReal {
class EditorView;
class Id;
class MainWindow;

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

int setPropertyInPropertyEditor(QMainWindow *mainwindow
								, QString const &propertyName
								, QString const &propertyValue);

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

qReal::Id getElementIdFromFriendlyId(QMainWindow *window, QString const &name);

void validateElementPropertyInModel(qReal::MainWindow *w, qReal::Id const &id
									, QString const &propertyName, QString const &expectedValue);

} // end namespace qrtestlib
