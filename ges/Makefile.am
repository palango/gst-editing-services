built_header_make =
built_source_make =

lib_LTLIBRARIES = libges-@GST_API_VERSION@.la

EXTRA_libges_@GST_API_VERSION@_la_SOURCES = gesmarshal.list

CLEANFILES = $(BUILT_SOURCES) $(built_header_make) $(built_source_make)

libges_@GST_API_VERSION@_la_SOURCES = 		\
	$(built_source_make)			\
	ges.c					\
	ges-enums.c				\
	ges-custom-timeline-source.c		\
	ges-metadata-container.c        \
	ges-simple-timeline-layer.c		\
	ges-timeline.c				\
	ges-timeline-layer.c			\
	ges-timeline-object.c			\
	ges-timeline-pipeline.c			\
	ges-timeline-source.c			\
	ges-timeline-effect.c		\
	ges-timeline-parse-launch-effect.c		\
	ges-timeline-file-source.c		\
	ges-timeline-operation.c		\
	ges-timeline-transition.c		\
	ges-timeline-standard-transition.c	\
	ges-timeline-test-source.c		\
	ges-timeline-title-source.c		\
	ges-timeline-overlay.c			\
	ges-timeline-text-overlay.c		\
	ges-track.c				\
	ges-track-object.c			\
	ges-track-source.c			\
	ges-track-operation.c			\
	ges-track-filesource.c			\
	ges-track-image-source.c		\
	ges-track-transition.c			\
	ges-track-audio-transition.c		\
	ges-track-video-transition.c		\
	ges-track-video-test-source.c		\
	ges-track-audio-test-source.c		\
	ges-track-title-source.c		\
	ges-track-text-overlay.c		\
	ges-track-effect.c		\
	ges-track-parse-launch-effect.c		\
	ges-screenshot.c			\
	ges-formatter.c				\
	ges-keyfile-formatter.c			\
	ges-pitivi-formatter.c			\
	ges-utils.c

libges_@GST_API_VERSION@includedir = $(includedir)/gstreamer-@GST_API_VERSION@/ges/
libges_@GST_API_VERSION@include_HEADERS = 	\
	$(built_header_make)			\
	ges-types.h				\
	ges.h					\
	ges-enums.h				\
	ges-custom-timeline-source.h		\
	ges-metadata-container.h        \
	ges-simple-timeline-layer.h		\
	ges-timeline.h				\
	ges-timeline-layer.h			\
	ges-timeline-object.h			\
	ges-timeline-pipeline.h			\
	ges-timeline-source.h			\
	ges-timeline-file-source.h		\
	ges-timeline-effect.h		\
	ges-timeline-parse-launch-effect.h		\
	ges-timeline-operation.h		\
	ges-timeline-transition.h		\
	ges-timeline-standard-transition.h	\
	ges-timeline-test-source.h		\
	ges-timeline-title-source.h		\
	ges-timeline-overlay.h			\
	ges-timeline-text-overlay.h		\
	ges-track-effect.h		\
	ges-track-parse-launch-effect.h		\
	ges-track.h				\
	ges-track-object.h			\
	ges-track-source.h			\
	ges-track-operation.h			\
	ges-track-filesource.h			\
	ges-track-image-source.h		\
	ges-track-transition.h			\
	ges-track-audio-transition.h		\
	ges-track-video-transition.h		\
	ges-track-video-test-source.h		\
	ges-track-audio-test-source.h		\
	ges-track-title-source.h		\
	ges-track-text-overlay.h		\
	ges-screenshot.h			\
	ges-formatter.h				\
	ges-keyfile-formatter.h			\
	ges-pitivi-formatter.h			\
	ges-utils.h

noinst_HEADERS = \
	ges-internal.h

libges_@GST_API_VERSION@_la_CFLAGS = -I$(top_srcdir) $(GST_PBUTILS_CFLAGS) \
		$(GST_VIDEO_CFLAGS) $(GST_CONTROLLER_CFLAGS) $(GST_PLUGINS_BASE_CFLAGS) \
		$(GST_CFLAGS) $(XML_CFLAGS) $(GIO_CFLAGS)
libges_@GST_API_VERSION@_la_LIBADD = $(GST_PBUTILS_LIBS) \
		$(GST_VIDEO_LIBS) $(GST_CONTROLLER_LIBS) $(GST_PLUGINS_BASE_LIBS) \
		$(GST_BASE_LIBS) $(GST_LIBS) $(XML_LIBS) $(GIO_LIBS)
libges_@GST_API_VERSION@_la_LDFLAGS = $(GST_LIB_LDFLAGS) $(GST_ALL_LDFLAGS) \
		$(GST_LT_LDFLAGS) $(GIO_CFLAGS)

DISTCLEANFILE = $(CLEANFILES)

#files built on make all/check/instal
BUILT_SOURCES =			\
	$(built_header_make)	\
	$(built_source_make)

