SUMMARY = "TrebleTrouble application"
LICENSE = "GPLv3"
LIC_FILES_CHKSUM = "file://LICENSE;md5=84dcc94da3adb52b53ae4fa38fe49e5d"

SRC_URI = "file://LICENSE \
	   file://Makefile \
	   file://src/* \
	  "

PR = "0"
S = "${WORKDIR}"

do_compile() {
    mkdir bin && make trebletrouble
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 bin/trebletrouble ${D}${bindir}
}

FILES_${PN} = "${bindir}"
