# jdfs – johnny.decimal file system tool

The **jdfs** utility is a simple but powerful tool navigate and manipulate
[johnny.decimal](https://johnnydecimal.com) file systems with.

## Features

- List the contents of any directory in your johnny.decimal file system.

- Display the path to any directory in your johnny.decimal file system.

- Get and set the utility's configuration options from the command line or using
  a configuration file.

## Planned features for version 1.0.0

- Check your johnny.decimal file system for misnamed directories or errant
  files.

- Find an id, category or area by a part of its name.

- Change the number of an area, category or id and update its subdirectories
  automatically.

## Disclaimer

**This project is still in early development and has not been tested
extensively. If you experience any problems (you probably will), please do not
hesitate to open an issue here on GitHub.**

## Installation

### Homebrew (macOS)

I maintain a homebrew tap containing my own software, which means you can
easily install **jdfs** with `brew` on macOS.

1. Add my tap:

```console
$ brew tap tifrueh/mytap
```

2. Install **jdfs**:

```console
$ brew install tifrueh/mytap/jdfs
```

### PKGBUILD (Arch Linux)

You can find a PKGBUILD file in [my PKGBUILD
repository](https://github.com/tifrueh/PKGBUILDs/tree/main/jdfs).
Download it or clone the repository and then use `makepkg` and `pacman` to
install it as you would a package from the AUR.

*Note: I might even put this on the AUR, but only if I can see that more than a
handful of people are using it, as suggested by the [AUR submission
guidelines](https://wiki.archlinux.org/title/AUR_submission_guidelines). If you
are "more than a handful of people" and would like to see **jdfs** on the AUR,
don't hesitate to open a GitHub issue here on this repository. You can, of
course, also send me an email, if you prefer.*

### Build from source (macOS, Linux)

To build **jdfs** without any additional hassle, you'll need to have the meson
build system and a compiler toolchain installed. If you don't know how to do
this, there is a [guide on meson's
website](https://mesonbuild.com/SimpleStart.html) you can read.

After having installed meson and a compiler, you can execute the following
commands in the root directory of this repository to build and install **jdfs**:

```console
$ meson setup build
```

This will create a build directory of the name `build`.

Next, you can set additional configuration options, if you want, using:

```console
$ meson configure [...] -C build
```

To learn more about possible configuration options, use the `--help` flag.

Now, to build and install **jdfs**, execute the following command:

```console
$ meson install -C build
```

*Note: If you want to see what would be installed where without installing
anything, use the `--dry-run` flag.*

### Windows

The project, in its current state, does not support Windows and I do not intend
to port it to Windows. This is mainly because I don't have any experience and
very little knowledge about the Windows ecosystem, but also because I wrote
this program primarily for myself and I don't use Windows. :)

But if you *do* use Windows, would like to use **jdfs** *on* Windows and have an
idea on how to port it *to* Windows, you're very welcome to open an issue or
pull request, or to send me an email about it.

## Additional information

Please refer to the manual pages for any additional information on the program.
`jdfs(1)` is a good starting point. All other relevant pages are mentioned in its
`SEE ALSO` section.

If you want to view the manual pages without installing them, clone the
repository and navigate your terminal to it. You can then use `man` to display
any manual page contained in the `Documentation` directory of this repository.
This could be done, for example, like so:

```console
$ man ./Documentation/jdfs.1.in
```
