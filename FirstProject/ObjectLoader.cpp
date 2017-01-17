#include "ObjectLoader.h"

std::vector<coordinates*> * load_shapes()
{
	const std::string dir = "Shapes\\";

	std::vector<std::string> files;


#ifdef linux
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL) {
		std::cout << "Error (" << errno << ") opening " << dir << std::endl;
		return errno;
	}

	while ((dirp = readdir(dp)) != NULL) {
		files.pushback(string(dirp->d_name));
	}
	closedir(dp);
#elif _WIN32 || _WIN64
	std::string searchPath = dir + "*";
	std::cout << searchPath << std::endl;

	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(searchPath.c_str(), &fd);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				files.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	else {
		std::cout << "INVALID_HANDLE_VALUE: " << GetLastError() << std::endl;
		std::cout << "See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx" << std::endl;
	}
#endif

	std::vector<coordinates*> * loadedShapes = new std::vector<coordinates*>;

	std::ifstream fb; // FileBuffer
	coordinates* shapeBuf; //ShapeBuffer

	for (std::string file : files) {

		std::cout << "Loading: " << (dir + file) << std::endl;

		fb.open((dir + file), std::ios::in);

		if (fb.is_open()) {
			std::string ObjectName;
			std::getline(fb, ObjectName);
			std::cout << ObjectName << std::endl;

			shapeBuf = new coordinates;
			shapeBuf->container = new std::vector<GLfloat>;

			// TODO: refactor into parser
			// This is lovely but won't work for complex data. I need to build a parser if I want to have complex files
			for (std::string each;
				std::getline(fb, each, ' ');
				shapeBuf->container->push_back(GLfloat(std::stof(each)))
				);

			shapeBuf->data = shapeBuf->container->data();
			shapeBuf->size = shapeBuf->container->size();
			loadedShapes->push_back(shapeBuf);

			shapeBuf = nullptr;
		}
		else {
			std::cout << "Error: " << file << " failed to open" << std::endl;
		}

		fb.close();

	}

	return loadedShapes;
}
