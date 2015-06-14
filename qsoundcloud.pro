TEMPLATE = subdirs
SUBDIRS = sub_src sub_qml #sub_tests

sub_src.subdir = src
sub_qml.subdir = qml
sub_qml.depends = sub_src
sub_tests.subdir = tests
sub_tests.depends = sub_src
