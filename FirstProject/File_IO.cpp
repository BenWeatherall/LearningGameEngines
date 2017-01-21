#include "File_IO.h"

std::vector<std::string> DirectoryContents(std::string dir)
{
	/* TODO: Dir addresses in Windows differs to linux. will need to change how dirs are handled for each system
	Consider starting each relevant section with a replace where windows replaces '/' with '\\', and
	linux replaces '\\' with '/' */

	std::vector<std::string> FileList;
#ifdef __linux__
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL) {
		std::cout << "Error (" << errno << ") opening " << dir << std::endl;
		return FileList;
	}

	while ((dirp = readdir(dp)) != NULL) {
		FileList.push_back(std::string(dirp->d_name));
	}
	closedir(dp);
#elif _WIN32 || _WIN64
	std::string searchPath = dir + "*";

	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(searchPath.c_str(), &fd);

	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				FileList.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	else {
		std::cout << "INVALID_HANDLE_VALUE: " << GetLastError() << std::endl;
		std::cout << "See: https://msdn.microsoft.com/en-us/library/windows/desktop/ms681381(v=vs.85).aspx" << std::endl;
	}
#endif

	return FileList;
}