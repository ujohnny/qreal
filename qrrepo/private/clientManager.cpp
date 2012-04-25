#include "clientManager.h"
//#include "client.h"
using namespace qrRepo;
using namespace qrRepo::details;

ClientManager::ClientManager()
{
}

ClientManager::ClientManager(QList<QString> const &files)
{
	foreach(QString const &file, files) {
		addClient(file);
	}
	mKey = mClients.keys()[0];
}

ClientManager::~ClientManager() {
	foreach (QString const &id, mClients.keys()) {
		removeClient(id);
	}
}

Client* ClientManager::access(const QString &clientId) {
	return mClients[clientId];
}

QString ClientManager::addClient(const QString &workingFile) {
	Client *c = new Client(workingFile);
	QString key = c->property(qReal::Id::rootId(), "project").toString();
	mClients.insert(key, c);
	return key;
}

void ClientManager::removeClient(const QString &clientId) {
	delete mClients[clientId];
	mClients.remove(clientId);
}

QList<QString> ClientManager::projectIds() {
	return mClients.keys();
}

void ClientManager::setCurrentClient(const QString &key) {
	mKey = key;
}

QHash<QString,QString> ClientManager::workingFiles() const {
	QHash<QString,QString> workingFiles;
	foreach (QString const &id, mClients.keys()) {
		workingFiles.insert(id, mClients.value(id)->workingFile());
	}
	return workingFiles;
}

QString ClientManager::filenameById(const QString &id) const {
	return mClients[id]->workingFile();
}
