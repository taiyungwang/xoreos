dnl xoreos - A reimplementation of BioWare's Aurora engine
dnl
dnl xoreos is the legal property of its developers, whose names
dnl can be found in the AUTHORS file distributed with this source
dnl distribution.
dnl
dnl xoreos is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU General Public License
dnl as published by the Free Software Foundation; either version 3
dnl of the License, or (at your option) any later version.
dnl
dnl xoreos is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with xoreos. If not, see <http://www.gnu.org/licenses/>.

AC_PREREQ([2.65])

AC_INIT([xoreos],[0.0.6],[xoreos-devel@xoreos.org],[xoreos],[https://xoreos.org/])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE([1.11 foreign -Wall -Wno-portability no-dist-gzip dist-xz subdir-objects])
dnl Support silent build rules. Disable by either passing
dnl --disable-silent-rules to configure or passing V=1 to make
AM_SILENT_RULES([yes])

dnl If AM_PROG_AR exists, call it, to shut up a libtool warning (and make it more portable, I guess?)
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

dnl libtool
LT_PREREQ([2.2])
LT_INIT([disable-shared])

dnl We're C++
AC_PROG_CXX
AC_LANG([C++])

dnl We want a make install
AC_PROG_INSTALL

dnl Endianness
AC_C_BIGENDIAN

dnl Special variables of the size of pointers
AC_TYPE_INTPTR_T
AC_TYPE_UINTPTR_T

dnl Check the sizes of integer types
AC_CHECK_SIZEOF([char])
AC_CHECK_SIZEOF([short])
AC_CHECK_SIZEOF([int])
AC_CHECK_SIZEOF([long])
AC_CHECK_SIZEOF([long long])
AC_CHECK_SIZEOF([void *])

dnl math library and special float functions
AC_CHECK_LIB([m], [cos])
AC_CHECK_FUNCS([fabs])
AC_CHECK_FUNCS([fmin])
AC_CHECK_FUNCS([fmax])
AC_CHECK_FUNCS([fabsf])
AC_CHECK_FUNCS([fminf])
AC_CHECK_FUNCS([fmaxf])

dnl Check for strcasecmp
AC_CHECK_FUNCS([strcasecmp])

dnl Check for -ggdb support
AX_CHECK_COMPILER_FLAGS_VAR([C++], [GGDB], [-ggdb])

AC_SUBST(GGDB)

dnl --with-werror
AC_ARG_WITH([werror], [AS_HELP_STRING([--with-werror], [Compile with -Werror @<:@default=no@:>@])], [], [with_werror=no])

WERROR=""
if test "x$with_werror" = "xyes"; then
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WERROR], [-Werror])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WERROR], [-Werror=unused-but-set-variable])
fi

AC_SUBST(WERROR)

dnl Compiler warning flags
AC_ARG_WITH([warnings], [AS_HELP_STRING([--without-warnings], [Compile without extra warnings @<:@default=no@:>@])], [], [with_warnings=yes])

WARN=""
if test "x$with_warnings" = "xyes"; then
	WARN="-Wall"

	dnl Extra warnings valid for both C and C++
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wignored-qualifiers])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wpointer-arith])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wshadow])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wsign-compare])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wtype-limits])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wuninitialized])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wunused-parameter])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wunused-but-set-parameter])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wduplicated-cond])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wduplicated-branches])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wlogical-op])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wshift-negative-value])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wshift-overflow=2])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wimplicit-fallthrough])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wvla])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wcast-function-type])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wnull-pointer-subtraction])

	dnl Extra warnings valid for C++
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wnon-virtual-dtor])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wdeprecated-copy])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wdeprecated-copy-dtor])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wredundant-move])
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wctad-maybe-unsupported])
fi

dnl Disable warnings about hiding virtual functions from a base class. We don't care
AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Woverloaded-virtual], [-Wno-overloaded-virtual])

dnl Disable warnings about the long long type. We need it
AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wlong-long], [-Wno-long-long])
AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wc++11-long-long], [-Wno-c++11-long-long])

