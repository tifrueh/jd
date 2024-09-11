# JD

The **jd** utility is a simple but powerful tool navigate and manipulate
[johnny.decimal](https://johnnydecimal.com) file systems with.

## Features

- List the contents of any directory in your johnny.decimal file system.

- Display the path to any directory in your johnny.decimal file system.

- Get and set the utility's configuration options from the command line or using
  a configuration file.

## Features that will be added in the future

- Check your johnny.decimal file system for misnamed directories.

- Find an id, category or area by a part of its name.

- Change the number of an area, category or id and update its subdirectories
  automatically.

## Building and installing

*Note: Right now, building and installing **jd** yourself is the only option. As
of the first tagged release, however, there will be a homebrew formula in my
homebrew tap and I'll also add a PGKBUILD to my [PKGBUILD
repository](https://github.com/tifrueh/PKGBUILDS).*

This project uses the meson build system, which you'll need to have installed to
be able to build **jd** without any additional hassle. Please refer to
[mesonbuild.com](https://mesonbuild.com) for information on how to install meson
on your system.

After having installed meson, you can execute the following commands in the root
directory of this repository to build and install **jd**:

```console
$ meson setup build
```

This will create a build directory of the name `build`.

Next, you can set additional configuration options, if you want, using:

```console
$ meson configure [...] -C build
```

To learn more about possible configuration options, use the `--help` flag.

Now, to build and install **jd**, execute the following command:

```console
$ meson install -C build
```

*Note: If you want to see what would be installed where without installing
anything, use the `--dry-run` flag.*

## Additional information

Please refer to the manual pages for any additional information on the program.
`jd(1)` is a good starting point. All other relevant pages are mentioned in its
`SEE ALSO` section.

If you want to view the manual pages without installing them, clone the
repository and navigate your terminal to it. You can then use `man` to display
any manual page contained in the `Documentation` directory of this repository.
This could be done, for example, like so:

```console
$ man ./Documentation/jd.1.in
```
