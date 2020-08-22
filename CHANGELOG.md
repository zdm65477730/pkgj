# Changelog

## 0.55

What's new:

- Compatibility packs are disabled by default.
- Remove legacy patch installation method.
- Support PSP Neogeo games. (credits: Yoti)

## 0.54

**Errors when refreshing:** Delete your config and completely uninstall [ITLS-enso](https://github.com/SKGleba/iTLS-Enso#faq).

**PSTV users:** Completely uninstall AntiBlacklist/Whitelist and install [DolcePolce](https://github.com/KuromeSan/DolcePolce).

What's new:

- PSM games can now be installed automatically without rebuilding database. (credits: SilicaAndPina)
- PSM runtime can be installed directly in PKGJ without Henkaku DNS.
- Cover images for games. (credits: dragonflylee)
- Exit from gameview with the cancel button. (credits: dragonflylee)
- Compat packs default URL changed and always enabled.
- Enhanced HTTP error messages.

Bug fixes:

- Fix PSM Unity extraction path.
- Fix all TLS errors with default URLs.

## 0.53

What's new:

- Compat pack lists are no longer downloaded if either reF00D or 0syscall6 are installed. This prevents a TLS 1.2 error for users of reF00D or 0syscall6 on firmware <=3.65 without ITLS-enso.
- Support Korean font.

Bug fixes:

- Stop input processing when IME dialog (search dialog) is active.
- Show all menu items when search is active.
- Install PSP DLC to configured partition.

## 0.52

What's new:

- 启动时增加 NoNpDrm 检查
- 检测 reF00D 插件, 添加 0syscall6 检测支持
- 优化 HTTPS 相关提示, 提示用户安装 [iTLS-Enso](https://github.com/SKGleba/iTLS-Enso/releases/latest)

Bug fixes:

- Removed (offline) proxy gate in default comppack_url in favor of iTLS-Enso.
- Fixed 'compitibility' typos.

## 0.51

Bug fixes:

- Fixed downloading of some PSP games as ISO files

## 0.50

Bug fixes:

- Fixed PSP manual extraction

## 0.49

What's new:

- Added support for some PSP games thanks to @edgeyboy

Bug fixes:

- Fixed detection of PSP game installation
- Fixed unknown region for some titles

## 0.48

What's new:

- Added support for PSP DLC installation thanks to @lusid1's work

## 0.47

What's new:

- pkgj now uses LiveArea's downloader
- Added support for theme installation
- Added support for demo installation

## 0.46

What's new:

- pkgj will not check for game presence anymore when installing DLCs. This is
    useful for people who want to install DLCs for physical games.
- Changed diagnostics messages to take into account reF00D presence

## 0.45

Bug fixes:

- Fix occasional bad_alloc errors

## 0.44

What's new:

- It is now possible to download the latest patch of the selected game. The
    minimum required firmware version will take into account that latest patch.
- It is now possible to cancel base or patch compatibility pack installation
    independently
- There is no more need for keeping URLs in the configuration file. Default URLs
    have been set up, it is still possible to override them in the configuration
    file.
- pkgj now works when there is no config file at all

Bug fixes:

- Button repeat when keeping a button pressed on game view is now fixed
- Fixed error when downgrading to base compatibility pack

## 0.43

What's new:

- Database is now stored as plain files, refresh has become much faster
- When installing a base compatibility pack, the rePatch directory is not erased
    anymore, to keep potential user-installed custom patches

## 0.42

What's new:

- When in Japanese mode, O now works for confirm in game view
- Reduced size of the game view window to show the progress bar below

Bug fixes:

- Fixed cancelation of DLC and PSP and PSX games

## 0.41

Bug fixes:

- Do not crash if ur0:app does not exist

## 0.40

What's new:

- It is now possible to filter by installed games
- When changing mode, the list scrolls back to the top

## 0.39

Bug fixes:

- Fixed installation of DLCs and PSX games

## 0.38

What's new:

- There is a new window when clicking on a game which makes it easier to install
    games and compatibility packs
- Added a diagnosis about game installation that helps detecting
    game/compatibility pack version mismatch
- Download cancelation is now more reactive

Removed features:

- PKGj can't download game updates anymore as they were outdated. Use the
    livearea to update your games.

## 0.37

What's new:

- PKGj will now use `ux0:pkgj` and `ur0:pkgj` as configuration and temporary
    file folder. If the configuration file can't be found, it will fallback to
    `pkgi` folders

Bug fixes:

- Fixed crash on application start or on download cancel

## 0.36

What's new:

- It is now possible to download the new version of pkgj when one becomes
    available

## 0.35

Bug fixes:

- Fixed bad_alloc error that happened when installing some PSP games

## 0.34

What's new:

- A message will appear upon start when a new pkgj version is available (unless
    `no_version_check 1` is in the config file)
- Dialog messages have been reworked, they include word wrapping for error
    messages that are too long
- The menu's refresh item now refreshes all lists at once, including
    compatibility packs
- "Show updates" has been renamed to "Show legacy updates" to reflect the fact
    that they are not the last available version. Use the livearea to install
    the last version of a game.

## 0.33

What's new:

- Added PSM support

## 0.32

What's new:

- Added support for downloading patches compatibility packs. No changes to
    config.txt
- Added app version and required firmware on game update list
- When updates are installed, they now correctly appear as a white bullet
- New LiveArea design from JesterIOS

Bug fixes:

- Sort by date option is now correctly saved to configuration

## 0.31

What's new:

- Added support for downloading compatibility packs with the `url_comppack`
    configuration option
- All games are now listed, reguardless of firmware version

## 0.30

What's new:

- Temporary data is now also deleted when deleting resume data after a resume
    failure
- Implemented update of Shell's database when installing an update so that
    livearea doesn't say that an update is available when it's already installed

Bug fixes:

- Resume data is now deleted after a successful installation to avoid an error
    when reinstalling

## 0.29

Bug fixes:

- Fixed error when unsafe mode is not activated
- Fixed downloading of games that contain a single file bigger than 2GB

## 0.28

What's new:

- Resume data is now deleted automatically when resuming fails

Bug fixes:

- Downloaded package integrity is now correctly checked

## 0.27

What's new:

- Improved various errors
- Current mode now appears in the top bar
- Added support for downloading packages without digest

## 0.26

What's new:

- ux0:patch is created on first update installation if it doesn't exist yet

Bug fixes:

- Search feature now works

## 0.25

Bug fixes:

- Fix error messages from previous version

## 0.24

What's new:

- Download resume feature was restored (except when downloading PSP games as ISO
  files)
- Made a couple errors more explicit

## 0.23

Bug fixes:

- Sorting by size is now fixed

## 0.22

What's new:

- Allowed sorting by last modification date
- Games that are not supported on the current firmware are now hidden
- When the list fails to download, there is no more need to exit pkgj

Bug fixes:

- Fixed crash when pressing X on an empty list

## 0.21

What's new:

- Percentage and speed are now shown during download

Bug fixes:

- When changing list, the list scrolls back up

## 0.20

What's new:

- List content is now cached and there is an explicit command to refresh it

## 0.19

What's new:

- Added option `install_psp_psx_location` to install games to other partitions
  than `ux0:`

Removed features:

- pkgj will not try to load pkgi.txt anymore

## 0.18

What's new:

- PSP games now default to an ISO installation. Add `install_psp_as_pbp 1` in
  your config.txt to install EBOOT.PBP files.
- Added support for PC Engine games.

## 0.17

What's new:

- PSP games are now installed as EBOOT.pbp files. They are smaller and faster
  to install. You will need to install the npdrm_free plugin in VSH to make
  them work.
- Added support for PSP-Mini games
- Increased menu size

## 0.16

What's new:

- Added automatic creation of PSP and PSX directories
- Improved installation error reporting

## 0.15

What's new:

- Further improved HTTP error reporting

## 0.14

What's new:

- Added support for PSP games
- Improved HTTP error reporting

## 0.13

Bug fixes:

- It is now possible to download more than 4 packages

## 0.12

What's new:

- More design in LiveArea screen from @JesterIOS
- Added download error reporting to user
- Do not rely on TSV column header anymore to determine the mode

Removed features:

- Removed support for old pkgi database format

## 0.11

What's new:

- Replaced LiveArea graphics by images from @JesterIOS
- Kept compatibility with old update TSV format

## 0.10

What's new:

- Updated to new TSV format

Bug fixes:

- Fixed bug that would hide the last line of TSV files
- Fixed installation of certain games

Removed features:

- Can't resume downloads anymore (will be restored later)

## 0.09

What's new:

- Added support for PSX games installation
- Disabled version check because of a SSL issue preventing me from testing the
  code

## 0.08

What's new:

- Items with missing URL or zRIF string are now discarded (instead of shown on a
  red background)

Bug fixes:

- Fixed crash when refreshing updates
- Fixed bug where some item where shown as installed when they were not or
  "corresponding game not installed" when it actually is

## 0.07

What's new:

- It is now possible to cancel downloads by pressing X once again on a package
- The current download is now shown at the bottom
- A progress bar of the current download is now shown at the bottom
- Games that can't be installed because the URL or the zRIF string is missing
  are now shown on red background

Bug fixes:

- Fixed a crash when trying to download a package that has no URL
