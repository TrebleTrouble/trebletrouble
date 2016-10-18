SUMMARY = "QtWebEngine combines the power of Chromium and Qt"

LICENSE = "LGPL-3.0 & BSD"
LIC_FILES_CHKSUM = " \
    file://src/core/browser_context_qt.cpp;md5=5fe719c44250955a5d5f8fb15fc8b1da;beginline=1;endline=35 \
    file://src/3rdparty/chromium/LICENSE;md5=537e0b52077bf0a616d0a0c8a79bc9d5 \
    file://LICENSE.LGPLv3;md5=8211fde12cc8a4e2477602f5953f5b71 \
    file://LICENSE.GPLv3;md5=88e2b9117e6be406b5ed6ee4ca99a705 \
    file://LICENSE.GPLv2;md5=c96076271561b0e3785dad260634eaa8 \
"

DEPENDS += " \
    ninja-native \
    qtwebchannel \
    qtbase qtdeclarative qtxmlpatterns qtquickcontrols \
    qtlocation \
    libdrm fontconfig pixman openssl pango cairo icu pciutils \
    libcap \
"

# when qtbase is built with xcb enabled (default with x11 in DISTRO_FEATURES),
# qtwebengine will have additional dependencies:
# contains(QT_CONFIG, xcb): REQUIRED_PACKAGES += libdrm xcomposite xcursor xi xrandr xscrnsaver xtst
# xscreensaver isn't covered in qtbase DEPENDS
DEPENDS += "${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'libxscrnsaver', '', d)}"

DEPENDS += "yasm-native"
EXTRA_QMAKEVARS_PRE += "GYP_CONFIG+=use_system_yasm"

# To use system ffmpeg you need to enable also libwebp, opus, vpx											    
# Only depenedencies available in oe-core are enabled by default
PACKAGECONFIG ??= "libwebp flac libevent libxslt speex"
PACKAGECONFIG[opus] = "WEBENGINE_CONFIG+=use_system_opus,,libopus"
PACKAGECONFIG[icu] = "WEBENGINE_CONFIG+=use_system_icu,,icu"
PACKAGECONFIG[ffmpeg] = "WEBENGINE_CONFIG+=use_system_ffmpeg,,libav"
PACKAGECONFIG[libwebp] = "WEBENGINE_CONFIG+=use_system_libwebp,,libwebp"
PACKAGECONFIG[flac] = "WEBENGINE_CONFIG+=use_system_flac,,flac"
PACKAGECONFIG[libevent] = "WEBENGINE_CONFIG+=use_system_libevent,,libevent"
PACKAGECONFIG[libxslt] = "WEBENGINE_CONFIG+=use_system_libxslt,,libxslt"
PACKAGECONFIG[speex] = "WEBENGINE_CONFIG+=use_system_speex,,speex"
PACKAGECONFIG[vpx] = "WEBENGINE_CONFIG+=use_system_vpx,,libvpx"

EXTRA_QMAKEVARS_PRE += "${PACKAGECONFIG_CONFARGS}"

COMPATIBLE_MACHINE = "(-)"
COMPATIBLE_MACHINE_x86 = "(.*)"
COMPATIBLE_MACHINE_x86-64 = "(.*)"
COMPATIBLE_MACHINE_armv6 = "(.*)"
COMPATIBLE_MACHINE_armv7a = "(.*)"
COMPATIBLE_MACHINE_armv7ve = "(.*)"

inherit qmake5
inherit gettext
inherit pythonnative
inherit perlnative

# we don't want gettext.bbclass to append --enable-nls
def gettext_oeconf(d):
    return ""

require qt5.inc
require qt5-git.inc

# To avoid trouble start with not separated build directory
SEPB = "${S}"
B = "${SEPB}"

export NINJA_PATH="${STAGING_BINDIR_NATIVE}/ninja"

