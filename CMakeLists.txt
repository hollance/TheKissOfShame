cmake_minimum_required(VERSION 3.15)
project(TheKissOfShame VERSION "${ProjectVersion}")

# project description stuff
set(BaseTargetName TheKissOfShame)
set(PluginName "The Kiss Of Shame")
set(ProjectVersion 1.0)


include (FetchContent)
Set(FETCHCONTENT_QUIET FALSE) # shows detailed info when fetching from github


set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# apple universal builds
if(APPLE)
set (CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum OS X deployment version" FORCE)
set (CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE INTERNAL "")
endif()

FetchContent_Declare(
	JUCE
	GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
	GIT_TAG 7.0.11
	GIT_PROGRESS TRUE
    GIT_SHALLOW 1
)

FetchContent_MakeAvailable(JUCE)


if(APPLE)
    set(buildFolder "Builds/MacOSX")
else()
    if(UNIX)
        set(buildFolder "Builds/Linux")
    else()
        set(buildFolder "Builds/Windows")
    endif()
endif()

 
juce_add_plugin("${BaseTargetName}"
    VERSION "${ProjectVersion}"                             # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    COMPANY_NAME Infernal Love                          # Specify the name of the plugin's author
    COMPANY_WEBSITE "https://github.com/hollance/TheKissOfShame"
    COMPANY_EMAIL ""
    IS_SYNTH FALSE                       # Is this a synth or an effect?
    NEEDS_MIDI_INPUT FALSE               # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE              # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                 # Is this plugin a MIDI effect?
    # COPY_PLUGIN_AFTER_BUILD TRUE        # Should the plugin be installed to a default location after building?
    # VST3_COPY_DIR "${buildFolder}"
	# VST_COPY_DIR "${buildFolder}"
	# AU_COPY_DIR "${buildFolder}"
    PLUGIN_MANUFACTURER_CODE ILov               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Tkos                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS VST3 Standalone AU                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "${PluginName}"  # The name of the final executable, which can differ from the target name
)

# for AAX
# juce_set_aax_sdk_path("/aax_sdk")

juce_generate_juce_header("${BaseTargetName}")

# directories to include in the compilation
add_subdirectory(Source)

target_compile_features("${BaseTargetName}" PUBLIC cxx_std_20)

target_compile_definitions("${BaseTargetName}" PUBLIC
	JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0
	JUCE_USE_FLAC=0
	JUCE_USE_OGGVORBIS=0
	JUCE_USE_MP3AUDIOFORMAT=0
	JUCE_USE_LAME_AUDIO_FORMAT=0
	JUCE_USE_WINDOWS_MEDIA_FORMAT=0
	JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_ENABLE_REPAINT_DEBUGGING=0
    JUCE_COREGRAPHICS_RENDER_WITH_MULTIPLE_PAINT_CALLS=1
    JUCE_COREGRAPHICS_DRAW_ASYNC=1
	JUCE_USE_CURL=0
	JUCE_LOAD_CURL_SYMBOLS_LAZILY=1
	JUCE_STRICT_REFCOUNTEDPOINTER=1
    JUCE_BUILD_EXTRAS=0
    JUCE_BUILD_EXAMPLES=0
	JUCE_WEB_BROWSER=0
	JUCE_USE_WIN_WEBVIEW2=0
	JUCE_USE_CAMERA=0)


juce_add_binary_data("${BaseTargetName}BinaryData" HEADER_NAME BinaryData.h NAMESPACE BinaryData SOURCES
    Audio_Resources/Hiss.wav
    Audio_Resources/LowLevelGrainNoise.wav
    Audio_Resources/PinkNoise.wav

    GUI_Resources/AgeKnob.png
    GUI_Resources/BlendKnob.png
    GUI_Resources/Bypass.png
    GUI_Resources/Environments.png
    GUI_Resources/Face.png
    GUI_Resources/FaceWithReels.png
    GUI_Resources/HissKnob.png
    GUI_Resources/InputKnob.png
    GUI_Resources/Link.png
    GUI_Resources/OutputKnob.png
    GUI_Resources/PrintThrough.png
    GUI_Resources/ShameCross.png
    GUI_Resources/ShameKnob.png
    GUI_Resources/TapeType.png
    GUI_Resources/VUMeterL.png
    GUI_Resources/VUMeterR.png
    GUI_Resources/Wheels.png
)

set_target_properties("${BaseTargetName}BinaryData" PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

# if you ever decide you'd like to support CLAP

# FetchContent_Declare (clap-juce-extensions
#   GIT_REPOSITORY https://github.com/free-audio/clap-juce-extensions.git
#   GIT_TAG origin/main)

# FetchContent_MakeAvailable (clap-juce-extensions)

# clap_juce_extensions_plugin(TARGET "${BaseTargetName}"
# CLAP_ID "com.InfernalLove.KissOfShame"
# CLAP_FEATURES effect tape saturation audio-effect)

target_link_libraries("${BaseTargetName}"
    PRIVATE
        # AudioPluginData           # If we'd created a binary data target, we'd link to it here
        "${BaseTargetName}BinaryData"
        
        juce_audio_basics
        juce_audio_devices
        juce_audio_formats
        juce_audio_plugin_client
        juce_audio_processors
        juce_audio_utils
        juce_core
        juce_data_structures
        juce_events
        juce_graphics
        juce_gui_basics
        juce_gui_extra
        juce_opengl
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

