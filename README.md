# Modsec Test

Example code for testing Modsec build on Windows. The modsec is built on windows using information provided in the [repo](https://github.com/owasp-modsecurity/ModSecurity/tree/v3/master/build/win32/README.md). 
The built dll is then imported using a [cc_import](https://bazel.build/reference/be/c-cpp#cc_import) rule of Bazel.
