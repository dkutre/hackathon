//
// Created by ovv on 13.05.17.
//

#include "mimeTypes.h"

struct mapping
{
    const char *extension;
    const char *mime_type;
} mappings[] = {{"html", "text/html"},
                {"css",  "text/css"},
                {"js",   "text/javascript"},
                {"jpg",  "image/jpeg"},
                {"jpeg", "image/jpeg"},
                {"png",  "image/png"},
                {"gif",  "image/gif"},
                {"swf",  "application/x-shockwave-flash"},
                {0,      0}};

std::string transformExtensionToType(const std::string &extension)
{
    for (mapping* m = mappings; m->extension; ++m) {
        if (m -> extension == extension) {
            return m -> mime_type;
        }
    }
    return "text/plain";
}
