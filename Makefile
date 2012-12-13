NAME=posix-lib
VERSION=0.8
INSTALL_DIR=usr/local/include/mrr/$(NAME)

install:
	mkdir -p $(DESTDIR)/$(INSTALL_DIR)
	cp *.hxx $(DESTDIR)/$(INSTALL_DIR)

package:
	rm -f ../$(NAME)-$(VERSION).tar.gz
	tar cvzf ../$(NAME)-$(VERSION).tar.gz -C ../ $(NAME)/*.hxx
