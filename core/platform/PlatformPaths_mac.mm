//
// Created by Benjamin Drong on 5/13/26.
//
#include "PlatformPaths.h"

#include <filesystem>
#include <Foundation/Foundation.h>

namespace fs = std::filesystem;

namespace PlatformPaths {

    fs::path settingsFile() {

        @autoreleasepool {

            NSArray* paths = NSSearchPathForDirectoriesInDomains(
                NSApplicationSupportDirectory,
                NSUserDomainMask,
                YES
            );

            NSString* appSupport =
                [paths firstObject];

            NSString* fullPath =
                [appSupport stringByAppendingPathComponent:@"FocusLab"];

            fs::path dir([fullPath UTF8String]);

            fs::create_directories(dir);

            return dir / "settings.json";
        }
    }

}