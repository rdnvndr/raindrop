TEMPLATE = subdirs
SUBDIRS += src
!isEmpty(BUILD_TESTS):SUBDIRS += tests
CONFIG += ordered