dnl clang is far too trigger-happy on this warning, so let's disable it
AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wundefined-var-template], [-Wno-undefined-var-template])

dnl We do not yet mark any of our overrides. Let's disable this for now
AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Winconsistent-missing-override], [-Wno-inconsistent-missing-override])

dnl We're far from stable, so we still have several of those
AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wunused-private-field], [-Wno-unused-private-field])

dnl Disable warnings about unused local typedefs that came with GLM
AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wunused-local-typedefs], [-Wno-unused-local-typedefs])

dnl Disable warnings about unused static const variables
AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wunused-const-variable], [-Wno-unused-const-variable])

dnl Disable warnings about >= 0 comparisons on enums
AX_CHECK_COMPILER_FLAGS_VAR([C++], [WARN], [-Wtautological-unsigned-enum-zero-compare], [-Wno-tautological-unsigned-enum-zero-compare])

AC_SUBST(WARN)

dnl C++ dialect
AC_ARG_WITH([std], [AS_HELP_STRING([--without-std], [Compile without specifying the C++ standard to follow @<:@default=no@:>@])], [], [with_std=yes])

STD=""
if test "x$with_std" = "xyes"; then
	case "$target" in
		*mingw*)
			dnl Needed by _wfopen in src/common/platform.cpp for mingw32 (but technically not for mingw-w64)
			AX_CHECK_COMPILER_FLAGS_VAR([C++], [STD], [-std=gnu++17])
			;;
		*)
			AX_CHECK_COMPILER_FLAGS_VAR([C++], [STD], [-std=c++17])
			;;
	esac;
fi

BOOST_STD="$STD"

AC_SUBST(STD)
AC_SUBST(BOOST_STD)

dnl Link-time optimization
AC_ARG_WITH([lto], [AS_HELP_STRING([--with-lto], [Compile with link-time optimization @<:@default=no@:>@])], [], [with_lto=no])

LTO=""
if test "x$with_lto" = "xyes"; then
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [LTO], [-flto])
fi

AC_SUBST(LTO)

dnl Optimize against local CPU
AC_ARG_WITH([native], [AS_HELP_STRING([--with-native], [Pass -march=native to the compiler to optimize directly against the local CPU instead of generating a more generic build @<:@default=no@:>@])], [], [with_native=no])

NATIVE=""
if test "x$with_native" = "xyes"; then
	AX_CHECK_COMPILER_FLAGS_VAR([C++], [NATIVE], [-march=native])
fi

AC_SUBST(NATIVE)

dnl Release version number
AC_ARG_WITH([release], [AS_HELP_STRING([--with-release=VER], [Set the version suffix to VER instead of the git revision. If no VER is given, do not add a version suffix at all])], [], [with_release=no])

if test "x$with_release" != "xno"; then
	AC_DEFINE([XOREOS_RELEASE], 1, [Define to 1 if we are in release version mode])

	if test "x$with_release" != "xyes"; then
		AC_DEFINE_UNQUOTED([XOREOS_DISTRO], ["+$with_release"], [Release version suffix])
	fi
fi

dnl Force compiling against the internal GLEW library
AC_ARG_ENABLE([external-glew], [AS_HELP_STRING([--disable-external-glew], [Do not check for an external GLEW library and always compile against the internal GLEW library @<:@default=no@:>@])], [], [enable_external_glew=yes])

dnl Force imgui to use GLEW instead of GL3W
IMGUI_CFLAGS="-DIMGUI_IMPL_OPENGL_LOADER_GLEW"
AC_SUBST(IMGUI_CFLAGS)

dnl Disable optional external libraries
AC_ARG_ENABLE([vorbis], [AS_HELP_STRING([--disable-vorbis], [Disable building with Vorbis support @<:@default=no@:>@])], [], [enable_vorbis=yes])
AC_ARG_ENABLE([mad], [AS_HELP_STRING([--disable-mad], [Disable building with MP3 support @<:@default=no@:>@])], [], [enable_mad=yes])
AC_ARG_ENABLE([faad], [AS_HELP_STRING([--disable-faad], [Disable building with AAC support @<:@default=no@:>@])], [], [enable_faad=yes])
AC_ARG_ENABLE([xvidcore], [AS_HELP_STRING([--disable-xvidcore], [Disable building with H.263 support @<:@default=no@:>@])], [], [enable_xvidcore=yes])
AC_ARG_ENABLE([vpx], [AS_HELP_STRING([--disable-vpx], [Disabling building with VP8/VP9 support @<:@default=no@:>@])], [], [enable_vpx=yes])
AC_ARG_ENABLE([lzma], [AS_HELP_STRING([--disable-lzma], [Disable building with LZMA support @<:@default=no@:>@])], [], [enable_lzma=yes])
AC_ARG_ENABLE([xml], [AS_HELP_STRING([--disable-xml], [Disable building with XML support @<:@default=no@:>@])], [], [enable_xml=yes])

dnl General purpose libraries
AX_CHECK_ICONV( , AC_MSG_ERROR([No useable iconv() function found!]))
AX_CHECK_ZLIB(1, 2, 3, 0, , AC_MSG_ERROR([zlib(>= 1.2.3) is required and could not be found!]))
if test "x$enable_lzma" = "xyes"; then
	AX_CHECK_LZMA(5, 0, 3, 2, , [enable_lzma=no])
fi
if test "x$enable_lzma" = "xyes"; then
	AC_DEFINE([ENABLE_LZMA], 1, [Defined to 1 if we are using liblzma])
	AM_CONDITIONAL(ENABLE_LZMA, true)
else
	AM_CONDITIONAL(ENABLE_LZMA, false)
	LZMA_CFLAGS=""
	LZMA_LIBS=""
fi
if test "x$enable_xml" = "xyes"; then
	AX_CHECK_XML2(2, 8, 0, , [enable_xml=no])
fi
if test "x$enable_xml" = "xyes"; then
	AC_DEFINE([ENABLE_XML], 1, [Defined to 1 if we are using libxml2])
	AM_CONDITIONAL(ENABLE_XML, true)
else
	AM_CONDITIONAL(ENABLE_XML, false)
	XML2_CFLAGS=""
	XML2_LIBS=""
fi

dnl Use Wincrypt instead of BCrypt in Boost.Uuid
AC_ARG_WITH([boost-uuid-wincrypt], [AS_HELP_STRING([--with-boost-uuid-wincrypt], [Make Boost.Uuid use Wincrypt instead of BCrypt on Windows @<:@default=no@:>@])], [], [with_boost_uuid_wincrypt=no])

BOOST_UUID_WINCRYPT=""
if test "x$with_boost_uuid_wincrypt" = "xyes"; then
	BOOST_UUID_WINCRYPT="-DBOOST_UUID_RANDOM_PROVIDER_FORCE_WINCRYPT"
fi

AC_SUBST(BOOST_UUID_WINCRYPT)

dnl Force default initializations in glm
GLM_FORCE_CTOR_INIT="-DGLM_FORCE_CTOR_INIT"
AC_SUBST(GLM_FORCE_CTOR_INIT)

dnl GLEW
if test "x$enable_external_glew" = "xyes"; then
	AX_CHECK_GLEW( , [enable_external_glew=no])
fi

if test "x$enable_external_glew" = "xyes"; then
	AC_MSG_NOTICE([using external GLEW library])

	AM_CONDITIONAL(INTERNAL_GLEW, false)
else
	AC_MSG_NOTICE([using internal GLEW library])

	GLEW_CFLAGS="-DGLEW_NO_GLU -DGLEW_STATIC"
	GLEW_LIBS=""

	AC_SUBST(GLEW_CFLAGS)
	AC_SUBST(GLEW_LIBS)

	AC_DEFINE([XOREOS_INTERNAL_GLEW], 1, [Defined to 1 if we are using our internal GLEW sources])
	AM_CONDITIONAL(INTERNAL_GLEW, true)
