PROJECT(ST)

cmake_minimum_required(VERSION 3.10)

# set(CMAKE_CXX_COMPILER /usr/bin/g++-7)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 17)

INCLUDE_DIRECTORIES(includes/cfitsio  includes/alglib utils tests)

set(SOURCEDIR utils)

# file(GLOB ALGLIB_SRC "includes/alglib/*.h" "includes/alglib/*.cpp")

# add_library(alglib.a STATIC ${ALGLIB_SRC})

add_executable(main main.cpp 
    ${SOURCEDIR}/calc_bg.cpp
    ${SOURCEDIR}/psf.cpp
    ${SOURCEDIR}/helper_types.cpp
    ${SOURCEDIR}/fits_io.cpp
    ${SOURCEDIR}/lsfit.cpp
    ${SOURCEDIR}/api.cpp

    tests/test_st.cpp
    tests/mock_data.cpp
    tests/test_alglib_lsfit.cpp

    # ${SOURCEDIR}/normal_io.cpp

    ${ALGLIB_SRC}
)


set(CMAKE_INSTALL_PREFIX ./)
install(TARGETS main 
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/bin
)


target_link_libraries(main 
    -L${CMAKE_SOURCE_DIR}/lib/cfitsio/ 
    -L${CMAKE_SOURCE_DIR}/lib/alglib/
    -L${CMAKE_SOURCE_DIR}/lib/
    cfitsio alglib stable_utils pthread
)


