#pragma once

#include "../../qrkernel/ids.h"
#include "client.h"
#include <QHash>

namespace qrRepo {

namespace details {

class ClientManager
{
public:
	ClientManager();
	ClientManager(QList<QString> const &files);
	~ClientManager();

	void addClient(QString const &workingFile);
	void removeClient(QString const &clientId);

	Client* access() { return mClients[mWorkingFile]; } //prototype element
	QList<QString> workingFiles();

	//replace QString to qReal::Id in realization
	//Client* access(QString const &clientId) { return mClients[clientId]; };

	//for future if we will create definition of current project
	//void setMainClient();

private:
	//QHash<qReal::Id, Client*> mClients; it will be good to manage projects with qReal::Id
	QHash<QString, Client*> mClients;

	QString mWorkingFile; //prototype element
};
}
}