fi

dnl Graphic libraries
AX_CHECK_SDL2(2004, , AC_MSG_ERROR([SDL2 (>= 2.0.0) is required and could not be found!]))
AX_CHECK_GL( , AC_MSG_ERROR([OpenGL is required and could not be found!]))
AX_CHECK_FT2([11.0.5], , AC_MSG_ERROR([FreeType2 (>= 11.0.5) is required and could not be found!]))
if test "x$enable_xvidcore" = "xyes"; then
	AX_CHECK_XVID(1, 2, 2, , [enable_xvidcore=no])
fi
if test "x$enable_xvidcore" = "xyes"; then
	AC_DEFINE([ENABLE_XVIDCORE], 1, [Defined to 1 if we are using libxvidcore])
	AM_CONDITIONAL(ENABLE_XVIDCORE, true)
else
	XVID_CFLAGS=""
	XVID_LIBS=""
	AM_CONDITIONAL(ENABLE_XVIDCORE, false)
fi
if test "x$enable_vpx" = "xyes"; then
	AX_CHECK_VPX(4, , [enable_vpx=no])
fi
if test "x$enable_vpx" = "xyes"; then
	AC_DEFINE([ENABLE_VPX], 1, [Defined to 1 if we are using libvpx])
	AM_CONDITIONAL(ENABLE_VPX, true)
else
	VPX_CFLAGS=""
	VPX_LIBS=""
	AM_CONDITIONAL(ENABLE_VPX, false)
fi

dnl Sound libraries
AX_CHECK_AL([1.12],  , AC_MSG_ERROR([OpenAL Soft (>= 1.12) is required and could not be found!]))
if test "x$enable_faad" = "xyes"; then
	AX_CHECK_FAAD(, [enable_faad=no])
fi
if test "x$enable_faad" = "xyes"; then
	AC_DEFINE([ENABLE_FAAD], 1, [Defined to 1 if we are using libfaad])
	AM_CONDITIONAL(ENABLE_FAAD, true)
else
	FAAD_CFLAGS=""
	FAAD_LIBS=""
	AM_CONDITIONAL(ENABLE_FAAD, false)
fi
if test "x$enable_mad" = "xyes"; then
	AX_CHECK_MAD(0, 15, 1, , [enable_mad=no])
fi
if test "x$enable_mad" = "xyes"; then
	AC_DEFINE([ENABLE_MAD], 1, [Defined to 1 if we are using libmad])
	AM_CONDITIONAL(ENABLE_MAD, true)
else
	MAD_CFLAGS=""
	MAD_LIBS=""
	AM_CONDITIONAL(ENABLE_MAD, false)
fi
if test "x$enable_vorbis" = "xyes"; then
	AX_CHECK_OGG(, [enable_vorbis=no])
	AX_CHECK_VORBIS(, [enable_vorbis=no])
fi
if test "x$enable_vorbis" = "xyes"; then
	AC_DEFINE([ENABLE_VORBIS], 1, [Defined to 1 if we are using libvorbis])
	AM_CONDITIONAL(ENABLE_VORBIS, true)
else
	OGG_CFLAGS=""
	OGG_LIBS=""
	VORBIS_CFLAGS=""
	VORBIS_LIBS=""
	AM_CONDITIONAL(ENABLE_VORBIS, false)
fi

dnl Boost
BOOST_REQUIRE(1.53.0)
BOOST_UTILITY
BOOST_STRING_ALGO
BOOST_SYSTEM
BOOST_FILESYSTEM
BOOST_DATE_TIME
BOOST_UUID([${BOOST_UUID_WINCRYPT}])
BOOST_SCOPE_EXIT
BOOST_LOCALE
BOOST_TYPETRAITS

dnl pthread
AX_PTHREAD()
AM_CONDITIONAL([HAVE_PTHREAD], [test x"$ax_pthread_ok" = xyes])

