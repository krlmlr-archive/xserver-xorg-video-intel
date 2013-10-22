PPA=ppa:krlmlr/ppa
SOURCE=xserver-xorg-video-intel

ARCHS=all
ARCH_DIRS=$(patsubst %,%.arch,$(ARCHS))

all: changes

reset-upstream: clean FORCE
	git checkout upstream
	git reset HEAD^
	git checkout .
	git checkout master
	make clean

update-upstream: clean FORCE
	git checkout upstream
	apt-get source ${SOURCE}
	rm -rf pkg
	mv ${SOURCE}-* pkg
	git add -A
	git commit -m "fetch from upstream"
	git checkout master
	git merge upstream || true
	sed -r -i '/^<<<<<<</d;/^>>>>>>>/d;s-^=======.*$$--' pkg/debian/changelog
	nano pkg/debian/changelog
	git add pkg/debian/changelog
	git commit

changes: clean FORCE
	cd pkg && \
        debuild -S -sd

upload:
	dput $(PPA) *.changes && make clean

clean: FORCE
	git clean -f -d

FORCE:
