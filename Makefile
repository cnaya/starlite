PROJECT=blog
MOD=mod/$(PROJECT).so

MODSRC=$(wildcard   src/*.cpp \
		    src/help/*.cpp \
		    \)
MODOBJ=$(patsubst %.cpp,%.o,$(MODSRC))

CC=g++
CXXFLAGS+=-O3 -std=c++11 -fPIC -Wall -I./include  `pkg-config --cflags opencv cryptopp`
LIBS+=-lPocoDataMySQL -lPocoData -lPocoJSON -lPocoNet -lPocoUtil -lPocoFoundation
LIBS+=`pkg-config --libs opencv cryptopp`
LIBS+=-lqrencode 
LDFLAGS+=-shared


TPLDIR=/var/webcppd/tpl
WWWDIR=/var/webcppd/www

all:$(MOD)

$(MOD):$(MODOBJ)
	$(CC) -o $@ $^ $(CXXFLAGS) $(LIBS) $(LDFLAGS)

clean:
	rm -f  $(MODOBJ) $(MOD)


install:
	install mod/*.so /var/webcppd/mod
	install etc/route.conf /etc/webcppd
	mkdir -pv $(WWWDIR)/assets/$(PROJECT)
	cp -Ru assets/assets    $(WWWDIR)/
	cp -Ru  assets/blog $(WWWDIR)/assets/
	mkdir -pv $(TPLDIR)/$(PROJECT)
	install tpl/*.* $(TPLDIR)/$(PROJECT)

