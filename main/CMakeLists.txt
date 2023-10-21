# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS main.c lapTimer.c lapTimer_wifi.c  lapTimer_http.c lapTimer_ir_sensor.c lapTimer_splits.c lapTimer_button.c lapTimer_slaves.c lapTimer_led.c  # list the source files of this component
    INCLUDE_DIRS        # optional, add here public include directories
    PRIV_INCLUDE_DIRS   # optional, add here private include directories
    REQUIRES            # optional, list the public requirements (component names)
    PRIV_REQUIRES       # optional, list the private requirements
)
