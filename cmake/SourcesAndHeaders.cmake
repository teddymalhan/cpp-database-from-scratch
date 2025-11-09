set(sources
    src/tmp.cpp
    src/main_window.cpp
)

set(exe_sources
    src/main.cpp
    ${sources}
)

set(headers
    include/project/tmp.hpp
    include/project/main_window.hpp
)

set(test_sources
  src/tmp_test.cpp
)
