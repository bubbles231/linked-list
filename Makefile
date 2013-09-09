# the default compiler: gcc
  CC = gcc

  CFLAGS  = -g -Wall

  # the build target executable:
  TARGET = ll

  all: $(TARGET)

  $(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

  # clean just the executable
  clean:
	$(RM) $(TARGET)

  # clean the executable and modified src files
  clean-all:
	$(RM) $(TARGET) && $(RM) *~
