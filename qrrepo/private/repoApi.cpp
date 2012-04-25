#include "../repoApi.h"

#include <QDebug>

using namespace qrRepo;
using namespace qrRepo::details;
using namespace qReal;

RepoApi::RepoApi(QList<QString> const &workingFiles)
	: mClientManager(workingFiles)
{
}

QString RepoApi::name(Id const &id) const
{
	Q_ASSERT(mClientManager.access()->property(id, "name").canConvert<QString>());
	return mClientManager.access()->property(id, "name").toString();
}

void RepoApi::setName(Id const &id, QString const &name)
{
	mClientManager.access()->setProperty(id, "name", name);
}

IdList RepoApi::children(Id const &id) const
{
	return mClientManager.access()->children(id);
}

void RepoApi::addChild(Id const &id, Id const &child)
{
	mClientManager.access()->addChild(id, child);
}

void RepoApi::addChild(Id const &id, Id const &child, Id const &logicalId)
{
	mClientManager.access()->addChild(id, child, logicalId);
}

void RepoApi::stackBefore(Id const &id, Id const &child, Id const &sibling)
{
	mClientManager.access()->stackBefore(id, child, sibling);
}

Id RepoApi::copy(qReal::Id const &src)
{
	return mClientManager.access()->cloneObject(src);
}

void RepoApi::removeChild(Id const &id, Id const &child)
{
	mClientManager.access()->removeChild(id, child);
}

void RepoApi::removeChildren(Id const &id)
{
	foreach (Id const child, children(id))
		removeChild(id, child);
}

void RepoApi::removeElement(Id const &id)
{
	Q_ASSERT(id != Id::rootId());

	foreach (Id const child, children(id))
		removeElement(child);

	if (hasProperty(id, "links")) {
		IdList links = property(id, "links").value<IdList>();
		foreach (Id const link, links) {
			if (hasProperty(link, "from") && property(link, "from").value<Id>() == id)
				setProperty(link, "from", Id::rootId().toVariant());
			if (hasProperty(link, "to") && property(link, "to").value<Id>() == id)
				setProperty(link, "to", Id::rootId().toVariant());
		}
	}

	removeLinkEnds("from", id);
	removeLinkEnds("to", id);

	if (hasProperty(id, "outgoingConnections")) {
		IdList connections = property(id, "outgoingConnections").value<IdList>();
		foreach (Id const target, connections)
			disconnect(id, target);
	}

	if (hasProperty(id, "incomingConnections")) {
		IdList connections = property(id, "incomingConnections").value<IdList>();
		foreach (Id const source, connections)
			disconnect(source, id);
	}

	if (hasProperty(id, "outgoingUsages")) {
		IdList connections = property(id, "outgoingUsages").value<IdList>();
		foreach (Id const target, connections)
			deleteUsage(id, target);
	}

	if (hasProperty(id, "incomingUsages")) {
		IdList connections = property(id, "incomingUsages").value<IdList>();
		foreach (Id const source, connections)
			deleteUsage(source, id);
	}

	mClientManager.access()->remove(id);
}

void RepoApi::removeLinkEnds(QString const &endName, Id const &id) {
	if (hasProperty(id, endName)) {
		Id target = property(id, endName).value<Id>();
		if (hasProperty(target, "links")) {
			removeFromList(target, "links", id, endName);
		}
	}
}

Id RepoApi::parent(Id const &id) const
{
	return mClientManager.access()->parent(id);
}

void RepoApi::setParent(Id const &id, Id const &parent)
{
	Id const oldParent = mClientManager.access()->parent(id);
	mClientManager.access()->removeChild(oldParent, id);
	mClientManager.access()->setParent(id, parent);
}

IdList RepoApi::links(Id const &id, QString const &direction) const
{
	IdList links = mClientManager.access()->property(id, "links").value<IdList>();
	IdList result;
	foreach (Id const link, links) {
		if (mClientManager.access()->property(link, direction).value<Id>() == id) {
			result.append(link);
		}
	}
	return result;
}

IdList RepoApi::outgoingLinks(Id const &id) const
{
	return links(id, "from");
}

IdList RepoApi::incomingLinks(Id const &id) const
{
	return links(id, "to");
}

IdList RepoApi::links(Id const &id) const
{
	return incomingLinks(id) << outgoingLinks(id);
}

qReal::IdList RepoApi::outgoingConnections(qReal::Id const &id) const
{
	return mClientManager.access()->property(id, "outgoingConnections").value<IdList>();
}

qReal::IdList RepoApi::incomingConnections(qReal::Id const &id) const
{
	return mClientManager.access()->property(id, "incomingConnections").value<IdList>();
}

void RepoApi::connect(qReal::Id const &source, qReal::Id const &destination)
{
	addToIdList(source, "outgoingConnections", destination);
	addToIdList(destination, "incomingConnections", source);
}

