＃pkgj

该自制软件可以直接在Vita上与您通过使用[NoNpDrm]或[NoPsmDrm]插件伪造许可一起下载并解压缩pkg文件。

＃ 特征

* **适用于所有PS Vita型号，包括PSTV。
*轻松**查看可用下载列表的方式，包括搜索，过滤和排序。
* **独立**，无需PC，一切都直接在Vita上进行。
*自动下载并解压缩，只需选择一个项目，它将被安装，包括实时区域中的气泡。
* **后台下载**，现在支持本机bgdl功能，因此您可以在下载内容时在控制台上执行任何操作。
* **队列**多次下载。
* **支持** TSV文件格式。
* **安装**游戏更新，DLC，演示，主题，PSM，PSP游戏，PSP DLC和PSX游戏。

当前限制：
* **没有PSX / PSP / PSM标题的后台下载**-如果应用程序关闭或Vita处于睡眠状态，则下载将停止。

＃ 下载

在[vpk档案这里] [pkgj_latest]取得最新版本。

＃用法

确保在Henkaku设置中启用了不安全模式。

使用应用程序非常简单。选择要安装的项目，然后按X并按照说明进行操作。要排序/过滤/搜索，请按三角形。
它将打开上下文菜单。再次按三角键以确认您在菜单中所做的选择。或按O取消您所做的任何更改。

按向左或向右按​​钮可向上或向下移动页面。

＃配置

pkgj附带有效的默认URL。如果您想更改某些设置，可以通过ux0：pkgj / config.txt或ur0：pkgj / config.txt进行配置。

|选项|描述
| --- | --- |
| url_games <URL>`| PS Vita游戏列表的网址|
| url_psv_demos <URL> PS Vita演示列表的URL |
| `url_dlcs <URL>`| PS Vita DLC列表的URL |
| url_psv_themes <URL> PS Vita主题列表的URL |
| url_psm_games <URL>`| PS Mobile列表的URL（请参阅“问答”）|
| url_psp_games <URL>`| PSP游戏列表的网址|
| url_psp_dlcs <URL> PSP DLC列表的URL |
| url_psx_games <URL>`| PSX游戏列表的网址|
| url_comppack <URL> PS Vita兼容性包列表的URL |
| `install_psp_as_pbp 1` |将PSP游戏安装为EBOOT.EBP文件而不是ISO文件（请参阅“问答”）|
| `install_psp_psx_location uma0：`|在“ uma0：”上安装PSP和PSX游戏。
| `no_version_check 1` |启动PKGj时不检查更新。

pkgj 读取 ux0:pkgj/font.ttf 作为游戏列表显示字体 若文件不存则使用系统字体
＃问答

1.在哪里删除中断/失败的下载以释放空间（仅PSV更新/ PSX / PSP游戏）？

    如果是PSV内容：只需删除您生活区中的排队下载。如果由于某种原因不能解决问题，您可以随时删除ux0：bgdl / t /中的文件夹-每次下载将按照排队的顺序放在单独的文件夹中。

    其他方面：ux0：pkgj文件夹-每个下载文件的标题ID均位于单独的文件夹中。只需删除文件夹和恢复文件。

2.下载速度太慢！

    通常，您应该看到约1-2 MB / s的速度。这对于Vita硬件而言是正常的。当然，这还取决于您拥有的WiFi路由器和WiFi信号强度。但是有时速度会降至仅几百KB / s。对于包含许多小文件或许多文件夹的pkg文件，会发生这种情况。创建新文件或新文件夹会花费额外的时间，这会减慢下载速度。

3.我想将PSP游戏安装为EBOOT文件。

    可以将PSP游戏安装为EBOOT文件。它允许更快地安装游戏，并使其占用更少的空间。但是，您需要安装[npdrm_free]插件才能使它们工作。

    要将PSP游戏安装为EBOOT文件，只需将以下行添加到您的配置中：
    ```
    install_psp_as_pbp 1
    ```

    如果要切换回其他模式，只需删除该行。写入0是不够的。

4.我不能玩PSP游戏，提示“游戏无法启动（80010087）”。

    您需要在VSH中安装[npdrm_free]插件，或将游戏安装为ISO。

