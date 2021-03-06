import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.1
import CodeEditorExtensionManager 1.0

ApplicationWindow {
	id: mainApplication
	visible: true
	width: 1200
	height: 600
	minimumWidth: 400
	minimumHeight: 300
	title: qsTr("mix")

	menuBar: MenuBar {
		Menu {
			title: qsTr("File")
			MenuItem { action: createProjectAction }
			MenuItem { action: openProjectAction }
			MenuSeparator {}
			MenuItem { action: saveAllFilesAction }
			MenuSeparator {}
			MenuItem { action: addExistingFileAction }
			MenuItem { action: addNewJsFileAction }
			MenuItem { action: addNewHtmlFileAction }
			MenuSeparator {}
			//MenuItem { action: addNewContractAction }
			MenuItem { action: closeProjectAction }
			MenuSeparator {}
			MenuItem { action: exitAppAction }
		}
		Menu {
			title: qsTr("Debug")
			MenuItem { action: debugRunAction }
			MenuItem { action: debugResetStateAction }
		}
		Menu {
			title: qsTr("Windows")
			MenuItem { action: showHideRightPanel }
		}
	}

	Component.onCompleted: {
		setX(Screen.width / 2 - width / 2);
		setY(Screen.height / 2 - height / 2);
	}

	MainContent {
		id: mainContent;
		anchors.fill: parent
	}

	ModalDialog {
		objectName: "dialog"
		id: dialog
	}

	AlertMessageDialog {
		objectName: "alertMessageDialog"
		id: messageDialog
	}

	Action {
		id: exitAppAction
		text: qsTr("Exit")
		shortcut: "Ctrl+Q"
		onTriggered: Qt.quit();
	}

	Action {
		id: debugRunAction
		text: "&Run"
		shortcut: "F5"
		onTriggered: {
			mainContent.ensureRightView();
			clientModel.debugDeployment();
		}
		enabled: codeModel.hasContract && !clientModel.running;
	}

	Action {
		id: debugResetStateAction
		text: "Reset &State"
		shortcut: "F6"
		onTriggered: clientModel.resetState();
	}

	Action {
		id: showHideRightPanel
		text: "Show/Hide right view"
		shortcut: "F7"
		onTriggered: mainContent.toggleRightView();
	}

	Action {
		id: createProjectAction
		text: qsTr("&New project")
		shortcut: "Ctrl+N"
		enabled: true;
		onTriggered: projectModel.createProject();
	}

	Action {
		id: openProjectAction
		text: qsTr("&Open project")
		shortcut: "Ctrl+O"
		enabled: true;
		onTriggered: projectModel.browseProject();
	}

	Action {
		id: addNewJsFileAction
		text: qsTr("New JavaScript file")
		shortcut: "Ctrl+Alt+J"
		enabled: !projectModel.isEmpty
		onTriggered: projectModel.newJsFile();
	}

	Action {
		id: addNewHtmlFileAction
		text: qsTr("New HTML file")
		shortcut: "Ctrl+Alt+H"
		enabled: !projectModel.isEmpty
		onTriggered: projectModel.newHtmlFile();
	}

	Action {
		id: addNewContractAction
		text: qsTr("New contract")
		shortcut: "Ctrl+Alt+C"
		enabled: !projectModel.isEmpty
		onTriggered: projectModel.newContract();
	}

	Action {
		id: addExistingFileAction
		text: qsTr("Add existing file")
		shortcut: "Ctrl+Alt+A"
		enabled: !projectModel.isEmpty
		onTriggered: projectModel.addExistingFile();
	}

	Action {
		id: saveAllFilesAction
		text: qsTr("Save all")
		shortcut: "Ctrl+S"
		enabled: !projectModel.isEmpty
		onTriggered: projectModel.saveAll();
	}

	Action {
		id: closeProjectAction
		text: qsTr("Close project")
		shortcut: "Ctrl+W"
		enabled: !projectModel.isEmpty
		onTriggered: projectModel.closeProject();
	}
}
