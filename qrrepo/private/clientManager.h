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

	Client* access() { return mClients[mKey]; } //prototype element
	QList<QString> workingFiles();

	Client* access(QString const &clientId);

	//for future if we will create definition of current project
	//void setMainClient();

private:
	QHash<QString, Client*> mClients;

	QString mKey; //prototype element
};
}
}
