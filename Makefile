CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra

# Change the following path to the location where the yaml-cpp library
# is installed. On macOS, you can find this path by typing:
# brew --prefix yaml-cpp
# in the terminal.
YAML_CPP_PREFIX := /opt/homebrew/opt/yaml-cpp

TARGET := qcomm
RCG_TARGET := rcg

OBJDIR := obj

MODULES := main architecture noc circuit communication teleportation_time core gate mapping parameters statistics utils simulation command_line
OBJS := $(addprefix $(OBJDIR)/,$(addsuffix .o,$(MODULES)))

RCG_MODULES := rcg circuit gate utils
RCG_OBJS := $(addprefix $(OBJDIR)/,$(addsuffix .o,$(RCG_MODULES)))

DEPS := $(OBJS:.o=.d)
RCG_DEPS := $(RCG_OBJS:.o=.d)

all: $(TARGET) $(RCG_TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -L$(YAML_CPP_PREFIX)/lib -lyaml-cpp

$(RCG_TARGET): $(RCG_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -L$(YAML_CPP_PREFIX)/lib -lyaml-cpp

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(YAML_CPP_PREFIX)/include -MMD -MP -c $< -o $@

-include $(DEPS)
-include $(RCG_DEPS)

clean:
	rm -rf $(OBJDIR) $(TARGET) $(RCG_TARGET)

rebuild: clean all

.PHONY: all clean rebuild
