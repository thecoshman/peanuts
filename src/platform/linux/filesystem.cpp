#include "../common/filesystem.hpp"

#include <cstdlib>
#include <memory>
#include <string>
#include <sstream>

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

#include <boost/filesystem.hpp>

namespace {
const std::string PROGRAM_FOLDER("/kyrostat");
const std::string USER_DATA_APPEND("/.local");
const std::string EXE_LINK("/proc/self/exe");

std::string GetEnvVariable(const std::string& varName) {
    const char* value = std::getenv(varName.c_str());
    if(value) {
        return std::string(value);
    } else {
        return std::string();
    }
}

std::string GetHomeFromSystem() {
    int bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if(bufsize == -1) {
        bufsize = 16384; //numbers from the getpwuid_r manpage
    }
        
    std::vector<char> buffer(bufsize);
    struct passwd pwd;
    struct passwd* result;

    uid_t uid = getuid();
    getpwuid_r(uid, &pwd, buffer.data(), bufsize, &result);
    if(result == nullptr) {
        return std::string();
    }

    return std::string(pwd.pw_dir);
}

bool DirectoryExists(const boost::filesystem::path& path) {
    using namespace boost::filesystem;
    file_status s = status(path);
    if(s.type() == status_error) {
        throw std::runtime_error("Error retrieving status about a path");
    }

    return !(s.type() == file_not_found);
}

bool CreateIfMissing(const boost::filesystem::path& path) {
    if(!DirectoryExists(path)) {
        return create_directory(path);
    }

    return true;
}

std::vector<boost::filesystem::path> TokenizePath(const std::string& path) {
    std::stringstream sspath(path);
    std::string item;
    std::vector<boost::filesystem::path> paths;

    while(std::getline(sspath, item, ':')) {
        if(item.size() > 0) {
            paths.push_back(boost::filesystem::path(item));
        }
    }

    return paths;
}

void AddRelativeDataDirs(std::vector<boost::filesystem::path>& paths) {
    boost::filesystem::path exePath = 
        boost::filesystem::read_symlink(EXE_LINK);

    if(exePath.empty()) return; //Not fatal, may still have data somewhere else

    paths.push_back(exePath / boost::filesystem::path("/../share"));

}

        
} //unnamed namespace
    
namespace Kyro {
namespace Platform {
boost::filesystem::path UserDataDirectory() {
        
    //First, check for $XDG_DATA_HOME
    std::string path;
    if((path = GetEnvVariable("XDG_DATA_HOME")) != "") {
        path = path + PROGRAM_FOLDER;
    } else if((path = GetEnvVariable("HOME")) != "") {
        path = path + USER_DATA_APPEND + PROGRAM_FOLDER;
    } else if((path = GetHomeFromSystem()) != "") {
        path = path + USER_DATA_APPEND + PROGRAM_FOLDER;
    } else {
        throw std::runtime_error("Could not find user data directory");
    }

    return path;
                
}
    
boost::filesystem::path ResourcesDirectory() {
    std::string paths_str = GetEnvVariable("XDG_DATA_DIRS");
    if(paths_str == "") {
        paths_str = "/usr/local/share/:/usr/share/";
    }

    auto paths = TokenizePath(paths_str);
    AddRelativeDataDirs(paths);
    for(auto path_str : paths) {
        boost::filesystem::path path = path_str / 
            boost::filesystem::path(PROGRAM_FOLDER);

        if(DirectoryExists(path)) { 
            return path;
        }

    }

    throw std::runtime_error("Could not find kyrostat data directory");
}
} //namespace Platform
} //namespace Kyro