void RepoApi::disconnect(qReal::Id const &source, qReal::Id const &destination)
{
	removeFromList(source, "outgoingConnections", destination);
	removeFromList(destination, "incomingConnections", source);
}

qReal::IdList RepoApi::outgoingUsages(qReal::Id const &id) const
{
	return mClientManager.access()->property(id, "outgoingUsages").value<IdList>();
}

qReal::IdList RepoApi::incomingUsages(qReal::Id const &id) const
{
	return mClientManager.access()->property(id, "incomingUsages").value<IdList>();
}

void RepoApi::addUsage(qReal::Id const &source, qReal::Id const &destination)
{
	addToIdList(source, "outgoingUsages", destination);
	addToIdList(destination, "incomingUsages", source);
}

void RepoApi::deleteUsage(qReal::Id const &source, qReal::Id const &destination)
{
	removeFromList(source, "outgoingUsages", destination);
	removeFromList(destination, "incomingUsages", source);
}

qReal::IdList RepoApi::connectedElements(qReal::Id const &id) const
{
	qReal::IdList result = outgoingConnectedElements(id);
	result.append(incomingConnectedElements(id));
	return result;
}

qReal::IdList RepoApi::outgoingConnectedElements(qReal::Id const &id) const
{
	qReal::IdList result;
	foreach (qReal::Id curLink, outgoingLinks(id)) {
		qReal::Id toElem = to(curLink);
		//if (toElem == Id::rootId())
		//	continue;

		result.append(toElem);
	}
	return result;
}

qReal::IdList RepoApi::incomingConnectedElements(qReal::Id const &id) const
{
	qReal::IdList result;
	foreach (qReal::Id curLink, incomingLinks(id)) {
		qReal::Id fromElem = from(curLink);
		//if (fromElem == Id::rootId())
		//	continue;

		result.append(fromElem);
	}
	return result;
}

QString RepoApi::typeName(Id const &id) const
{
	return id.element();
}

QVariant RepoApi::property(Id const &id, QString const &propertyName) const
{
	return mClientManager.access()->property(id, propertyName);
}

QString RepoApi::stringProperty(Id const &id, QString const &propertyName) const
{
	Q_ASSERT(mClientManager.access()->property(id, propertyName).canConvert<QString>());
	return mClientManager.access()->property(id, propertyName).toString();
}

void RepoApi::setProperty(Id const &id, QString const &propertyName, QVariant const &value)
{
	mClientManager.access()->setProperty(id, propertyName, value);
}

void RepoApi::removeProperty(Id const &id, QString const &propertyName)
{
	mClientManager.access()->removeProperty(id, propertyName);
}

void RepoApi::copyProperties(const Id &dest, const Id &src)
{
	mClientManager.access()->copyProperties(dest, src);
}

bool RepoApi::hasProperty(Id const &id, QString const &propertyName) const
{
	return mClientManager.access()->hasProperty(id, propertyName);
}

Id RepoApi::from(Id const &id) const
{
	Q_ASSERT(mClientManager.access()->property(id, "from").canConvert<Id>());
	return mClientManager.access()->property(id, "from").value<Id>();
}

void RepoApi::setFrom(Id const &id, Id const &from)
{
	if (hasProperty(id, "from")) {
		Id prev = mClientManager.access()->property(id, "from").value<Id>();
		removeFromList(prev, "links", id, "from");
	}
	mClientManager.access()->setProperty(id, "from", from.toVariant());
	addToIdList(from, "links", id, "from");
}

Id RepoApi::to(Id const &id) const
{
	Q_ASSERT(mClientManager.access()->property(id, "to").canConvert<Id>());
	return mClientManager.access()->property(id, "to").value<Id>();
}

void RepoApi::setTo(Id const &id, Id const &to)
{
	if (hasProperty(id, "to")) {
		Id prev = mClientManager.access()->property(id, "to").value<Id>();
		removeFromList(prev, "links", id, "to");
	}
	mClientManager.access()->setProperty(id, "to", to.toVariant());
	addToIdList(to, "links", id, "to");
}

double RepoApi::fromPort(Id const &id) const
{
	Q_ASSERT(mClientManager.access()->property(id, "fromPort").canConvert<double>());
	return mClientManager.access()->property(id, "fromPort").value<double>();
}

void RepoApi::setFromPort(Id const &id, double fromPort)
{
	mClientManager.access()->setProperty(id, "fromPort", fromPort);
}

double RepoApi::toPort(Id const &id) const
{
	Q_ASSERT(mClientManager.access()->property(id, "toPort").canConvert<double>());
	return mClientManager.access()->property(id, "toPort").value<double>();
}

void RepoApi::setToPort(Id const &id, double toPort)
{
	mClientManager.access()->setProperty(id, "toPort", toPort);
}

QVariant RepoApi::position(Id const &id) const
{
	return mClientManager.access()->property(id, "position");
}

QVariant RepoApi::configuration(Id const &id) const
{
	return mClientManager.access()->property(id, "configuration");
}

