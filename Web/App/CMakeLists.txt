cmake_minimum_required(VERSION 3.12)
project(OpenSiv3D_Linux_App CXX C)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 -pg -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g3 -O2 -pg")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Wextra -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -O2 -pg")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")

if (NOT ${CMAKE_C_FLAGS} MATCHES "-msimd128")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_XM_NO_INTRINSICS_")
endif ()

if (NOT ${CMAKE_CXX_FLAGS} MATCHES "-msimd128")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_XM_NO_INTRINSICS_")
endif ()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --no-export-dynamic -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s ALLOW_MEMORY_GROWTH=1")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 --js-library ${CMAKE_SOURCE_DIR}/../Siv3D.js")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_OGG=1 -s USE_LIBPNG=1  -s USE_VORBIS=1")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --pre-js ${CMAKE_SOURCE_DIR}/../Siv3D.pre.js")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --js-library ${CMAKE_SOURCE_DIR}/../Siv3D.js")

if (NOT CMAKE_BUILD_TYPE STREQUAL "MINSIZEREL")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s EXCEPTION_CATCHING_ALLOWED=[\"InitSiv3D\"]")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s EXCEPTION_CATCHING_ALLOWED=[\"InitSiv3D\"]")
endif()

if (SIV3D_WEBGPU_BACKEND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSIV3D_WEBGPU_BACKEND")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSIV3D_WEBGPU_BACKEND")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_WEBGPU=1 --post-js \"${CMAKE_SOURCE_DIR}/../Siv3D.post.js\"")
else()
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s FULL_ES3 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2")
endif()

if (SIV3D_RUN_TEST_ON_NODE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSIV3D_ENABLE_TEST=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lnodefs.js --pre-js \"${CMAKE_SOURCE_DIR}/../../Test/Siv3DTest.js\"")
else()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file \"${CMAKE_SOURCE_DIR}/resources@/resources\" --preload-file \"${CMAKE_SOURCE_DIR}/example@/example\"")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASYNCIFY=1 -s ASYNCIFY_IGNORE_INDIRECT=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASYNCIFY_IMPORTS=\"[ 'siv3dRequestAnimationFrame', 'siv3dOpenDialog', 'siv3dDecodeAudioFromFile', 'siv3dGetClipboardText' ]\"")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASYNCIFY_ADD=\"[ 'main', 'Main()', 's3d::System::Update()', 's3d::AACDecoder::decode(*) const', 's3d::MP3Decoder::decode(*) const', 's3d::CAudioDecoder::decode(*)', 's3d::AudioDecoder::Decode(*)', 's3d::Wave::Wave(*)', 's3d::Audio::Audio(*)', 's3d::Clipboard::GetText(*)', 's3d::CClipboard::getText(*)' ]\"")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

if (NOT CMAKE_CONFIGURATION_TYPES AND 
    NOT CMAKE_NO_BUILD_TYPE AND
    NOT CMAKE_BUILD_TYPE AND
    CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    message(STATUS "[!] Setting build type to 'Release' as none was specified.")
    set(CMAKE_BUILD_TYPE Release)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  add_compile_options ("-fcolor-diagnostics")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options ("-fdiagnostics-color=always")
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

if (SIV3D_RUN_TEST_ON_NODE)
  add_executable(Siv3DTest
    Main.cpp
    ../../Test/Siv3DTest.cpp
    ../../Test/Siv3DTest_Array.cpp
    ../../Test/Siv3DTest_BinaryReader.cpp
    ../../Test/Siv3DTest_BinaryWriter.cpp
  #  ../../Test/Siv3DTest_FileSystem.cpp
    ../../Test/Siv3DTest_Image.cpp
    ../../Test/Siv3DTest_Resource.cpp
    ../../Test/Siv3DTest_TextEncoding.cpp
    ../../Test/Siv3DTest_TextReader.cpp
    ../../Test/Siv3DTest_TextWriter.cpp
    )
else ()
  add_executable(Siv3DTest
    Main.cpp
  )
endif()

target_include_directories(Siv3DTest PRIVATE
  "../../Siv3D/include"
  "../../Siv3D/include/ThirdParty"
  )

if (${CMAKE_EXE_LINKER_FLAGS} MATCHES "-pthread")

  target_link_libraries(Siv3DTest PRIVATE
    ${PROJECT_SOURCE_DIR}/../build/libSiv3D.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-mt-simd/opencv/libopencv_objdetect.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-mt-simd/opencv/libopencv_photo.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-mt-simd/opencv/libopencv_imgproc.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-mt-simd/opencv/libopencv_core.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-mt-simd/harfbuzz/libharfbuzz.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-mt-simd/freetype/libfreetype.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-mt-simd/turbojpeg/libturbojpeg.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-mt-simd/giflib/libgif.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-mt-simd/webp/libwebp.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-mt-simd/opus/libopusfile.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-mt-simd/opus/libopus.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-mt-simd/tiff/libtiff.a
    )

else ()

  target_link_libraries(Siv3DTest PRIVATE
    # ${PROJECT_SOURCE_DIR}/../build/libSiv3DScript.a
    ${PROJECT_SOURCE_DIR}/../build/libSiv3D.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/opencv/libopencv_objdetect.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/opencv/libopencv_photo.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/opencv/libopencv_imgproc.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/opencv/libopencv_core.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/harfbuzz/libharfbuzz.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/freetype/libfreetype.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/turbojpeg/libturbojpeg.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/giflib/libgif.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/webp/libwebp.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/opus/libopusfile.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/opus/libopus.a
    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/tiff/libtiff.a

    ${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/SDL2/libSDL2.a
    )

endif ()

if(BUILD_TESTING)
enable_testing()
add_test(
  NAME Test
  COMMAND node Siv3DTest.js
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
endif()
