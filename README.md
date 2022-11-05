## What's this?
I made a boot animation for my linux machine, which is a glowing tracer running in
an upside down triangle path. The whole triangle will spin then flash when the
booting process is completed.

## Preview
### Loop
![loopityloop](preview/loop.gif)

### End
![endityend](preview/end.gif)

## Compiling
### Dependencies
`sfml` for rendering C++ animation scene

`imagemagick` for glowing effect

`optipng` for png optimization

### Commands
Just run `./make` from the repo root

## Installing
After compiling, there should be a folder name `build`, copy it as
`/usr/share/plymouth/themes/hueless-plymouth`, you can also use prebuilt package in the
![Release](https://github.com/Cationiz3r/huelessos-plymouth/releases) page

To verify that the theme has been installed correctly, `huelessos-plymouth` should appear
in the output of
```sh
plymouth-set-default-theme -l
```

From here, to make the boot animation permanent, run
```sh
sudo plymouth-set-default-theme -R huelessos-plymouth
```

## TODO
- [x] Remove rendering to screen while making theme
- [x] Make release package
