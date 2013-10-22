PPA=ppa:krlmlr/ppa

ARCHS=all
ARCH_DIRS=$(patsubst %,%.arch,$(ARCHS))

all: changes

changes: clean FORCE
	cd pkg && \
        debuild -S -sd

upload:
	dput $(PPA) *.changes && make clean

clean: FORCE
	git clean -f -d

FORCE:
