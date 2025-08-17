# Steps to build PCRE2

```shell
cmake -B build -S . -DPCRE2_BUILD_PCRE2_16=ON -DPCRE2_BUILD_PCRE2_32=ON -DPCRE2_SUPPORT_UNICODE=ON -DPCRE2_SUPPORT_JIT=ON -DPCRE2_BUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX="C:\pcre2"

cmake --build build --config Release
cmake --install build --config Release
```