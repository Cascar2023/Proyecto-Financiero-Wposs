CFLAGS := -std=c11 -Wall -Wextra -g -I.
SRCDIR := .
BUILDDIR := build
OBJDIR := $(BUILDDIR)/obj
TARGET := $(BUILDDIR)/Debug/outDebug.exe

SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))

.PHONY: all clean run

all: $(TARGET)

# Enlaza
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

# Compila cada fuente a objeto
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	@echo "Ejecutando $(TARGET)..."
	@"$(TARGET)"

clean:
	-rm -rf $(BUILDDIR)