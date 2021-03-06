/*
	This file is part of cpp-ethereum.
	cpp-ethereum is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	cpp-ethereum is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file AppContext.cpp
 * @author Yann yann@ethdev.com
 * @date 2014
 * Provides access to the current QQmlApplicationEngine which is used to add QML file on the fly.
 * In the future this class can be extended to add more variable related to the context of the application.
 * For now AppContext provides reference to:
 * - QQmlApplicationEngine
 * - dev::WebThreeDirect (and dev::eth::Client)
 * - KeyEventManager
 */

#include <QMessageBox>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include "CodeModel.h"
#include "FileIo.h"
#include "ClientModel.h"
#include "CodeEditorExtensionManager.h"
#include "Exceptions.h"
#include "AppContext.h"
#include "QEther.h"
#include <libwebthree/WebThree.h>

using namespace dev;
using namespace dev::eth;
using namespace dev::mix;

const QString c_projectFileName = "project.mix";

AppContext::AppContext(QQmlApplicationEngine* _engine)
{
	m_applicationEngine = _engine;
	//m_webThree = std::unique_ptr<dev::WebThreeDirect>(new WebThreeDirect(std::string("Mix/v") + dev::Version + "/" DEV_QUOTED(ETH_BUILD_TYPE) "/" DEV_QUOTED(ETH_BUILD_PLATFORM), getDataDir() + "/Mix", false, {"eth", "shh"}));
	m_codeModel.reset(new CodeModel(this));
	m_clientModel.reset(new ClientModel(this));
	m_fileIo.reset(new FileIo());
/*
	m_applicationEngine->rootContext()->setContextProperty("appContext", this);
	qmlRegisterType<FileIo>("org.ethereum.qml", 1, 0, "FileIo");
	qmlRegisterSingletonType(QUrl("qrc:/qml/ProjectModel.qml"), "org.ethereum.qml.ProjectModel", 1, 0, "ProjectModel");
	qmlRegisterType<QEther>("org.ethereum.qml.QEther", 1, 0, "QEther");
	qmlRegisterType<QBigInt>("org.ethereum.qml.QBigInt", 1, 0, "QBigInt");
	m_applicationEngine->rootContext()->setContextProperty("codeModel", m_codeModel.get());
	m_applicationEngine->rootContext()->setContextProperty("fileIo", m_fileIo.get());
*/
}

AppContext::~AppContext()
{
}

void AppContext::load()
{
	m_applicationEngine->rootContext()->setContextProperty("appContext", this);
	qmlRegisterType<FileIo>("org.ethereum.qml", 1, 0, "FileIo");
	m_applicationEngine->rootContext()->setContextProperty("codeModel", m_codeModel.get());
	m_applicationEngine->rootContext()->setContextProperty("fileIo", m_fileIo.get());
	qmlRegisterType<QEther>("org.ethereum.qml.QEther", 1, 0, "QEther");
	qmlRegisterType<QBigInt>("org.ethereum.qml.QBigInt", 1, 0, "QBigInt");
	QQmlComponent projectModelComponent(m_applicationEngine, QUrl("qrc:/qml/ProjectModel.qml"));
	QObject* projectModel = projectModelComponent.create();
	if (projectModelComponent.isError())
	{
		QmlLoadException exception;
		for (auto const& e : projectModelComponent.errors())
			exception << QmlErrorInfo(e);
		BOOST_THROW_EXCEPTION(exception);
	}
	m_applicationEngine->rootContext()->setContextProperty("projectModel", projectModel);
	qmlRegisterType<CodeEditorExtensionManager>("CodeEditorExtensionManager", 1, 0, "CodeEditorExtensionManager");
	m_applicationEngine->load(QUrl("qrc:/qml/main.qml"));
	appLoaded();
}

QQmlApplicationEngine* AppContext::appEngine()
{
	return m_applicationEngine;
}

void AppContext::displayMessageDialog(QString _title, QString _message)
{
	// TODO : move to a UI dedicated layer.
	QObject* dialogWin = m_applicationEngine->rootObjects().at(0)->findChild<QObject*>("alertMessageDialog", Qt::FindChildrenRecursively);
	QObject* dialogWinComponent = m_applicationEngine->rootObjects().at(0)->findChild<QObject*>("alertMessageDialogContent", Qt::FindChildrenRecursively);
	dialogWinComponent->setProperty("source", QString("qrc:/qml/BasicMessage.qml"));
	dialogWin->setProperty("title", _title);
	dialogWin->setProperty("width", "250");
	dialogWin->setProperty("height", "100");
	dialogWin->findChild<QObject*>("messageContent", Qt::FindChildrenRecursively)->setProperty("text", _message);
	QMetaObject::invokeMethod(dialogWin, "open");
}
