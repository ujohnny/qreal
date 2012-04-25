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

	QString addClient(QString const &workingFile);
	void removeClient(QString const &clientId);

	Client* access() { return mClients[mKey]; } //prototype element
	QList<QString> workingFiles();

	Client* access(QString const &clientId);

	QList<QString> projectIds();
	void setCurrentClient(QString const &key);

private:
	QHash<QString, Client*> mClients;

	QString mKey; //prototype element
};
}
}