do_configure() {
    # replace LD with CXX, to workaround a possible gyp inheritssue?
    export LD="${CXX}"
    export CC="${CC}"
    export CXX="${CXX}"
    export CC_host="gcc"
    export CXX_host="g++"
    export QMAKE_MAKE_ARGS="${EXTRA_OEMAKE}"
    export QMAKE_CACHE_EVAL="${PACKAGECONFIG_CONFARGS}"

    # Disable autodetection from sysroot:
    sed -i 's/packagesExist([^)]*vpx[^)]*):/false:/g; s/config_srtp:/false:/g; s/config_snappy:/false:/g; s/packagesExist(nss):/false:/g; s/packagesExist(minizip, zlib):/false:/g; s/packagesExist(libwebp,libwebpdemux):/false:/g; s/packagesExist(libxml-2.0,libxslt):/false:/g; s/^ *packagesExist($$package):/false:/g' ${S}/tools/qmake/mkspecs/features/configure.prf

    # qmake can't find the OE_QMAKE_* variables on it's own so directly passing them as
    # arguments here
    ${OE_QMAKE_QMAKE} -r ${EXTRA_QMAKEVARS_PRE} QTWEBENGINE_ROOT="${S}" \
        QMAKE_CXX="${OE_QMAKE_CXX}" QMAKE_CC="${OE_QMAKE_CC}" \
        QMAKE_LINK="${OE_QMAKE_LINK}" \
        QMAKE_CFLAGS="${OE_QMAKE_CFLAGS}" \
        QMAKE_CXXFLAGS="${OE_QMAKE_CXXFLAGS}" \
        QMAKE_AR="${OE_QMAKE_AR} cqs" \
        -after ${EXTRA_QMAKEVARS_POST}
}

do_install_append() {
    rmdir ${D}${OE_QMAKE_PATH_PLUGINS}/${BPN} ${D}${OE_QMAKE_PATH_PLUGINS} || true
    sed -i 's@ -Wl,--start-group.*-Wl,--end-group@@g; s@-L${B}[^ ]* @ @g' ${D}${libdir}/pkgconfig/Qt5WebEngineCore.pc
}
PACKAGE_DEBUG_SPLIT_STYLE = "debug-without-src"

# for /usr/share/qt5/qtwebengine_resources.pak
FILES_${PN} += "${OE_QMAKE_PATH_QT_TRANSLATIONS} ${OE_QMAKE_PATH_QT_DATA}"

RDEPENDS_${PN}-examples += " \
    ${PN}-qmlplugins \
    qtquickcontrols-qmlplugins \
    qtdeclarative-qmlplugins \
"

QT_MODULE_BRANCH_CHROMIUM = "45-based"

SRC_URI += " \
    ${QT_GIT}/qtwebengine-chromium.git;name=chromium;branch=${QT_MODULE_BRANCH_CHROMIUM};destsuffix=git/src/3rdparty \
    file://0001-functions.prf-Don-t-match-QMAKE_EXT_CPP-or-QMAKE_EXT.patch \
    file://0002-functions.prf-Make-sure-we-only-use-the-file-name-to.patch \
    file://0003-functions.prf-allow-build-for-linux-oe-g-platform.patch \
    file://0004-WebEngine-qquickwebengineview_p_p.h-add-include-QCol.patch \
    file://0005-Include-dependency-to-QCoreApplication-translate.patch \
    file://0001-chromium-base.gypi-include-atomicops_internals_x86_g.patch \
    file://0002-chromium-Change-false-to-FALSE-and-1-to-TRUE-FIX-qtw.patch \
"

SRCREV_qtwebengine = "808ca6e6917cf17e4c30fcd3ac609164b36594cc"
# This is in git submodule, but we're using latest in 45-based
# SRCREV_chromium = "79930a541473b2e0f950d040c16ab6f22e4aeef3"
SRCREV_chromium = "9d6566ee193e8996fb867cf776dc13f697400f24"
SRCREV = "${SRCREV_qtwebengine}"

SRCREV_FORMAT = "qtwebengine_chromium"

S = "${WORKDIR}/git"

# WARNING: qtwebengine-5.5.99+5.6.0-rc+gitAUTOINC+3f02c25de4_779a2388fc-r0 do_package_qa: QA Issue: ELF binary '/OE/build/oe-core/tmp-glibc/work/i586-oe-linux/qtwebengine/5.5.99+5.6.0-rc+gitAUTOINC+3f02c25de4_779a2388fc-r0/packages-split/qtwebengine/usr/lib/libQt5WebEngineCore.so.5.6.0' has relocations in .text [textrel]
INSANE_SKIP_${PN} += "textrel"
