# pkgj

PKGj是一款搭配NoNpDRM、NoNpPSM使用的PSV游戏下载器。（中文俗称黑商店）

# 功能

* **简单** 所见即所得.
* **独立性**, 无需PC，所有操作均在PSV上完成.
* **自动化** 下载、解压缩、安装、刷新livearea全部都会自动完成.
* **队列** 多任务下载支持.
* **文件格式** TSV格式文件列表（标准csv格式）.
* **安装** 可以安装PSV游戏、DLC、PSP游戏等等.

小缺陷:
* **无法实现后台下载** - 下载会在软件关闭或vita休眠时停止.


# 配置文件

pkgj 读取 `ux0:pkgj/config.txt` 或 `ur0:pkgj/config.txt`.作为配置文件
若文件不存在PKGj将会采用默认配置（1.00\[0.46\]+）

| 选项 | 介绍 |
| --- | --- |
| `url_games <URL>` | The URL of the PS Vita game list |
| `url_dlcs <URL>` | The URL of the PS Vita DLC list |
| `url_psm_games <URL>` | The URL of the PS Mobile list (see Q&A) |
| `url_psp_games <URL>` | The URL of the PSP game list |
| `url_psx_games <URL>` | The URL of the PSX game list |
| `url_comppack <URL>` | The URL of the PS Vita compatibility pack list |
| `install_psp_as_pbp 1` | Install PSP games as EBOOT.EBP files instead of ISO files (see Q&A) |
| `install_psp_psx_location uma0:` | Install PSP and PSX games on `uma0:` |
| `no_version_check 1` | Do not check for update when starting PKGj |



# 许可协议

This software is released under the 2-clause BSD license.

puff.h and puff.c files are under [zlib][] license.

[NoNpDrm]: https://github.com/TheOfficialFloW/NoNpDrm/releases
[npdrm_free]: https://github.com/kyleatlast/npdrm_free/releases
[NoPsmDrm]: https://github.com/frangarcj/NoPsmDrm/
[zrif_online_converter]: https://rawgit.com/mmozeiko/pkg2zip/online/zrif.html
[pkg_dec]: https://github.com/weaknespase/PkgDecrypt
[pkg_releases]: https://github.com/blastrock/pkgj/releases
[vitasdk]: https://vitasdk.org/
[libvita2d]: https://github.com/xerpi/libvita2d
[PSDLE]: https://repod.github.io/psdle/
[socat]: http://www.dest-unreach.org/socat/
[zlib]: https://www.zlib.net/zlib_license.html
[pkgj_travis]: https://travis-ci.org/blastrock/pkgj/
[pkgj_downloads]: https://github.com/blastrock/pkgj/releases
[pkgj_latest]: https://github.com/blastrock/pkgj/releases/latest
[pkgj_license]: https://github.com/blastrock/pkgj/blob/master/LICENSE
[img_travis]: https://api.travis-ci.org/blastrock/pkgj.svg?branch=master
[img_downloads]: https://img.shields.io/github/downloads/blastrock/pkgj/total.svg?maxAge=3600
[img_latest]: https://img.shields.io/github/release/blastrock/pkgj.svg?maxAge=3600
[img_license]: https://img.shields.io/github/license/blastrock/pkgj.svg?maxAge=2592000

# 捐赠
以下是原作者的BitCoin钱包代码
Bitcoin: 128vikqd3AyNEXEiU5uSJvCrRq1e3kRX6n
Monero: 45sCwEFcPD9ZfwD2UKt6gcG3vChFrMmJHUmVVBUWwPFoPsjmkzvN7i9DKn4pUkyif5axgbnYNqU3NCqugudjTWqdFv5uKQV