void RepoApi::setPosition(Id const &id, QVariant const &position)
{
	mClientManager.access()->setProperty(id, "position", position);
}

void RepoApi::setConfiguration(Id const &id, QVariant const &configuration)
{
	mClientManager.access()->setProperty(id, "configuration", configuration);
}

bool RepoApi::isLogicalElement(qReal::Id const &id) const
{
	return mClientManager.access()->isLogicalId(id);
}

bool RepoApi::isGraphicalElement(qReal::Id const &id) const
{
	return !mClientManager.access()->isLogicalId(id);
}

qReal::Id RepoApi::logicalId(qReal::Id const &id) const
{
	return mClientManager.access()->logicalId(id);
}

void RepoApi::exterminate()
{
	mClientManager.access()->exterminate();
}

void RepoApi::open(QString const &saveFile)
{
	mClientManager.setCurrentClient(mClientManager.addClient(saveFile));
}

void RepoApi::saveAll() const
{
	mClientManager.access()->saveAll();
}

void RepoApi::saveTo(QString const &workingFile)
{
	mClientManager.access()->setWorkingFile(workingFile);
	mClientManager.access()->saveAll();
}

void RepoApi::importFromDisk(QString const &importedFile)
{
	mClientManager.access()->importFromDisk(importedFile);
}

void RepoApi::save(qReal::IdList list) const
{
	mClientManager.access()->save(list);
}

QHash<QString,QString> RepoApi::workingFiles() const
{
	return mClientManager.workingFiles();
}

void RepoApi::addToIdList(Id const &target, QString const &listName, Id const &data, QString const &direction)
{
	if (target == Id::rootId())
		return;

	IdList list = mClientManager.access()->property(target, listName).value<IdList>();

	// Значения в списке должны быть уникальны.
	if (list.contains(data))
		return;

	list.append(data);
	mClientManager.access()->setProperty(target, listName, IdListHelper::toVariant(list));

	if (listName == "links") {
		IdList temporaryRemovedList = mClientManager.access()->temporaryRemovedLinksAt(target, direction);
		temporaryRemovedList.removeAll(data);
		mClientManager.access()->setTemporaryRemovedLinks(target, direction, temporaryRemovedList);
	}
}

void RepoApi::removeFromList(Id const &target, QString const &listName, Id const &data, QString const &direction)
{
	if (target == Id::rootId())
		return;

	IdList list = mClientManager.access()->property(target, listName).value<IdList>();
	IdList temporaryRemovedList = mClientManager.access()->temporaryRemovedLinksAt(target, direction);
	if(listName == "links" && list.contains(data)) {
		temporaryRemovedList.append(data);
	}
	list.removeAll(data);

	mClientManager.access()->setProperty(target, listName, IdListHelper::toVariant(list));
	mClientManager.access()->setTemporaryRemovedLinks(target, direction, temporaryRemovedList);
}

Id RepoApi::otherEntityFromLink(Id const &linkId, Id const &firstNode) const
{
	Id const fromId = from(linkId);
	if (fromId != firstNode)
		return fromId;
	else
		return to(linkId);
}

IdList RepoApi::logicalElements(Id const &type) const
{
	Q_ASSERT(type.idSize() == 3);

	IdList result;
	foreach (Id id, mClientManager.access()->elements()) {
		if (id.element() == type.element() && mClientManager.access()->isLogicalId(id))
			result.append(id);
	}
	return result;
}

IdList RepoApi::graphicalElements(Id const &type) const
{
	Q_ASSERT(type.idSize() == 3);

	IdList result;
	foreach (Id id, mClientManager.access()->elements()) {
		if (id.element() == type.element() && !mClientManager.access()->isLogicalId(id))
			result.append(id);
	}
	return result;
}

IdList RepoApi::elementsByType(QString const &type) const
{
	IdList result;
	foreach (Id id, mClientManager.access()->elements()) {
		if (id.element() == type)
			result.append(id);
	}
	return result;
}

int RepoApi::elementsCount() const
{
	return mClientManager.access()->elements().size();
}

bool RepoApi::exist(Id const &id) const
{
	return mClientManager.access()->exist(id);
}

IdList RepoApi::temporaryRemovedLinksAt(Id const &id, QString const &direction) const
{
	return mClientManager.access()->temporaryRemovedLinksAt(id, direction);
}

void RepoApi::setTemporaryRemovedLinks(Id const &id, IdList const &value, QString const &direction)
{
	mClientManager.access()->setTemporaryRemovedLinks(id, direction, value);
}

void RepoApi::removeTemporaryRemovedLinks(Id const &id)
{
	mClientManager.access()->removeTemporaryRemovedLinks(id);
}

QList<QString> RepoApi::projectIds() const {
	return mClientManager.projectIds();
}

void RepoApi::setCurrentProject(const QString &id) {
	mClientManager.setCurrentClient(id);
}

QString RepoApi::filenameById(const QString &id) const {
	return mClientManager.filenameById(id);
}