1. PSM游戏不起作用。

    如果您按照[NoPsmDrm]的说明进行操作，则可以尝试使用[NoPsmDrm Fixer]（https://github.com/Yoti/psv_npdrmfix）激活psm游戏帐户。

6.无法在我的PSTV上下载更新或DLC

    此错误是由AntiBlackList引起的。要修复此问题，请完全撤消然后卸载AntiBlackList，然后安装[DolcePolce]（https://forum.devchroma.nl/index.php/topic,58.0.html）插件。

7.如何使用兼容包？

    默认情况下，不推荐使用和禁用兼容性包。建议使用[reF00D]（https://github.com/dots-tb/reF00D）或[0syscall6]（https://github.com/SKGleba/0syscall6）。如果您仍想使用兼容包，请在配置文件中将url_comppack设置为https://gitlab.com/nopaystation_repos/nps_compati_packs/raw/master/。固件3.65或更低版本需要TLS的解决方法。自2019年10月以来，兼容性包列表尚未更新。

＃ 编译

pkgj使用柯南和cmake进行构建。设置有些繁琐，因此
推荐的方法是运行ci / ci.sh。它将使用以下命令创建一个Python virtualenv
柯南，为交叉编译设置配置，注册一些配方，
然后运行cmake并为您的vita和pkgj_cli构建pkgj进行测试。

先决条件：

* Debian软件包（或等效的软件包）：

  -必要的
  -git-core
  -使
  -cmake
  -python3-pip
  -pipenv（pip3 install --user pipenv）
  -忍者生成

pkgj将在ci / build中构建，您可以通过运行任意时间对其进行重建
忍者在同一目录中。

您可以将环境变量“ PSVITAIP”（在运行cmake之前）设置为的IP地址。
Vita，它将允许使用make send直接将eboot.bin文件发送到ux0：app / PKGJ00000文件夹。

要启用调试日志记录，请将`-DPKGI_ENABLE_LOGGING = ON`参数传递给cmake。然后应用程序将调试消息发送到
UDP多播地址239.255.0.100:30000。要接收它们，您可以在PC上使用[socat]：

    $ socat udp4-recv：30000，ip-add-membership = 239.255.0.100：0.0.0.0-

＃ 许可

该软件根据2条款BSD许可发布。

puff.h和puff.c文件受[zlib]许可。

[NoNpDrm]：https://github.com/TheOfficialFloW/NoNpDrm/releases
[npdrm_free]：https://github.com/kyleatlast/npdrm_free/releases
[NoPsmDrm]：https://github.com/frangarcj/NoPsmDrm/
[zrif_online_converter]：https://rawgit.com/mmozeiko/pkg2zip/online/zrif.html
[pkg_dec]：https：//github.com/weaknespase/PkgDecrypt
[pkg_releases]：https：//github.com/blastrock/pkgj/releases
[vitasdk]：https：//vitasdk.org/
[libvita2d]：https://github.com/xerpi/libvita2d
[PSDLE]：https：//repod.github.io/psdle/
[socat]：http://www.dest-unreach.org/socat/
[zlib]：https://www.zlib.net/zlib_license.html
[pkgj_travis]：https://travis-ci.org/blastrock/pkgj/
[pkgj_downloads]：https://github.com/blastrock/pkgj/releases
[pkgj_latest]：https://github.com/blastrock/pkgj/releases/latest
[pkgj_license]：https://github.com/blastrock/pkgj/blob/master/LICENSE
[img_travis]：https://api.travis-ci.org/blastrock/pkgj.svg?branch=master
[img_downloads]：https：//img.shields.io/github/downloads/blastrock/pkgj/total.svg？maxAge = 3600
[img_latest]：https://img.shields.io/github/release/blastrock/pkgj.svg?maxAge=3600
[img_license]：https：//img.shields.io/github/license/blastrock/pkgj.svg？maxAge = 2592000

＃捐赠

比特币：128vikqd3AyNEXEiU5uSJvCrRq1e3kRX6n
门罗币：45sCwEFcPD9ZfwD2UKt6gcG3vChFrMmJHUmVVBUWwPFoPsjmkzvN7i9DKn4pUkyif5axgbnYNqU3NCqugudjTWqdFv5uKQV