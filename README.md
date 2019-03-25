| README.md |
|:---|

<p align="center">
  <img src="img/gimp.png" height="120" />
  <h3 align="center">gimp-rpc</h3>
</p>

### Features

<p align="center">
  <img src="img/demo.png" height="200" />
</p>

* Displays GIMP name and icon
* Displays currently selected tool
* Displays currently opened file
* Displays elapsed time in file

### Install

```sh
git clone https://github.com/mrousavy/gimp-rpc.git
gimptool-2.0 --install-admin gimp-rpc/plugin.c
```

> (Installer script coming soon)

<details>
  <summary>
    <b>Contribute</b>
  </summary>

1. Clone `gimp-rpc`

    ```sh
    git clone https://github.com/mrousavy/gimp-rpc.git
    cd gimp-rpc
    git submodule update --init --recursive
    ```

2. Build libraries

    * `gimp`

      ```sh
      cd gimp
      export INSTALL_PREFIX=$HOME/dev/gimp-prefix/
      export SRC_DIR=/vol/scratch/gimp-beta/src
      mkdir -p $INSTALL_PREFIX
      mkdir -p $SRC_DIR
      export PATH=$INSTALL_PREFIX/bin:$PATH
      export PKG_CONFIG_PATH=$INSTALL_PREFIX/lib/pkgconfig:$PKG_CONFIG_PATH
      export LD_LIBRARY_PATH=$INSTALL_PREFIX/lib:$LD_LIBRARY_PATH
      ./configure --prefix=$INSTALL_PREFIX
      ./autogen.sh --prefix=$INSTALL_PREFIX --disable-gtk-doc
      make
      make install
      ```

    * `discord-rpc`

      ```sh
      cd discord-rpc
      mkdir build
      cd build
      cmake .. -DCMAKE_INSTALL_PREFIX=<path to install discord-rpc to>
      cmake --build . --config Release --target install
      ```

3. Make changes to `gimp-rpc`

    The main plugin logic is programmed in the `plugin.c` file. Change as desired.

4. Build `gimp-rpc`

    ```sh
    gimptool-2.0 --install-admin plugin.c
    ```
</details>

<details>
  <summary>
    <b>Credits</b>
  </summary>
  <ul>
    <li><a href="https://github.com/discordapp/discord-rpc">discord-rpc</a></li>
    <li><a href="https://github.com/GNOME/gimp">libgimp</a></li>
  </ul>
</details>

<a href='https://ko-fi.com/F1F8CLXG' target='_blank'><img height='36' style='border:0px;height:36px;' src='https://az743702.vo.msecnd.net/cdn/kofi2.png?v=0' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>
