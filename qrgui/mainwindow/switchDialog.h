#pragma once

#include <QDialog>

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
	explicit SwitchDialog(models::Models *model, QWidget *parent = 0);
	~SwitchDialog();
	
private slots:
	void loadProject();
	void projectChanged(int id);

private:
	Ui::SwitchDialog *mUi;
	QList<QString> mIdList;
	int mSelectedProject;
	models::Models *mModels;
};
}
}
