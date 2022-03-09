#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

std::string CreatePath(std::string, std::string);
bool RenameModule(const std::string&, const std::string&, const std::string&);
bool CheckIfFileExists(const std::string&);
void ReplaceFromFile(const std::string&, const std::string&);

const std::string initialModuleName = "EmptyModule";

int main(int argc, char* argv[])
{
	// Path where the .exe is located. We need that to rename all the folders and files
	const std::filesystem::path path = std::filesystem::path(argv[0]);
	const std::string stringPath = std::string(argv[0]);

	// Here we get the desired name of the module
	const std::string fileName = path.filename().u8string();
	const std::string folderPath = stringPath.substr(0, stringPath.find(fileName));
	const std::string moduleName = fileName.substr(0, fileName.find(".exe"));

	const std::string emptyModulePath = CreatePath(folderPath, initialModuleName);
	if (!std::filesystem::exists(emptyModulePath)) 
	{
		std::cout << "EmptyModule folder at path: " << emptyModulePath << " doesn't exist" << std::endl;
		system("pause");
	}
	else
	{
		if (!RenameModule(emptyModulePath, CreatePath(folderPath, moduleName), moduleName)) system("pause");
	}

	return 0;
}


// ------------------------------------------------------------------------------------------------------------
// Methods
// ------------------------------------------------------------------------------------------------------------


// Given a path where file should be located, returns the full path of the file
std::string CreatePath(std::string initialPath, std::string file)
{
	return initialPath.append(file);
}

std::string CreateFilePath(std::string initialPath, std::string file)
{
	return initialPath.append("/").append(file);
}

bool RenameModule(const std::string& emptyModulePath, const std::string& desiredModuleFolderPath, const std::string& moduleName)
{
	// Rename the root folder with the desired module name
	std::filesystem::rename(emptyModulePath, desiredModuleFolderPath);

	// Change the EmptyModule.Build.cs to [ModuleName].Build.cs
	std::string buildCsFile = CreatePath(desiredModuleFolderPath, "/" + initialModuleName + ".Build.cs");
	if (!CheckIfFileExists(buildCsFile)) return false;
	const std::string newBuildCsPath = CreateFilePath(desiredModuleFolderPath, CreatePath(moduleName, ".Build.cs"));
	std::filesystem::rename(buildCsFile, newBuildCsPath);
	// Replace all "EmptyModule" inside [ModuleName].Build.cs for [moduleName]
	ReplaceFromFile(newBuildCsPath, moduleName);

	// Chenge the /Public/EmptyModuleModule.h to /Public/[ModuleName]Module.h
	std::string moduleH = CreatePath(desiredModuleFolderPath, "/Public/" + initialModuleName + "Module.h");
	if (!CheckIfFileExists(moduleH)) return false;
	const std::string newModuleHPath = CreateFilePath(desiredModuleFolderPath, "Public/" + moduleName + "Module.h");
	std::filesystem::rename(moduleH, newModuleHPath);
	// Replace all "EmptyModule inside [ModuleName]Module.h for [moduleName]
	ReplaceFromFile(newModuleHPath, moduleName);

	// Chenge the /Private/EmptyModuleModule.cpp to /Private/[ModuleName]Module.cpp
	std::string moduleCpp = CreatePath(desiredModuleFolderPath, "/Private/" + initialModuleName + "Module.cpp");
	if (!CheckIfFileExists(moduleCpp)) return false;
	const std::string newModuleCppPath = CreateFilePath(desiredModuleFolderPath, "Private/" + moduleName + "Module.cpp");
	std::filesystem::rename(moduleCpp, newModuleCppPath);
	// Replace all "EmptyModule inside [ModuleName]Module.h for [moduleName]
	ReplaceFromFile(newModuleCppPath, moduleName);

	return true;
}

bool CheckIfFileExists(const std::string& filePath)
{
	if (!std::filesystem::exists(filePath))
	{
		std::cout << "File at path: " << filePath << " doesn't exist" << std::endl;
		return false;
	}

	return true;
}

void ReplaceFromFile(const std::string& filePath, const std::string& moduleName)
{
	std::string fileContent;

	std::fstream file(filePath, std::fstream::in);
	if (file.is_open())
	{
		int index;
		std::string line;
		while (std::getline(file, line))
		{
			while ((index = line.find(initialModuleName)) != std::string::npos)
			{
				line.replace(index, initialModuleName.length(), moduleName);
			}

			fileContent.append(line).append("\n");
		}
		file.close();
	}
	else std::cout << "Can't open the file at path: " << filePath << std::endl;

	std::fstream newFile(filePath, std::fstream::trunc | std::fstream::out);
	if (newFile.is_open())
	{
		newFile << fileContent;
		newFile.close();
	}
	else std::cout << "Can't open the file at path: " << filePath << std::endl;

}