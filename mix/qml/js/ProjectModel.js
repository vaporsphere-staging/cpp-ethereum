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
/** @file ProjectModel.js
 * @author Arkadiy Paronyan arkadiy@ethdev.com
 * @date 2015
 * Ethereum IDE client.
 */

function saveAll() {
	saveProject();
}

function createProject() {
	newProjectDialog.open();
}

function browseProject() {
	openProjectFileDialog.open();
}

function closeProject() {
	if (!isEmpty) {
		if (haveUnsavedChanges)
			saveMessageDialog.open();
		else
			doCloseProject();
	}
}

function saveProject() {
	if (!isEmpty) {
		projectSaving(projectData);
		var json = JSON.stringify(projectData);
		var projectFile = projectPath + projectFileName;
		fileIo.writeFile(projectFile, json);
	}
}

function loadProject(path) {
	closeProject();
	console.log("loading project at " + path);
	var projectFile = path + projectFileName;
	var json = fileIo.readFile(projectFile);
	projectData = JSON.parse(json);
	if (!projectData.title) {
		var parts = path.split("/");
		projectData.title = parts[parts.length - 2];
	}
	projectPath = path;
	if (!projectData.files)
		projectData.files = [];

	for(var i = 0; i < projectData.files.length; i++) {
		addFile(projectData.files[i]);
	}
	projectSettings.lastProjectPath = path;
	projectLoaded();
}

function addExistingFile() {
	addExistingFileDialog.open();
}

function addProjectFiles(files) {
	for(var i = 0; i < files.length; i++)
		addFile(files[i]);
}

function addFile(fileName) {
	var p = projectPath + fileName;
	var extension = fileName.substring(fileName.length - 4, fileName.length);
	var isContract = extension === ".sol";
	var fileData = {
		contract: false,
		path: p,
		name: isContract ? "Contract" : fileName,
		documentId: fileName,
		isText: isContract || extension === ".html" || extension === ".js",
		isContract: isContract,
	};

	projectListModel.append(fileData);
}

function findDocument(documentId)
{
	for (var i = 0; i < projectListModel.count; i++)
		if (projectListModel.get(i).documentId === documentId)
			return i;
	console.error("Cant find document " + documentId);
	return -1;
}

function openDocument(documentId) {
	documentOpened(projectListModel.get(findDocument(documentId)));
}

function doCloseProject() {
	console.log("closing project");
	projectListModel.clear();
	projectPath = "";
	projectData = null;
	projectClosed();
}

function doCreateProject(title, path) {
	closeProject();
	console.log("creating project " + title + " at " + path);
	if (path[path.length - 1] !== "/")
		path += "/";
	var dirPath = path + title + "/";
	fileIo.makeDir(dirPath);
	var projectFile = dirPath + projectFileName;

	var indexFile = "index.html";
	var contractsFile = "contracts.sol";
	var projectData = {
		title: title,
		files: [ contractsFile, indexFile ]
	};
	//TODO: copy from template
	fileIo.writeFile(dirPath + indexFile, "<html></html>");
	fileIo.writeFile(dirPath + contractsFile, "contract MyContract {\n}\n");
	var json = JSON.stringify(projectData);
	fileIo.writeFile(projectFile, json);
	loadProject(dirPath);
}

function doAddExistingFiles(files) {
	for(var i = 0; i < files.length; i++) {
		var sourcePath = files[i];
		console.log(sourcePath);
		var sourceFileName = sourcePath.substring(sourcePath.lastIndexOf("/") + 1, sourcePath.length);
		console.log(sourceFileName);
		var destPath = projectPath + sourceFileName;
		console.log(destPath);
		if (sourcePath !== destPath)
			fileIo.copyFile(sourcePath, destPath);
		addFile(sourceFileName);
	}
}

function renameDocument(documentId, newName) {
	var i = findDocument(documentId);
	var document = projectListModel.get(i);
	if (!document.isContract) {
		var sourcePath = document.path;
		var destPath = projectPath + newName;
		fileIo.moveFile(sourcePath, destPath);
		document.path = destPath;
		document.name = newName;
		projectListModel.set(i, document);
		documentUpdated(documentId);
	}
}

function removeDocument(documentId) {
	var i = findDocument(documentId);
	var document = projectListModel.get(i);
	if (!document.isContract) {
		projectListModel.remove(i);
		documentUpdated(documentId);
	}
}

function newHtmlFile() {
	createAndAddFile("page", "html", "<html>\n</html>");
}

function newJsFile() {
	createAndAddFile("script", "js", "function foo() {\n}\n");
}

function createAndAddFile(name, extension, content) {
	var fileName = generateFileName(name, extension);
	var filePath = projectPath + fileName;
	fileIo.writeFile(filePath, content);
	addFile(fileName);
}

function generateFileName(name, extension) {
	var i = 1;
	do {
		var fileName = name + i + "." + extension;
		var filePath = projectPath + fileName;
		i++;
	} while (fileIo.fileExists(filePath));
	return fileName
}

