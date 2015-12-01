set(CMAKE_C_COMPILER "/opt/intel/compilers_and_libraries_2016.0.109/linux/mpi/intel64/bin/mpiicc")
set(CMAKE_C_COMPILER_ARG1 "")
set(CMAKE_C_COMPILER_ID "Intel")
set(CMAKE_C_COMPILER_VERSION "16.0.0.20150815")
set(CMAKE_C_COMPILE_FEATURES "")
set(CMAKE_C90_COMPILE_FEATURES "")
set(CMAKE_C99_COMPILE_FEATURES "")
set(CMAKE_C11_COMPILE_FEATURES "")

set(CMAKE_C_PLATFORM_ID "Linux")
set(CMAKE_C_SIMULATE_ID "")
set(CMAKE_C_SIMULATE_VERSION "")

set(CMAKE_AR "/usr/bin/ar")
set(CMAKE_RANLIB "/usr/bin/ranlib")
set(CMAKE_LINKER "/usr/bin/ld")
set(CMAKE_COMPILER_IS_GNUCC )
set(CMAKE_C_COMPILER_LOADED 1)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_C_ABI_COMPILED TRUE)
set(CMAKE_COMPILER_IS_MINGW )
set(CMAKE_COMPILER_IS_CYGWIN )
if(CMAKE_COMPILER_IS_CYGWIN)
  set(CYGWIN 1)
  set(UNIX 1)
endif()

set(CMAKE_C_COMPILER_ENV_VAR "CC")

if(CMAKE_COMPILER_IS_MINGW)
  set(MINGW 1)
endif()
set(CMAKE_C_COMPILER_ID_RUN 1)
set(CMAKE_C_SOURCE_FILE_EXTENSIONS c;m)
set(CMAKE_C_IGNORE_EXTENSIONS h;H;o;O;obj;OBJ;def;DEF;rc;RC)
set(CMAKE_C_LINKER_PREFERENCE 10)

# Save compiler ABI information.
set(CMAKE_C_SIZEOF_DATA_PTR "8")
set(CMAKE_C_COMPILER_ABI "ELF")
set(CMAKE_C_LIBRARY_ARCHITECTURE "")

if(CMAKE_C_SIZEOF_DATA_PTR)
  set(CMAKE_SIZEOF_VOID_P "${CMAKE_C_SIZEOF_DATA_PTR}")
endif()

if(CMAKE_C_COMPILER_ABI)
  set(CMAKE_INTERNAL_PLATFORM_ABI "${CMAKE_C_COMPILER_ABI}")
endif()

if(CMAKE_C_LIBRARY_ARCHITECTURE)
  set(CMAKE_LIBRARY_ARCHITECTURE "")
endif()




set(CMAKE_C_IMPLICIT_LINK_LIBRARIES "mpifort;mpi;mpigi;dl;rt;pthread;imf;svml;irng;m;ipgo;decimal;cilkrts;stdc++;irc;svml;c;irc_s;dl;c")
set(CMAKE_C_IMPLICIT_LINK_DIRECTORIES "/opt/intel/compilers_and_libraries_2016.0.109/linux/mpi/intel64/lib/release_mt;/opt/intel/compilers_and_libraries_2016.0.109/linux/mpi/intel64/lib;/opt/intel/compilers_and_libraries_2016.0.109/linux/compiler/lib/intel64;/opt/intel/compilers_and_libraries_2016.0.109/linux/mkl/lib/intel64;/opt/intel/compilers_and_libraries_2016.0.109/linux/tbb/lib/intel64/gcc4.1;/opt/intel/compilers_and_libraries_2016.0.109/linux/compiler/lib/intel64_lin;/usr/local/stow/gcc-5.2/lib/gcc/x86_64-unknown-linux-gnu/5.2.0;/usr/local/stow/gcc-5.2/lib64;/lib64;/usr/lib64;/usr/local/stow/gcc-5.2/lib;/lib;/usr/lib")
set(CMAKE_C_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES "")