AC_DEFUN([AX_ADD_ENGINE], [
AC_ARG_ENABLE([$1], [AS_HELP_STRING([--disable-$1], [Disable building with support for $3 @<:@default=no@:>@])], [], [enable_$1=yes])
if test "x$enable_$1" = "xyes"; then
	missing_$1=""
	for i in $4; do
		if test "x$(eval printf '%s' "\${enable_${i}}")" != "xyes"; then
			enable_$1=no
			missing_$1="$missing_$1 $i"
		fi
	done
	if test "x$enable_$1" != "xyes"; then
		AC_MSG_NOTICE([Disabling $3 as the following dependencies are missing: $missing_$1])
	fi
fi
if test "x$enable_$1" = "xyes"; then
	AC_DEFINE([ENABLE_$2], 1, [Defined to 1 if we are building with support for $3])
	AM_CONDITIONAL(ENABLE_$2, true)
	ENGINE_SUMMARY="$ENGINE_SUMMARY
	$3: yes"
else
	AM_CONDITIONAL(ENABLE_$2, false)
	ENGINE_SUMMARY="$ENGINE_SUMMARY
	$3: no"
fi
])

AX_ADD_ENGINE([nwn], [NWN], [Neverwinter Nights], [])
AX_ADD_ENGINE([nwn2], [NWN2], [Neverwinter Nights 2], [])
AX_ADD_ENGINE([kotor], [KOTOR], [Star Wars: Knights of the Old Republic], [lzma])
AX_ADD_ENGINE([kotor2], [KOTOR2], [Star Wars: Knights of the Old Republic II: The Sith Lords], [kotor])
AX_ADD_ENGINE([jade], [JADE], [Jade Empire], [])
AX_ADD_ENGINE([witcher], [WITCHER], [The Witcher], [])
AX_ADD_ENGINE([sonic], [SONIC], [Sonic Chronicles: The Dark Brotherhood], [])
AX_ADD_ENGINE([dragonage], [DRAGONAGE], [Dragon Age: Origins], [xml])
AX_ADD_ENGINE([dragonage2], [DRAGONAGE2], [Dragon Age II], [xml])

dnl Extra flags
case "$target" in
	*darwin*)
		XOREOS_CFLAGS="-DUNIX -DMACOSX"
		XOREOS_LIBS=""
		AM_CONDITIONAL(WIN32, false)
		;;
	*mingw*)
		AC_CHECK_TOOL([RC], [windres], [:])
		if test "x$RC" = "x:"; then
			AC_MSG_ERROR([A Windows resource compiler (windres) is required and could not be found!])
		fi

		XOREOS_CFLAGS="-mconsole"
		XOREOS_LIBS=""
		AM_CONDITIONAL(WIN32, true)
		;;
	*)
		XOREOS_CFLAGS="-DUNIX"
		XOREOS_LIBS=""
		AM_CONDITIONAL(WIN32, false)
		;;
esac;

dnl Force BOOST_SCOPE_EXIT to behave in lambda mode
XOREOS_CFLAGS="$XOREOS_CFLAGS -DBOOST_SCOPE_EXIT_CONFIG_USE_LAMBDAS"

AC_SUBST(XOREOS_CFLAGS)
AC_SUBST(XOREOS_LIBS)

AC_CONFIG_FILES([Makefile])

AC_OUTPUT

AS_ECHO([])
AS_ECHO(["Enabled engines: $ENGINE_SUMMARY"])
AS_ECHO([])
AS_ECHO(["Enabled libraries:"])
AS_ECHO(["	libmad: $enable_mad"])
AS_ECHO(["	libvorbis: $enable_vorbis"])
AS_ECHO(["	libfaad: $enable_faad"])
AS_ECHO(["	xvidcore: $enable_xvidcore"])
AS_ECHO(["	libvpx: $enable_vpx"])
AS_ECHO(["	liblzma: $enable_lzma"])
AS_ECHO(["	libxml2: $enable_xml"])
