# escape=`

FROM mcr.microsoft.com/windows/servercore:ltsc2022

# reset the shell.
SHELL ["powershell", "-Command"]

COPY InstallBuildTools.cmd C:\TEMP\
ADD https://aka.ms/vscollect.exe C:\TEMP\collect.exe

# download channel for fixed install.
ARG CHANNEL_URL=https://aka.ms/vs/17/release/channel
ADD ${CHANNEL_URL} C:\TEMP\VisualStudio.chman

# download and install Build Tools for Visual Studio 2022 for native desktop workload.
ADD https://aka.ms/vs/17/release/vs_buildtools.exe C:\TEMP\vs_buildtools.exe
RUN C:\TEMP\InstallBuildTools.cmd C:\TEMP\vs_buildtools.exe --quiet --wait --norestart --nocache `
    --channelUri C:\TEMP\VisualStudio.chman `
    --installChannelUri C:\TEMP\VisualStudio.chman `
    --add Microsoft.VisualStudio.Workload.VCTools `
    --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    --add Microsoft.VisualStudio.Component.VC.CMake.Project `
    --includeRecommended `
    --installPath C:\BuildTools

RUN $vcDir = Get-ChildItem 'C:\BuildTools\VC\Tools\MSVC' | Sort-Object Name -Descending | Select-Object -First 1 ; `
    Write-Host "Detected MSVC version: $($vcDir.Name)" ; `
    setx BAZEL_VC "C:\BuildTools\VC" ; `
    setx BAZEL_VC_FULL "C:\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

# download & install GIT
ARG GIT_VERSION=2.44.0
ARG GIT_BINARY=Git-${GIT_VERSION}-64-bit.exe
ARG GIT_URL=https://github.com/git-for-windows/git/releases/download/v${GIT_VERSION}.windows.1/${GIT_BINARY}

COPY git.inf C:\TEMP\
ARG INSTALLER="C:\\TEMP\\${GIT_BINARY}"
ADD ${GIT_URL} ${INSTALLER}
RUN & $env:INSTALLER /SP- /VERYSILENT /SUPPRESSMSGBOXES /NOCANCEL /NORESTART /CLOSEAPPLICATIONS /RESTARTAPPLICATIONS /LOADINF=C:\TEMP\git.inf

# download & setup conan
ARG CONAN_VERSION=2.10.2
ARG CONAN_BINARY=conan-${CONAN_VERSION}-windows-x86_64-installer.exe
ARG CONAN_URL=https://github.com/conan-io/conan/releases/download/${CONAN_VERSION}/${CONAN_BINARY}

ARG INSTALLER="C:\\TEMP\\${CONAN_BINARY}"
ADD ${CONAN_URL} ${INSTALLER}
RUN & $env:INSTALLER /SP- /VERYSILENT /SUPPRESSMSGBOXES
SHELL ["cmd", "/S", "/C"]
RUN C:\BuildTools\VC\Auxiliary\Build\vcvars64.bat && conan profile detect --force

# switch back to PowerShell if needed
SHELL ["powershell", "-Command"]

COPY conanfile.txt C:\Conan\
WORKDIR C:\Conan\

RUN mkdir Libs

RUN conan install . --build=missing --deployer=direct_deploy --output-folder=.

RUN cp C:\Conan\direct_deploy\pcre2\lib\pcre2-8-static.lib C:\Conan\Libs\
RUN cp C:\Conan\direct_deploy\poco\lib\PocoFoundationmd.lib C:\Conan\Libs\

RUN setx LIB "C:\Conan\Libs;$env:LIB"

ARG BAZEL_VERSION=8.3.1
ARG BAZEL_INSTALLER="bazel-${BAZEL_VERSION}-windows-x86_64.exe"
ARG BAZEL_URL="https://github.com/bazelbuild/bazel/releases/download/${BAZEL_VERSION}/${BAZEL_INSTALLER}"

ADD ${BAZEL_URL} C:\\bazel\\bazel.exe

RUN setx PATH "C:\bazel;$env:PATH"
