SUMMARY = "An image which boots into the TrebleTrouble application"
HOMEPAGE = "http://uottawa.ca"

IMAGE_LINGUAS = "en-us"

require qt5-image.bb

IMAGE_INSTALL += " \
    trebletrouble \
 "

export IMAGE_BASENAME = "trebletrouble-image"
