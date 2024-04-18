add_subdirectory(AudioProcessing)
add_subdirectory(GUIUtilities)

target_sources("${BaseTargetName}"
    PRIVATE
        Parameters.cpp
        PluginEditor.cpp
        PluginProcessor.cpp
        )