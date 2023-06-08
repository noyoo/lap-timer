# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS main.c lapTimer.c lapTimer_wifi.c  lapTimer_i2c.c ../core/src/vl53l0x_api.c ../platform//src/vl53l0x_platform.c ../core/src/vl53l0x_api_calibration.c ../core/src/vl53l0x_api_strings.c ../core/src/vl53l0x_api_core.c  # list the source files of this component
    INCLUDE_DIRS ../core/inc ../platform/inc       # optional, add here public include directories
    PRIV_INCLUDE_DIRS   # optional, add here private include directories
    REQUIRES            # optional, list the public requirements (component names)
    PRIV_REQUIRES       # optional, list the private requirements
)
