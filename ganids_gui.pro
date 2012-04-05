#-------------------------------------------------
#
# Project created by QtCreator 2012-03-19T00:55:34
#
#-------------------------------------------------

QT       += core gui

INCLUDEPATH +=  ../boost-1.49.0/include \
                ../re2

LIBS += -L../boost-1.49.0/lib -lpcap -lboost_log -lboost_filesystem -lboost_system -lboost_date_time -lboost_thread -lboost_regex \
        -L../re2/obj -lre2

TARGET = ganids_gui
TEMPLATE = app

SOURCES += main.cpp\
        ganidsmainwindow.cpp \
    trie_node.cpp \
    trie.cpp \
    rule.cpp \
    regex.cpp \
    packet.cpp \
    nids.cpp \
    common.cpp \
    ac.cpp \
    capturethread.cpp

HEADERS  += ganidsmainwindow.h \
    trie_node.h \
    trie.h \
    rule_spirit.h \
    rule.h \
    regex.h \
    packet.h \
    nids.h \
    my_nids_cuda.h \
    grammar.h \
    common.h \
    ac.h \
    capturethread.h

FORMS    += ganidsmainwindow.ui

OTHER_FILES += my_nids_cuda.cu \
    rules.txt

# CUDA settings (for Arch Linux)
CUDA_SOURCES += my_nids_cuda.cu
# Project dir and outputs
PROJECT_DIR = .#$$system(pwd)
OBJECTS_DIR = .#$$PROJECT_DIR/Obj
DESTDIR = .#../bin

# Path to cuda toolkit install
CUDA_DIR = /opt/cuda-toolkit
# GPU architecture
CUDA_ARCH = sm_10
# nvcc flags (ptxas option verbose is always useful)
NVCCFLAGS = --compiler-options -fno-strict-aliasing -use_fast_math --ptxas-options=-v
# include path
INCLUDEPATH += $$CUDA_DIR/include
# lib dir
QMAKE_LIBDIR += $$CUDA_DIR/lib64
# libs - note than i'm using a x_86_64 machine
LIBS += -lcudart
# join the includes in a line
CUDA_INC = $$join(INCLUDEPATH,' -I', '-I', ' ')

# Prepare the extra compiler configuration (taken from the nvidia forum - i'm not an expert in this part)
cuda.input = CUDA_SOURCES
cuda.output = ${OBJECTS_DIR}${QMAKE_FILE_BASE}.o
cuda.commands = $$CUDA_DIR/bin/nvcc -m64 -g -G -arch=$$CUDA_ARCH -c $$NVCCFLAGS $$CUDA_INC $$LIBS ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT}

cuda.dependcy_type = TYPE_C
cuda.depend_command = $$CUDA_DIR/bin/nvcc -g -G -M $$CUDA_INC $$NVCCFLAGS ${QMAKE_FILE_NAME}
# Tell Qt that we want add more stuff to the Makefile
QMAKE_EXTRA_COMPILERS += cuda
