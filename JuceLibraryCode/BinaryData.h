/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   Hiss_wav;
    const int            Hiss_wavSize = 12740652;

    extern const char*   LowLevelGrainNoise_wav;
    const int            LowLevelGrainNoise_wavSize = 6991920;

    extern const char*   PinkNoise_wav;
    const int            PinkNoise_wavSize = 3813424;

    extern const char*   AgeKnob_png;
    const int            AgeKnob_pngSize = 760777;

    extern const char*   BlendKnob_png;
    const int            BlendKnob_pngSize = 743163;

    extern const char*   Bypass_png;
    const int            Bypass_pngSize = 10189;

    extern const char*   Environments_png;
    const int            Environments_pngSize = 41040;

    extern const char*   Face_png;
    const int            Face_pngSize = 325163;

    extern const char*   FaceWithReels_png;
    const int            FaceWithReels_pngSize = 801270;

    extern const char*   HissKnob_png;
    const int            HissKnob_pngSize = 788970;

    extern const char*   InputKnob_png;
    const int            InputKnob_pngSize = 1503539;

    extern const char*   Link_png;
    const int            Link_pngSize = 4529;

    extern const char*   OutputKnob_png;
    const int            OutputKnob_pngSize = 1456115;

    extern const char*   PrintThrough_png;
    const int            PrintThrough_pngSize = 8492;

    extern const char*   ShameCross_png;
    const int            ShameCross_pngSize = 2232079;

    extern const char*   ShameKnob_png;
    const int            ShameKnob_pngSize = 3203066;

    extern const char*   TapeType_png;
    const int            TapeType_pngSize = 7628;

    extern const char*   VUMeterL_png;
    const int            VUMeterL_pngSize = 1466485;

    extern const char*   VUMeterR_png;
    const int            VUMeterR_pngSize = 1483870;

    extern const char*   Wheels_png;
    const int            Wheels_pngSize = 11441092;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 20;

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
