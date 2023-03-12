/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   ejectbutton_png;
    const int            ejectbutton_pngSize = 755;

    extern const char*   mute_png;
    const int            mute_pngSize = 897;

    extern const char*   pausebutton_png;
    const int            pausebutton_pngSize = 1343;

    extern const char*   playbutton_png;
    const int            playbutton_pngSize = 754;

    extern const char*   plus_png;
    const int            plus_pngSize = 475;

    extern const char*   remove_png;
    const int            remove_pngSize = 1615;

    extern const char*   unmute_png;
    const int            unmute_pngSize = 614;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 7;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
