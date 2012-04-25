#include "switchDialog.h"
#include "ui_switchDialog.h"

#include "../models/models.h"

using namespace qReal;
using namespace gui;

SwitchDialog::SwitchDialog(models::Models *model, MainWindow *parent)
	: QDialog(parent)
	, mUi(new Ui::SwitchDialog)
	, mIdList(model->repoControlApi().workingFiles().values())
	, mModels(model)
	, mParent(parent)
{
	mUi->setupUi(this);
	mUi->idList->addItems(mIdList);
	connect(mUi->idList, SIGNAL(currentRowChanged(int)), this, SLOT(projectChanged(int)));
	connect(mUi->loadButton, SIGNAL(clicked()), this, SLOT(loadProject()));
	connect(mUi->closeButton, SIGNAL(clicked()), this, SLOT(close()));
}
void SwitchDialog::loadProject() {
	//QString id = mModels->repoControlApi().filenameById()

	mModels->repoControlApi().setCurrentProject(
				mModels->repoControlApi().workingFiles().key(mIdList[mSelectedProject]));
	mParent->closeAllTabs();
	mModels->reinit();

	if (mModels->graphicalModel()->rowCount() > 0) {
		mParent->openNewTab(mModels->graphicalModel()->index(0, 0, QModelIndex()));
	}
	mParent->setWindowTitle(mIdList[mSelectedProject]);
	mParent->setSaveFile(mIdList[mSelectedProject]);
}

void SwitchDialog::projectChanged(int id) {
	mSelectedProject = id;
}

SwitchDialog::~SwitchDialog()
{
	delete mUi;
}
