#pragma once

#include <boost/filesystem.hpp>

namespace Peanuts {
    namespace Platform {
        boost::filesystem::path UserDataDirectory();
        boost::filesystem::path ResourcesDirectory();
    }
}
