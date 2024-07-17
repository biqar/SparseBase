if(APPLE)
  find_package(OpenMP)
  # From https://github.com/microsoft/LightGBM/pull/5563/files
  if(NOT OpenMP_FOUND)
    # libomp 15.0+ from brew is keg-only, so have to search in other locations.
    # See https://github.com/Homebrew/homebrew-core/issues/112107#issuecomment-1278042927.
    execute_process(COMMAND brew --prefix libomp
            OUTPUT_VARIABLE HOMEBREW_LIBOMP_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -I${HOMEBREW_LIBOMP_PREFIX}/include")
    set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I${HOMEBREW_LIBOMP_PREFIX}/include")
    set(OpenMP_C_LIB_NAMES omp)
    set(OpenMP_CXX_LIB_NAMES omp)
    set(OpenMP_omp_LIBRARY ${HOMEBREW_LIBOMP_PREFIX}/lib/libomp.dylib)
    find_package(OpenMP REQUIRED)
  endif()
else()
  find_package(OpenMP REQUIRED)
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/run_all_examples.py
  ${CMAKE_CURRENT_BINARY_DIR}/run_all_examples.py
  COPYONLY
)

file(COPY ${CMAKE_SOURCE_DIR}/examples/data DESTINATION ${CMAKE_BINARY_DIR}/examples/)

if (${USE_CUDA})
  add_subdirectory(cuda_example)
  add_subdirectory(array_example)
  add_subdirectory(example_experiment)
endif()
if(NOT ${_HEADER_ONLY} OR NOT ${USE_CUDA})
  add_subdirectory(csr_coo)
  add_subdirectory(degree_order)
  add_subdirectory(amd_order)
  add_subdirectory(custom_order)
  add_subdirectory(format_conversion)
  add_subdirectory(gray_order)
  add_subdirectory(rcm_order)
  add_subdirectory(sparse_feature)
  add_subdirectory(sparse_reader)
  add_subdirectory(custom_format)
  add_subdirectory(custom_converter)
  add_subdirectory(custom_experiment)
  add_subdirectory(linear_solver)

  if(${USE_METIS})
    add_subdirectory(metis_partition)
  endif()

endif()
