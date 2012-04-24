#include "clientManager.h"
//#include "client.h"
using namespace qrRepo;
using namespace qrRepo::details;

ClientManager::ClientManager()
{
}

ClientManager::ClientManager(QList<QString> const &files)
	: mWorkingFile(files[0]) //prototype
{
	foreach(QString const &file, files) {
		addClient(file);
	}
}

ClientManager::~ClientManager() {
	foreach (QString const &id, mClients.keys()) {
		delete mClients[id];
		mClients.remove(id);
	}
}

void ClientManager::addClient(const QString &workingFile) {
	Client *c = new Client(workingFile);
	mClients.insert(workingFile, c);
}

void ClientManager::removeClient(const QString &clientId) {
	delete mClients[clientId];
	mClients.remove(clientId);
}

QList<QString> ClientManager::workingFiles() {
	QList<QString> workingFiles;
	foreach (QString const &id, mClients.keys()) {
		workingFiles.push_back(id);
	}
	return workingFiles;
}
