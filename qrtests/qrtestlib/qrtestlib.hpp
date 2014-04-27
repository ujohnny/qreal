#pragma once

class QMainWindow;
class QString;

namespace qReal {
class EditorView;

namespace gui {
	class DraggableElement;
	class TabWidget;
}
}

namespace qrtestlib {

int activateActionOnToolbar(QMainWindow *
		, QString const &
		, QString const &);

qReal::gui::DraggableElement * getElementFromPalette(QMainWindow *
													 , QString const &);

qReal::gui::TabWidget * getTabWidget(QMainWindow *);

qReal::EditorView * getCurrentEditorView(QMainWindow *);

void findElementOnScene(QMainWindow *);

} // end namespace qrtestlib
