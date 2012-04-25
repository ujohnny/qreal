#pragma once

#include <QDialog>
#include "mainWindow.h"

namespace Ui {
class SwitchDialog;
}

namespace qReal {

namespace models {
	class Models;
}

namespace gui {

class SwitchDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit SwitchDialog(models::Models *model, MainWindow *parent);
	~SwitchDialog();
	
private slots:
	void loadProject();
	void projectChanged(int id);

private:
	MainWindow *mParent;
	Ui::SwitchDialog *mUi;
	QList<QString> mIdList;
	int mSelectedProject;
	models::Models *mModels;
};
}
}
