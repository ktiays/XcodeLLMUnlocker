# Introduction

This project is an automation tool to bypass restriction of Xcode predictive code completion on China models.

It's based on the principles from [unixzii](https://gist.github.com/unixzii/6f25be1842399022e16ad6477a304286).

This method bypasses restrictions but will become ineffective after the device restarts. You can use the system's startup capabilities to automatically re-execute this operation.

# Prerequisites

-   Xcode is installed and run at least once.
-   SIP debugging restrictions are disabled (via `csrutil enable --without debug` command in recovery mode).

# Usage

You can find the executable in the release page.
If you want to build it yourself, please refer to the build section.

You should run this executable as root.

```bash
sudo ./xllmu
```

# Build

-   Clone the repository.

```bash
git clone https://github.com/ktiays/XcodeLLMUnlocker.git
```

-   Create a `build` folder in the project directory.

```bash
mkdir build && cd build
```

-   Configure the CMake project.

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

-   Compile

```bash
make
```
