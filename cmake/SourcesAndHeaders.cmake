set(sources
    src/tmp.cpp
    src/main_window_core.cpp
    src/database/value.cpp
    src/database/schema.cpp
    src/database/tuple.cpp
    src/database/page.cpp
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
    include/database/types.hpp
    include/database/value.hpp
    include/database/schema.hpp
    include/database/tuple.hpp
    include/database/page.hpp
)

set(test_sources
  src/tmp_test.cpp
  src/types_test.cpp
  src/value_test.cpp
  src/schema_test.cpp
  src/tuple_test.cpp
  src/page_test.cpp
)
