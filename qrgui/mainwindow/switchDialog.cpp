#include "switchDialog.h"
#include "ui_switchDialog.h"

#include "../models/models.h"

using namespace qReal;
using namespace gui;

SwitchDialog::SwitchDialog(models::Models *model, QWidget *parent)
	: QDialog(parent)
	, mUi(new Ui::SwitchDialog)
	, mIdList(model->repoControlApi().projectIds())
	, mModels(model)
{
	mUi->setupUi(this);
	mUi->idList->addItems(mIdList);
	connect(mUi->idList, SIGNAL(currentRowChanged(int)), this, SLOT(projectChanged(int)));
	connect(mUi->loadButton, SIGNAL(clicked()), this, SLOT(loadProject()));
	connect(mUi->closeButton, SIGNAL(clicked()), this, SLOT(close()));
}
void SwitchDialog::loadProject() {
	mModels->repoControlApi().setCurrentProject(mIdList[mSelectedProject]);
	mModels->reinit();

}

void SwitchDialog::projectChanged(int id) {
	mSelectedProject = id;
}

SwitchDialog::~SwitchDialog()
{
	delete mUi;
}
