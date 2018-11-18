# ----------------------------
# Set NAME to the program name
# Set ICON to the png icon file name
# Set DESCRIPTION to display within a compatible shell
# Set COMPRESSED to "YES" to create a compressed program
# ----------------------------

NAME        ?= CHKNADV
COMPRESSED  ?= YES
ICON        ?= iconc.png
DESCRIPTION ?= "The chicken adventure I"

# ----------------------------

include $(CEDEV)/include/.makefile
