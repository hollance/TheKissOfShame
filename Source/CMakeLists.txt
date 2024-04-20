add_subdirectory(AudioProcessing)
add_subdirectory(GUIUtilities)

target_sources("${BaseTargetName}"
    PRIVATE
        Parameters.h
        Parameters.cpp
        PluginEditor.h
        PluginEditor.cpp
        PluginProcessor.h
        PluginProcessor.cpp
        shameConfig.h
        )