procevt: procevt.cxx
	g++ procevt.cxx -o procevt -fPIC \
	  -I${KEY4HEP_PODIO}/include \
	  -I${KEY4HEP_EDM4HEP}/include \
	  `root-config --cflags --libs` \
	  -L${KEY4HEP_PODIO}/lib64 -lpodio -lpodioRootIO \
	  -L${KEY4HEP_EDM4HEP}/lib64 -ledm4hep
clean:
	@rm -rf procevt
