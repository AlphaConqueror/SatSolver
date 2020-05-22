BIN_NAME    := satsolver
TESTER_NAME := testrunner

MAIN_FILE   := main
BIN_DIR     := bin
BUILD_DIR   := build
SRC_DIR     := src
INCLUDE_DIR := include
TEST_DIR    := test
TEST_SCRIPT = $(TEST_DIR)/run_tests.py
Q ?= @

SOURCES := $(wildcard ${SRC_DIR}/*.c)
OBJECTS := $(patsubst ${SRC_DIR}/%.c,${BUILD_DIR}/%.o,${SOURCES})

TEST_BUILD_DIR := ${BUILD_DIR}/${TEST_DIR}
TEST_SOURCES   := $(wildcard ${TEST_DIR}/*.c)
OBJECTS_NOMAIN := $(filter-out ${BUILD_DIR}/$(MAIN_FILE).o, $(OBJECTS))
# in this project there are only integration tests, thus the object files satsolver are not to be linked with the test object files
TEST_OBJECTS   := $(patsubst ${TEST_DIR}/%.c,${TEST_BUILD_DIR}/test.%.o,${TEST_SOURCES}) #$(OBJECTS_NOMAIN)

HEADERS := $(wildcard $(INCLUDE_DIR)/*.h) $(wildcard $(TEST_DIR)/*.h)

DEBUG   := -O0 -g -fsanitize=address -fsanitize=undefined
OPT     := -O3
CFLAGS  += -I $(INCLUDE_DIR)
CFLAGS  += -Wall -Wextra -Wpedantic
CFLAGS  += $(DEBUG) # this option is disabled on the server for better performance
LDFLAGS +=

.PHONY: clean $(BIN_NAME) $(TESTER_NAME)

all: $(BIN_NAME) $(TESTER_NAME)

$(BIN_NAME): ${BIN_DIR}/$(BIN_NAME)
$(TESTER_NAME): ${BIN_DIR}/$(TESTER_NAME)

${BIN_DIR}/$(BIN_NAME): ${OBJECTS}
	$(Q)mkdir -p ${BIN_DIR}
	@echo "===> LD $@"
	$(Q)$(CC) -o $@ ${CFLAGS} $(OPT) $+ ${LDFLAGS}

${BIN_DIR}/$(TESTER_NAME): ${TEST_OBJECTS}
	$(Q)mkdir -p ${BIN_DIR}
	@echo "===> LD $@"
	$(Q)$(CC) -o $@ ${CFLAGS} $(DEBUG) ${LDFLAGS} $+

${TEST_BUILD_DIR}/test.%.o: ${TEST_DIR}/%.c $(HEADERS)
	$(Q)mkdir -p $(dir $@)
	@echo "===> CC $@"
	$(Q)$(CC) -o $@ -c ${CFLAGS} ${DEBUG} $<

${BUILD_DIR}/%.o: ${SRC_DIR}/%.c
	$(Q)mkdir -p $(dir $@)
	@echo "===> CC $@"
	$(Q)$(CC) -o $@ -c ${CFLAGS} $<

check:  $(TESTER_NAME) $(BIN_NAME)
	@echo "===> CHECK"
	$(Q)$(TEST_SCRIPT)

clean:
	@echo "===> CLEAN"
	$(Q)rm -rf ${BIN_DIR} ${BUILD_DIR}