include $(top_srcdir)/common/gst-glib-gen.mak

if HAVE_INTROSPECTION
BUILT_GIRSOURCES = GES-@GST_API_VERSION@.gir

gir_headers=$(patsubst %,$(srcdir)/%, $(libges_@GST_API_VERSION@include_HEADERS))
gir_headers+=$(patsubst %,$(builddir)/%, $(nodist_libges_@GST_API_VERSION@include_HEADERS))
gir_sources=$(patsubst %,$(srcdir)/%, $(libges_@GST_API_VERSION@_la_SOURCES))
gir_sources+=$(patsubst %,$(builddir)/%, $(nodist_libges_@GST_API_VERSION@_la_SOURCES))
gir_cincludes=$(patsubst %,--c-include='ges/%',$(libges_@GST_API_VERSION@include_HEADERS))
gir_cincludes+=$(patsubst %,--c-include='ges/%',$(nodist_libges_@GST_API_VERSION@include_HEADERS))

GES-@GST_API_VERSION@.gir: $(INTROSPECTION_SCANNER) libges-@GST_API_VERSION@.la
	$(AM_V_GEN)PKG_CONFIG_PATH="$(GST_PKG_CONFIG_PATH)" \
		$(INTROSPECTION_SCANNER) -v --namespace GES \
		--nsversion=@GST_API_VERSION@ \
		--identifier-prefix=GES \
		--symbol-prefix=ges \
		--warn-all \
		$(gir_cincludes) \
		-I$(top_srcdir) \
		-I$(top_builddir) \
		--add-include-path=`$(PKG_CONFIG) --variable=girdir gstreamer-@GST_API_VERSION@` \
		--add-include-path=`$(PKG_CONFIG) --variable=girdir gstreamer-pbutils-@GST_API_VERSION@` \
		--add-include-path=`$(PKG_CONFIG) --variable=girdir gio-2.0` \
		--library=libges-@GST_API_VERSION@.la \
		--include=Gst-@GST_API_VERSION@ \
		--include=GstPbutils-@GST_API_VERSION@ \
		--libtool="$(top_builddir)/libtool" \
		--pkg gstreamer-@GST_API_VERSION@ \
		--pkg gstreamer-pbutils-@GST_API_VERSION@ \
		--pkg gio-2.0 \
		--pkg-export ges-@GST_API_VERSION@ \
		--add-init-section="gst_init(NULL, NULL);" \
		--add-init-section="ges_init(NULL, NULL);" \
		--output $@ \
		$(gir_headers) \
		$(gir_sources)

# INTROSPECTION_GIRDIR/INTROSPECTION_TYPELIBDIR aren't the right place to
# install anything - we need to install inside our prefix.
girdir = $(datadir)/gir-1.0
gir_DATA = $(BUILT_GIRSOURCES)

typelibsdir = $(libdir)/girepository-1.0/

typelibs_DATA = $(BUILT_GIRSOURCES:.gir=.typelib)

%.typelib: %.gir $(INTROSPECTION_COMPILER)
	$(AM_V_GEN)PKG_CONFIG_PATH="$(GST_PKG_CONFIG_PATH)" \
		$(INTROSPECTION_COMPILER) \
		--includedir=$(srcdir) \
		--includedir=$(srcdir)/../video \
		--includedir=$(builddir) \
		--includedir=`$(PKG_CONFIG) --variable=girdir gstreamer-@GST_API_VERSION@` \
		--includedir=`$(PKG_CONFIG) --variable=girdir gstreamer-pbutils-@GST_API_VERSION@` \
		--includedir=`$(PKG_CONFIG) --variable=girdir gio-2.0` \
		$(INTROSPECTION_COMPILER_OPTS) $< -o $(@F)

CLEANFILES += $(BUILT_GIRSOURCES) $(typelibs_DATA)
endif

Android.mk: Makefile.am $(BUILT_SOURCES)
	androgenizer -:PROJECT libges -:SHARED libges-@GST_API_VERSION@ \
	 -:TAGS eng debug \
         -:REL_TOP $(top_srcdir) -:ABS_TOP $(abs_top_srcdir) \
	 -:SOURCES $(libges_@GST_API_VERSION@_la_SOURCES) \
	           $(nodist_libges_@GST_API_VERSION@_la_SOURCES) \
	 -:CFLAGS $(DEFS) $(libges_@GST_API_VERSION@_la_CFLAGS) \
	 -:LDFLAGS $(libges_@GST_API_VERSION@_la_LDFLAGS) \
	           $(libges_@GST_API_VERSION@_la_LIBADD) \
	           -ldl \
	 -:HEADER_TARGET gstreamer-@GST_API_VERSION@/gst/interfaces \
	 -:HEADERS $(libges_@GST_API_VERSION@include_HEADERS) \
	           $(nodist_libges_@GST_API_VERSION@include_HEADERS) \
	 -:PASSTHROUGH LOCAL_ARM_MODE:=arm \
	> $@
