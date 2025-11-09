set(sources
    src/tmp.cpp
    src/main_window_core.cpp
)

set(objcxx_sources
    src/app_delegate.mm
    src/main_window_controller.mm
)

set(exe_sources
    src/main.mm
    ${sources}
    ${objcxx_sources}
)

set(headers
    include/project/tmp.hpp
    include/project/main_window_core.hpp
    include/project/app_delegate.hpp
    include/project/main_window_controller.hpp
)

set(test_sources
  src/tmp_test.cpp
)
