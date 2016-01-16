CXXFLAGS ?= -O2 -Wall -g
CXXFLAGS += `pkg-config --cflags sdl2 SDL2_image SDL2_mixer`
LIBS = `pkg-config --libs sdl2 SDL2_image SDL2_mixer`
OBJS = alien.o audio.o bullet.o cargo.o collectable.o colors.o cutscene.o engine.o event.o explosion.o game.o gfx.o intermission.o loadSave.o messages.o misc.o missions.o player.o renderer.o resources.o screen.o ship.o shop.o Starfighter.o title.o weapons.o window.o

VERSION = 1.5.1
PROG = starfighter
DOCS = docs/*
DATA = data gfx sound music
DATAFILES = data/* gfx/* sound/* music/*
LAUNCHER = misc/starfighter.desktop
ICON = misc/starfighter.png

PREFIX ?= /usr
BINDIR ?= $(PREFIX)/games/
DATADIR ?= $(PREFIX)/share/games/parallelrealities/
DOCDIR ?= $(PREFIX)/share/doc/$(PROG)/
MENUDIR ?= $(PREFIX)/share/applications/
ICONDIR ?= $(PREFIX)/share/pixmaps/

# top-level rule to create the program.
ALL = $(PROG)

all: $(ALL)

# compiling other source files.
%.o: src/%.cpp src/*.h
	$(CXX) $(CXXFLAGS) -c -DVERSION=\"$(VERSION)\" -DDATADIR=\"$(DATADIR)\" $<

# linking the program.
$(PROG): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -o $(PROG) $(LIBS)

# cleaning everything that can be automatically recreated with "make".
clean:
	$(RM) $(OBJS) $(ALL)

# install
install: $(ALL)
	mkdir -p $(DESTDIR)$(BINDIR)
	mkdir -p $(DESTDIR)$(DATADIR)
	mkdir -p $(DESTDIR)$(DOCDIR)
	mkdir -p $(MENUDIR)
	mkdir -p $(ICONDIR)

	install -m 755 $(PROG) $(DESTDIR)$(BINDIR)$(PROG)
	cp -r $(DATA) $(DESTDIR)$(DATADIR)
	cp $(DOCS) $(DESTDIR)$(DOCDIR)
	cp $(LAUNCHER) $(MENUDIR)
	cp $(ICON) $(ICONDIR)

optimise:
	advpng -z gfx/*.png
	jpegoptim --strip-all gfx/*.jpg

dist:
	rm -rf starfighter-$(VERSION)
	mkdir starfighter-$(VERSION)
	cp --parents -lt starfighter-$(VERSION) `git ls-files`
	git log >starfighter-$(VERSION)/ChangeLog
	tar czf starfighter-$(VERSION).tar.gz starfighter-$(VERSION)
	rm -rf starfighter-$(VERSION)

.PHONY: all clean install optimise dist
